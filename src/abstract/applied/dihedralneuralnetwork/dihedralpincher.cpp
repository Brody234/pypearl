#ifndef DIHEDRALPINCHER
#define DIHEDRALPINCHER
#include "dihedralpincher.hpp"

static void dihedral_pincher_dealloc(dihedral_pincher *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * dihedral_pincher_str(dihedral_pincher *self){
    PyObject* out = PyUnicode_FromFormat(
        "Dihedral Pincher"
    );
    return out;
}

static PyObject * dihedral_pincher_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    (void)args;
    (void)kwds;
    dihedral_pincher* self = (dihedral_pincher*)type->tp_alloc(type, 0);
    return (PyObject*)self;
}

static int dihedral_pincher_init(dihedral_pincher *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = { (char*)"in_n", (char*)"out_n", NULL };
    
    int64_t n_in;
    int64_t n_out;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ll", kwlist, &n_in, &n_out))
        return -1;

    if(n_in%n_out != 0){
        PyErr_SetString(PyExc_ArithmeticError, "n_in is not a scalar integer multiple of n_out");
        return -1;
    }

    if(n_in == 0 || n_out == 0){
        PyErr_SetString(PyExc_ArithmeticError, "D_0 is not a valid dihedral group");
        return -1;
    }

    if(n_in < 0 || n_out < 0){
        PyErr_SetString(PyExc_ArithmeticError, "Cannot have negative n");
        return -1;
    }

    if(n_in == 1 || n_out == 1){
        PyErr_SetString(PyExc_ArithmeticError, "Reducing to a single vertex removes all information, not elligible");
        return -1;
    }
    if(n_in < n_out){
        PyErr_SetString(PyExc_ArithmeticError, "Cannot have output larger than input, were reducing dimensions.");
        return -1;
    }

    self->n_start = n_in;
    self->n_end = n_out;

    self->factor = n_in/n_out;

    return 0;
}

dihedral_matrix* dihedral_pincher_forward(dihedral_pincher* self, dihedral_matrix* input){

    if(input->r->nd == 3){

    }
    else if (input->r->nd == 2){
        ndarray* r_copy = arrayCInitCopy(input->r);
        ndarray* s_copy = arrayCInitCopy(input->s);
        dihedral_matrix* output = dihedralMatrixCInit(r_copy, self->n_end, s_copy);
        int64_t temp;
        for(size_t i = 0; i < input->r->dims[0]; i++){
            for(size_t j = 0; j < input->r->dims[1]; j++){
                fastGet2D8(r_copy, i, j, &temp);
                temp /= self->factor;

                fastSet2D8(r_copy, i, j, &temp);
            }
        }
        return output;
    }
    else{
        return nullptr;
    }
}

static dihedral_matrix* PyDihedralPincher_forward(dihedral_pincher *self, PyObject *arg){

    static char *kwlist[] = { (char*)"x", NULL };
    if (!PyObject_TypeCheck(arg, &dihedral_matrixType)) {
        PyErr_SetString(PyExc_TypeError, "Dihedral forward pass requires a DihedralMatrix");
        return NULL;
    }
    dihedral_matrix* input = (dihedral_matrix*)arg;
    try {
        dihedral_matrix* out = dihedral_pincher_forward(self, input);
        if(out){
            return out;
        }
        else{
            return NULL;
        }
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

PyMethodDef dihedral_pincher_methods[] = {
    {"forward", (PyCFunction)PyDihedralPincher_forward, METH_O, "forward(x)->y"},
    {NULL, NULL, 0, NULL}
};



PyTypeObject dihedral_pincherType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pypearl.DihedralPincher",               // tp_name
    sizeof(dihedral_pincher),         // tp_basicsize
    0,                               // tp_itemsize
    (destructor)dihedral_pincher_dealloc,   // tp_dealloc
    0,                               // tp_vectorcall_offset / tp_print
    0,                               // tp_getattr
    0,                               // tp_setattr
    0,                               // tp_reserved / tp_compare
    (reprfunc)dihedral_pincher_str,         // tp_repr
    0,              // tp_as_number
    0,                               // tp_as_sequence
    0,                               // tp_as_mapping
    0,                               // tp_hash
    0,                               // tp_call
    (reprfunc)dihedral_pincher_str,         // tp_str
    0,                               // tp_getattro
    0,                               // tp_setattro
    0,                               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,              // tp_flags
    "Dihedral Pincher",              // tp_doc
    0,                               // tp_traverse
    0,                               // tp_clear
    0,                               // tp_richcompare
    0,                               // tp_weaklistoffset
    0,                               // tp_iter
    0,                               // tp_iternext
    dihedral_pincher_methods,               // tp_methods
    0,                               // tp_members
    0,                // tp_getset
    0,                               // tp_base
    0,                               // tp_dict
    0,                               // tp_descr_get
    0,                               // tp_descr_set
    0,                               // tp_dictoffset
    (initproc)dihedral_pincher_init,        // tp_init
    0,                               // tp_alloc
    dihedral_pincher_new                    // tp_new
};

#endif
