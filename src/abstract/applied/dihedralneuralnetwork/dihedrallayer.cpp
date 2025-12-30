#ifndef DIHEDRALLAYER
#define DIHEDRALLAYER

#include "dihedrallayer.hpp"

static void dihedral_layer_dealloc(dihedral_layer *self)
{
    if(self->weights)
    Py_DECREF(self->weights);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * dihedral_layer_str(dihedral_layer *self){
    PyObject *r_str = PyObject_Str((PyObject*) self->weights);
    if (!r_str) { Py_DECREF(r_str); return NULL; }


    PyObject* out = PyUnicode_FromFormat(
        "Dihedral Layer\n"
        "Weights: \n%U",
        r_str
    );
    Py_DECREF(r_str);
    return out;
}

static PyObject * dihedral_layer_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    (void)args;
    (void)kwds;
    dihedral_layer *self = (dihedral_layer*)type->tp_alloc(type, 0);
    self->weights = nullptr;
    return (PyObject*)self;
}

static int dihedral_layer_init(dihedral_layer *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = { (char*)"dims", (char*)"degrees", (char*) "s", (char*)"n", NULL };
    
    PyObject* dims;
    long deg;
    long s;
    int64_t n;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "Olll", kwlist, &dims, &deg, &s, &n))
        return -1;



    if(!PyTuple_Check(dims)){
        PyErr_SetString(PyExc_TypeError, "dims must be a tuple.");
        return -1;
    }
    Py_ssize_t nd = PyTuple_GET_SIZE(dims);
    if(nd != 2){

        PyErr_SetString(PyExc_TypeError, "Weights must be a 2D tensor.");
        return -1;
    }

    PyObject* dim1;
    PyObject* dim2;

    dim1 = PyTuple_GET_ITEM(dims, 0);
    dim2 = PyTuple_GET_ITEM(dims, 1);

    size_t ds[2];

    if(!PyLong_Check(dim1) || !PyLong_Check(dim2)){
            PyErr_SetString(PyExc_TypeError, "Both dimensions must be longs");
            Py_DECREF(dim1);
            Py_DECREF(dim2);

            return -1;
        }

        if (PyLong_AsLongLong(dim1) <= 0 && PyLong_AsLongLong(dim2) <= 0&& !PyErr_Occurred()) {
            Py_DECREF(dim1);
            Py_DECREF(dim2);
            PyErr_SetString(PyExc_ValueError, "dims must be positive integers");
            return -1;
        }
        if (PyErr_Occurred()) {
            Py_DECREF(dim1);
            Py_DECREF(dim2);
            return -1;
        }


        unsigned long long d1 = PyLong_AsUnsignedLongLong(dim1);
        unsigned long long d2 = PyLong_AsUnsignedLongLong(dim2);

        ds[0] = (size_t) d1;
        ds[1] = (size_t) d2;


    ndarray* r_weights = arrayCInit(2, 0x3, ds);

    ndarray* s_weights = arrayCInit(2, 0x3, ds);

    if(deg >= n){
        PyErr_SetString(PyExc_ValueError, "Degrees cannot be greater than n.");
        return -1;
    }

    std::random_device rd;

    std::mt19937 engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_int_distribution<int64_t> dist(0, deg);

    int64_t half = deg/2;

    for(size_t i = 0; i < ds[0]; i++){
        for(size_t j = 0; j < ds[1]; j++){
            int64_t w = dist(engine);
            w-=half;

            if(w < 0){

                w=n+w;
            }
            if(w >= n || w < 0){
                // One last should be unnecessary layer of protection since all dihedral functions depend on this assumption.
                PyErr_SetString(PyExc_ValueError, "Weight initialization failed.");
                return -1;
            }
            fastSet2D8(r_weights, i, j, &w);
        }
    }

    std::uniform_int_distribution<int64_t> dist2(0, 1);

    for(size_t i = 0; i < ds[0]; i++){
        for(size_t j = 0; j < ds[1]; j++){
            int64_t w = dist2(engine);

            if(w != 0 && w != 1){
                // One last should be unnecessary layer of protection since all dihedral functions depend on this assumption.
                PyErr_SetString(PyExc_ValueError, "Initialization of S weights failed (this should be literally impossible).");
                return -1;
            }
            fastSet2D8(s_weights, i, j, &w);
        }
    }

    dihedral_matrix* weights = dihedralMatrixCInit(r_weights, n, s_weights);

    self->weights = weights;

    return 0;
}

dihedral_matrix* dihedral_layer_forward(dihedral_layer* self, dihedral_matrix* input){
    dihedral_matrix* y = (dihedral_matrix*)PyDihedral_Matrix_add_new((PyObject*)self->weights, (PyObject*)input);
    if(PyErr_Occurred()){
        return nullptr;
    }
    else{
        return y;
    }
}

static dihedral_matrix* PyDihedralLayer_forward(dihedral_layer *self, PyObject *arg){

    static char *kwlist[] = { (char*)"x", NULL };
    if (!PyObject_TypeCheck(arg, &dihedral_matrixType)) {
        PyErr_SetString(PyExc_TypeError, "Dihedral forward pass requires a DihedralMatrix");
        return NULL;
    }
    dihedral_matrix* input = (dihedral_matrix*)arg;
    try {
        dihedral_matrix* y = dihedral_layer_forward(self, input);
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

PyMethodDef dihedral_layer_methods[] = {
    {"forward", (PyCFunction)PyDihedralLayer_forward, METH_O, "forward(x)->y"},
    {NULL, NULL, 0, NULL}
};



PyTypeObject dihedral_layerType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pypearl.DihedralLayer",               // tp_name
    sizeof(dihedral_layer),         // tp_basicsize
    0,                               // tp_itemsize
    (destructor)dihedral_layer_dealloc,   // tp_dealloc
    0,                               // tp_vectorcall_offset / tp_print
    0,                               // tp_getattr
    0,                               // tp_setattr
    0,                               // tp_reserved / tp_compare
    (reprfunc)dihedral_layer_str,         // tp_repr
    0,              // tp_as_number
    0,                               // tp_as_sequence
    0,                               // tp_as_mapping
    0,                               // tp_hash
    0,                               // tp_call
    (reprfunc)dihedral_layer_str,         // tp_str
    0,                               // tp_getattro
    0,                               // tp_setattro
    0,                               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,              // tp_flags
    "Dihedral Layer",              // tp_doc
    0,                               // tp_traverse
    0,                               // tp_clear
    0,                               // tp_richcompare
    0,                               // tp_weaklistoffset
    0,                               // tp_iter
    0,                               // tp_iternext
    dihedral_layer_methods,               // tp_methods
    0,                               // tp_members
    0,                // tp_getset
    0,                               // tp_base
    0,                               // tp_dict
    0,                               // tp_descr_get
    0,                               // tp_descr_set
    0,                               // tp_dictoffset
    (initproc)dihedral_layer_init,        // tp_init
    0,                               // tp_alloc
    dihedral_layer_new                    // tp_new
};

#endif