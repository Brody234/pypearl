#ifndef DIRECTIMP
#define DIRECTIMP

#include "directproduct.hpp"

static void direct_dealloc(direct *self)
{    
    for(int64_t i = 0; i < self->len; i++){
        Py_DECREF(self->values[i].component);
    }
    free(self->values);

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * direct_str(direct *self){

    PyObject *parts = PyList_New(0);
    if (!parts) return NULL;

    for(int64_t i = 0; i < self->len; i++){
        PyObject* s = nullptr;
        if(self->values[i].type == 0x0){
            s= znz_str((znz*)self->values[i].component);
        }
        else if(self->values[i].type == 0x1){
            s=dihedral_str((dihedral*)self->values[i].component);
        }
        else if(self->values[i].type == 0x2){
            s=dihedral_matrix_str((dihedral_matrix*)self->values[i].component);
        }
        else if(self->values[i].type == 0x3){
            s=symmetric_str((symmetric*)self->values[i].component);
        }
        else{
            PyErr_SetString(PyExc_TypeError, "Type not found for an element");
            Py_DECREF(parts);
            return nullptr;
        }
        if (PyErr_Occurred()) {
            Py_DECREF(parts);

            // string will be set in the add_new that failed
            return nullptr;
        }

        if (PyList_Append(parts, s) < 0) {
            Py_DECREF(s);
            Py_DECREF(parts);
            return NULL;
        }
        Py_DECREF(s);
    }

    PyObject *sep = PyUnicode_FromString("\n\n");
    if (!sep) {
        Py_DECREF(parts);
        return NULL;
    }

    PyObject *body = PyUnicode_Join(sep, parts);
    Py_DECREF(sep);
    Py_DECREF(parts);
    if (!body) return NULL;

    PyObject *header = PyUnicode_FromString(
        "Direct product of groups, comprised of the following elements:\n\n"
    );
    if (!header) {
        Py_DECREF(body);
        return NULL;
    }

    PyObject *out = PyUnicode_Concat(header, body);
    Py_DECREF(header);
    Py_DECREF(body);
    return out;
}

static PyObject * direct_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    (void)args;
    (void)kwds;
    direct *self = (direct*)type->tp_alloc(type, 0);

    return (PyObject*)self;
}

static int direct_init(direct *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = { (char*)"elements", NULL };
    
    PyObject* vals;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &vals))
        return -1;

    if(!vals || vals == Py_None){
        PyErr_SetString(PyExc_TypeError, "Must pass elements as a tuple of group elements");
        return -1;
    }

    if (!PyTuple_Check(vals)) {
        PyErr_SetString(PyExc_TypeError, "Must pass elements as a tuple of group elements");
        return -1;
    }

    Py_ssize_t length = PyTuple_GET_SIZE(vals);

    if(length <= 0){
        PyErr_SetString(PyExc_TypeError, "Must at least one element");
        return -1;
    }

    ssize_t len = (ssize_t)length;

    direct_element* values = (direct_element*)malloc(len*sizeof(direct_element));

    for(ssize_t i = 0; i < len; i++){
        PyObject* obj = PyTuple_GET_ITEM(vals, i);
        Py_INCREF(obj);

        if(PyObject_TypeCheck(obj, &symmetricType)){
            symmetric* input = (symmetric*) obj;
            
            ndarray* ordering = arrayCInitCopy(input->ordering);

            symmetric* elem = symmetricCInit(ordering, input->n);

            values[i].component = (PyObject*) elem;

            values[i].type = 0x3;
        }
        else if(PyObject_TypeCheck(obj, &dihedralType)){
            dihedral* input = (dihedral*) obj;
        
            dihedral* elem = dihedralCInit(input->r, input->n, input->s);

            values[i].component = (PyObject*) elem;

            values[i].type = 0x1;
        }
        else if(PyObject_TypeCheck(obj, &znzType)){
            znz* input = (znz*) obj;

            znz* elem = znzCInit(input->val, input->n);

            values[i].component = (PyObject*) elem;

            values[i].type = 0x0;
        }
        else if(PyObject_TypeCheck(obj, &dihedral_matrixType)){
            dihedral_matrix* input = (dihedral_matrix*) obj;
            
            ndarray* r_arr = arrayCInitCopy(input->r);

            ndarray* s_arr = arrayCInitCopy(input->s);

            dihedral_matrix* elem = dihedralMatrixCInit(r_arr, input->n, s_arr);

            values[i].component = (PyObject*) elem;

            values[i].type = 0x2;
        }
        else{
            PyErr_SetString(PyExc_TypeError, "All elements must be supported group elements. Refer to documentation.");
            return -1;
        }
    }

    self->values = values;

    self->len = len;

    return 0;
}

