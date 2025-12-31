#ifndef DIHEDRALACTIVATION
#define DIHEDRALACTIVATION

#include "dihedralactivation.hpp"

static void dihedral_activation_dealloc(dihedral_activation *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * dihedral_activation_str(dihedral_activation *self){

    PyObject* out = PyUnicode_FromFormat(
        "Dihedral Activation"
    );

    return out;
}

static PyObject * dihedral_activation_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    (void)args;
    (void)kwds;
    dihedral_activation *self = (dihedral_activation*)type->tp_alloc(type, 0);
    return (PyObject*)self;
}

static int dihedral_activation_init(dihedral_activation *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = { (char*)"type", NULL };
    
    const char *type = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "z", kwlist, &type))
        return -1;


    if(!type){
        PyErr_SetString(PyExc_TypeError, "Type not found.");
        return -1;
    }

    if(strcmp(type, "S") == 0){
        self->type = 0x1;
    }
    else if(strcmp(type, "R") == 0){
        self->type = 0x0;
    }
    else if(strcmp(type, "B") == 0){
        self->type = 0x2; // unnecessary hex numbers is the programming equivalent of that guy who answers yes or no questions with exuberantly.
    }
    else{
        PyErr_SetString(PyExc_TypeError, "Type not valid, choose strings S, R, B. See documentation for reference.");
        return -1;
    }

    return 0;
}



dihedral_matrix* dihedral_activation_forward(dihedral_activation* self, dihedral_matrix* input){
    int64_t r;
    int64_t s;
    int64_t zero = 0x0;

    ndarray* r_arr = arrayCInitCopy(input->r);
    ndarray* s_arr = arrayCInitCopy(input->s);

    dihedral_matrix* output = dihedralMatrixCInit(r_arr, input->n, s_arr);

    if(output->r->nd == 2 && output->s->nd == 2){
        for(size_t i = 0; i < output->r->dims[0]; i++){
            for(size_t j = 0; j < output->r->dims[1]; j++){
                if(self->type == 0x0){
                    fastGet2D8(output->r, i, j, &r);
                    if(r > (output->n/2)){
                        fastSet2D8(output->r, i, j, &zero);
                        fastSet2D8(output->s, i, j, &zero);
                    }
                }
                else if(self->type == 0x1){
                    fastGet2D8(output->s, i, j, &s);
                    if(s != 0){
                        fastSet2D8(output->r, i, j, &zero);
                        fastSet2D8(output->s, i, j, &zero);
                    }
                }
                else if(self->type == 0x2){
                    fastGet2D8(output->s, i, j, &s);
                    fastGet2D8(output->r, i, j, &r);
                    if((s == 0 && r > output->n/2) || (s != 0 && r < output->n/2)){
                        fastSet2D8(output->r, i, j, &zero);
                        fastSet2D8(output->s, i, j, &zero);
                    }
                }
            }
        }
    }
    else if(output->r->nd == 3 && output->s->nd == 3){
        for(size_t i = 0; i < output->r->dims[0]; i++){
            for(size_t j = 0; j < output->r->dims[1]; j++){
                for(size_t k = 0; k < output->r->dims[2]; k++){
                    if(self->type == 0x0){
                        fastGet3D8(output->r, i, j, k, &r);
                        if(r > (output->n/2)){
                            fastSet3D8(output->r, i, j, k, &zero);
                            fastSet3D8(output->s, i, j, k, &zero);
                        }
                    }
                    else if(self->type == 0x1){
                        fastGet3D8(output->s, i, j, k, &s);
                        if(s != 0){
                            fastSet3D8(output->r, i, j, k, &zero);
                            fastSet3D8(output->s, i, j, k, &zero);
                        }
                    }
                    else if(self->type == 0x2){
                        fastGet3D8(output->s, i, j, k, &s);
                        fastGet3D8(output->r, i, j, k, &r);
                        if((s == 0 && r > output->n/2) || (s != 0 && r < output->n/2)){
                            fastSet3D8(output->r, i, j, k, &zero);
                            fastSet3D8(output->s, i, j, k, &zero);
                        }
                    }
                }
            }
        }
    }
    else{
        PyErr_SetString(PyExc_TypeError, "Can only activate 2 or 3 dimensional tensors.");
        return nullptr;
    }
    return output;
}

static dihedral_matrix* PyDihedralActivation_forward(dihedral_activation *self, PyObject *arg){

    static char *kwlist[] = { (char*)"x", NULL };
    if (!PyObject_TypeCheck(arg, &dihedral_matrixType)) {
        PyErr_SetString(PyExc_TypeError, "Dihedral forward pass requires a DihedralMatrix");
        return NULL;
    }
    dihedral_matrix* input = (dihedral_matrix*)arg;
    try {
        dihedral_matrix* y = dihedral_activation_forward(self, input);
        if(y){
            return y;
        }
        else{
            return NULL;
        }
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
}

PyMethodDef dihedral_activation_methods[] = {
    {"forward", (PyCFunction)PyDihedralActivation_forward, METH_O, "forward(x)->y"},
    {NULL, NULL, 0, NULL}
};



PyTypeObject dihedral_activationType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pypearl.DihedralActivation",               // tp_name
    sizeof(dihedral_activation),         // tp_basicsize
    0,                               // tp_itemsize
    (destructor)dihedral_activation_dealloc,   // tp_dealloc
    0,                               // tp_vectorcall_offset / tp_print
    0,                               // tp_getattr
    0,                               // tp_setattr
    0,                               // tp_reserved / tp_compare
    (reprfunc)dihedral_activation_str,         // tp_repr
    0,              // tp_as_number
    0,                               // tp_as_sequence
    0,                               // tp_as_mapping
    0,                               // tp_hash
    0,                               // tp_call
    (reprfunc)dihedral_activation_str,         // tp_str
    0,                               // tp_getattro
    0,                               // tp_setattro
    0,                               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,              // tp_flags
    "Dihedral Activation",              // tp_doc
    0,                               // tp_traverse
    0,                               // tp_clear
    0,                               // tp_richcompare
    0,                               // tp_weaklistoffset
    0,                               // tp_iter
    0,                               // tp_iternext
    dihedral_activation_methods,               // tp_methods
    0,                               // tp_members
    0,                // tp_getset
    0,                               // tp_base
    0,                               // tp_dict
    0,                               // tp_descr_get
    0,                               // tp_descr_set
    0,                               // tp_dictoffset
    (initproc)dihedral_activation_init,        // tp_init
    0,                               // tp_alloc
    dihedral_activation_new                    // tp_new
};

#endif