direct* directCInit(int64_t len, direct_element* values){
    direct* self = (direct*)directType.tp_alloc(&directType, 0);
    self->len = len;
    self->values = values;
    return self;
}

static PyObject* PyDirect_add_new(PyObject *a, PyObject *b){
    if(!PyObject_TypeCheck(a, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }
    if(!PyObject_TypeCheck(b, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }
    direct* d1 = (direct*) a;
    direct* d2 = (direct*) b;

    if(d1->len != d2->len){
        PyErr_SetString(PyExc_TypeError, "Cannot add groups that are stored with different lengths, even if they might technically be isomorphic.");
        return NULL;
    }

    for(int64_t i = 0; i < d1->len; i++){
        if(d1->values[i].type != d2->values[i].type){
            PyErr_SetString(PyExc_TypeError, "Group elements must line up by same type sequentially.");
            return NULL;
        }
    }

    direct_element* values = (direct_element*)malloc(d1->len*sizeof(direct_element));

    // the burden of throwing typing errors with proper type checks in this loop is on the add functions
    for(int64_t i = 0; i < d1->len; i++){
        if(d1->values[i].type == 0x0){
            values[i].component = Pyznz_add_new(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x1){
            values[i].component = PyDihedral_add_new(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x2){
            values[i].component = PyDihedral_Matrix_add_new(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x3){
            values[i].component = PySymmetric_add_new(d1->values[i].component, d2->values[i].component);
        }
        else{
            PyErr_SetString(PyExc_TypeError, "Type not found for an element");
        }
        if (PyErr_Occurred()) {
            // string will be set in the add_new that failed
            return nullptr;
        }
        values[i].type = d1->values[i].type;
    }
    
    direct* self = directCInit(d1->len, values);

    return (PyObject*) self;
}

static PyObject* PyDirect_add(PyObject *a, PyObject *b){
    if(!PyObject_TypeCheck(a, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }
    if(!PyObject_TypeCheck(b, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }
    direct* d1 = (direct*) a;
    direct* d2 = (direct*) b;

    if(d1->len != d2->len){
        PyErr_SetString(PyExc_TypeError, "Cannot add groups that are stored with different lengths, even if they might technically be isomorphic.");
        return NULL;
    }

    for(int64_t i = 0; i < d1->len; i++){
        if(d1->values[i].type != d2->values[i].type){
            PyErr_SetString(PyExc_TypeError, "Group elements must line up by same type sequentially.");
            return NULL;
        }
    }

    // the burden of throwing typing errors with proper type checks in this loop is on the add functions
    for(int64_t i = 0; i < d1->len; i++){
        if(d1->values[i].type == 0x0){
            Pyznz_add(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x1){
            PyDihedral_add(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x2){
            PyDihedral_Matrix_add(d1->values[i].component, d2->values[i].component);
        }
        else if(d1->values[i].type == 0x3){
            PySymmetric_add(d1->values[i].component, d2->values[i].component);
        }
        else{
            PyErr_SetString(PyExc_TypeError, "Type not found for an element");
        }
        if (PyErr_Occurred()) {
            // string will be set in the add_new that failed
            return nullptr;
        }
    }
    
    Py_INCREF(d1);
    return (PyObject*) d1;
}


static PyObject* PyDirect_richcompare(PyObject* a, PyObject* b, int op)
{
    if(!PyObject_TypeCheck(a, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }
    if(!PyObject_TypeCheck(b, &directType)){
        PyErr_SetString(PyExc_TypeError, "Both elements must be direct products of groups.");
        return NULL;
    }

    direct* d1 = (direct*) a;
    direct* d2 = (direct*) b;

    if (op != Py_EQ && op != Py_NE) {
        PyErr_SetString(PyExc_TypeError, "What does inequality even mean in a weird group?? Like actually email me.");
        Py_RETURN_NOTIMPLEMENTED;
    }
    int eq = 1;
    if(d1->len != d2->len){
        eq = 0;
    }
    else{
        for(size_t i = 0; i < d1->len; i++){
            if(d1->values[i].type != d2->values[i].type){
                eq = 0;
            }
            else{
                if(d1->values[i].type == 0x0){
                    if(PyBool_Check(Pyznz_richcompare(d1->values[i].component, d2->values[i].component, Py_NE))){
                        eq=0;
                    }
                }
                else if(d1->values[i].type == 0x1){
                    if(PyBool_Check(PyDihedral_richcompare(d1->values[i].component, d2->values[i].component, Py_NE))){
                        eq=0;
                    }
                }
                else if(d1->values[i].type == 0x2){
                    if(PyBool_Check(PyDihedral_Matrix_richcompare(d1->values[i].component, d2->values[i].component, Py_NE))){
                        eq=0;
                    }
                }
                else if(d1->values[i].type == 0x3){
                    if(PyBool_Check(PySymmetric_richcompare(d1->values[i].component, d2->values[i].component, Py_NE))){
                        eq=0;
                    }
                }
                else{
                    eq = 0;
                }
                if(PyErr_Occurred()){
                    return NULL;
                }
            }
        }
    }


    switch(op){
        case Py_EQ:
        return PyBool_FromLong(eq);
        break;
        case Py_NE:
        return PyBool_FromLong(!eq);
        break;
    }
}

PyMethodDef direct_methods[] = {
    /*{"div", (PyCFunction)PyNDArray_div, METH_O, "divides a number from an array"},*/
    {NULL, NULL, 0, NULL}
};

static PyMemberDef PyDirect_members[] = {
    {"len", T_LONG, offsetof(direct, len), READONLY, "group order parameter n"},
    {NULL}
};

static PyNumberMethods direct_as_number = {
    /* nb_add */                  PyDirect_add_new,        // a + b
    /* nb_subtract */             0,   // a - b
    /* nb_multiply */             0,   // a * b
    /* nb_remainder */            0,                  // a % b
    /* nb_divmod */               0,                  // divmod(a, b)
    /* nb_power */                0,                  // a ** b
    /* nb_negative */             0,   // -a
    /* nb_positive */             0,                  // +a
    /* nb_absolute */             0,                  // abs(a)
    /* nb_bool */                 0,                  // bool(a)
    /* nb_invert */               0,                  // ~a
    /* nb_lshift */               0,                  // a << b
    /* nb_rshift */               0,                  // a >> b
    /* nb_and */                  0,                  // a & b
    /* nb_xor */                  0,                  // a ^ b
    /* nb_or */                   0,                  // a | b
    /* nb_int */                  0,                  // int(a) / __int__
    /* nb_reserved */             0,                  // reserved / legacy
    /* nb_float */                0,                  // float(a) / __float__

    /* nb_inplace_add */          PyDirect_add,       // a += b
    /* nb_inplace_subtract */     0,       // a -= b
    /* nb_inplace_multiply */     0,       // a *= b
    /* nb_inplace_remainder */    0,                  // a %= b
    /* nb_inplace_power */        0,                  // a **= b
    /* nb_inplace_lshift */       0,                  // a <<= b
    /* nb_inplace_rshift */       0,                  // a >>= b
    /* nb_inplace_and */          0,                  // a &= b
    /* nb_inplace_xor */          0,                  // a ^= b
    /* nb_inplace_or */           0,                  // a |= b

    /* nb_floor_divide */         0,                  // a // b
    /* nb_true_divide */          0,    // a / b
    /* nb_inplace_floor_divide */ 0,                  // a //= b
    /* nb_inplace_true_divide */  0,   // a /= b

    /* nb_index */                0,                  // a.__index__()

    /* nb_matrix_multiply */      0,                  // a @ b
    /* nb_inplace_matrix_multiply */ 0                // a @= b
};

PyTypeObject directType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pypearl.DirectProduct",               // tp_name
    sizeof(direct),         // tp_basicsize
    0,                               // tp_itemsize
    (destructor)direct_dealloc,   // tp_dealloc
    0,                               // tp_vectorcall_offset / tp_print
    0,                               // tp_getattr
    0,                               // tp_setattr
    0,                               // tp_reserved / tp_compare
    (reprfunc)direct_str,         // tp_repr
    &direct_as_number,              // tp_as_number
    0,                               // tp_as_sequence
    0,                               // tp_as_mapping
    0,                               // tp_hash
    0,                               // tp_call
    (reprfunc)direct_str,         // tp_str
    0,                               // tp_getattro
    0,                               // tp_setattro
    0,                               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,              // tp_flags
    "Direct Product of Arbitrary Groups",              // tp_doc
    0,                               // tp_traverse
    0,                               // tp_clear
    PyDirect_richcompare,                               // tp_richcompare
    0,                               // tp_weaklistoffset
    0,                               // tp_iter
    0,                               // tp_iternext
    direct_methods,               // tp_methods
    PyDirect_members,                               // tp_members
    0,                // tp_getset
    0,                               // tp_base
    0,                               // tp_dict
    0,                               // tp_descr_get
    0,                               // tp_descr_set
    0,                               // tp_dictoffset
    (initproc)direct_init,        // tp_init
    0,                               // tp_alloc
    direct_new                    // tp_new
};

#endif