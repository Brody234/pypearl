#ifndef DIHEDRALPOOLING
#define DIHEDRALPOOLING
#include "dihedralpooling.hpp"

static void dihedral_pool_dealloc(dihedral_pool *self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * dihedral_pool_str(dihedral_pool *self){
    PyObject* out = PyUnicode_FromFormat(
        "Dihedral Pool"
    );
    return out;
}

static PyObject * dihedral_pool_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    (void)args;
    (void)kwds;
    dihedral_pool* self = (dihedral_pool*)type->tp_alloc(type, 0);
    return (PyObject*)self;
}

static int dihedral_pool_init(dihedral_pool *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = { (char*)"in_size", (char*)"out_size", (char*)"n", NULL };
    
    PyObject* in_dims;
    PyObject* out_dims;
    int64_t n;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOl", kwlist, &in_dims, &out_dims, &n))
        return -1;

    if(!PyTuple_Check(in_dims) || !PyTuple_Check(out_dims)){
        PyErr_SetString(PyExc_TypeError, "in_size and out_size must be tuples.");
        return -1;
    }
    Py_ssize_t nd_in = PyTuple_GET_SIZE(in_dims);
    Py_ssize_t nd_out = PyTuple_GET_SIZE(out_dims);
    if(nd_in != 2 || nd_out != 2){

        PyErr_SetString(PyExc_TypeError, "Pooling layers must take 2D input and output 2D output.");
        return -1;
    }

    PyObject* dim1;
    PyObject* dim2;

    dim1 = PyTuple_GET_ITEM(in_dims, 0);
    dim2 = PyTuple_GET_ITEM(in_dims, 1);

    size_t* ds_in = (size_t*)(malloc(2*sizeof(size_t)));;

    if(!PyLong_Check(dim1) || !PyLong_Check(dim2)){
        PyErr_SetString(PyExc_TypeError, "Both in dimensions must be longs");
        Py_DECREF(dim1);
        Py_DECREF(dim2);

        return -1;
    }

    if (PyLong_AsLongLong(dim1) <= 0 && PyLong_AsLongLong(dim2) <= 0&& !PyErr_Occurred()) {
        Py_DECREF(dim1);
        Py_DECREF(dim2);
        PyErr_SetString(PyExc_ValueError, "in_size must be positive integers");
        return -1;
    }
    if (PyErr_Occurred()) {
        Py_DECREF(dim1);
        Py_DECREF(dim2);
        return -1;
    }


    unsigned long long d1 = PyLong_AsUnsignedLongLong(dim1);
    unsigned long long d2 = PyLong_AsUnsignedLongLong(dim2);

    ds_in[0] = (size_t) d1;
    ds_in[1] = (size_t) d2;

    Py_DECREF(dim1);
    Py_DECREF(dim2);

    dim1 = PyTuple_GET_ITEM(out_dims, 0);
    dim2 = PyTuple_GET_ITEM(out_dims, 1);

    size_t* ds_out = (size_t*)(malloc(2*sizeof(size_t)));

    if(!PyLong_Check(dim1) || !PyLong_Check(dim2)){
        PyErr_SetString(PyExc_TypeError, "Both out dimensions must be longs");
        Py_DECREF(dim1);
        Py_DECREF(dim2);

        return -1;
    }

    if (PyLong_AsLongLong(dim1) <= 0 && PyLong_AsLongLong(dim2) <= 0&& !PyErr_Occurred()) {
        Py_DECREF(dim1);
        Py_DECREF(dim2);
        PyErr_SetString(PyExc_ValueError, "dims out must be positive integers");
        return -1;
    }
    if (PyErr_Occurred()) {
        Py_DECREF(dim1);
        Py_DECREF(dim2);
        return -1;
    }


    d1 = PyLong_AsUnsignedLongLong(dim1);
    d2 = PyLong_AsUnsignedLongLong(dim2);

    ds_out[0] = (size_t) d1;
    ds_out[1] = (size_t) d2;

    Py_DECREF(dim1);
    Py_DECREF(dim2);

    if(ds_in[0] % ds_out[0] != 0){
        PyErr_SetString(PyExc_ValueError, "In dimension 0 must be an integer multiple of out dimension 0");
        return -1;
    }

    if(ds_in[1] % ds_out[1] != 0){
        PyErr_SetString(PyExc_ValueError, "In dimension 1 must be an integer multiple of out dimension 1. Note 1 is the second dimension, 0 is first.");
        return -1;
    }

    self->in_size = ds_in;

    self->out_size = ds_out;

    self->step_0 = ds_in[0] / ds_out[0];

    self->step_1 = ds_in[0] / ds_out[0];

    ndarray* r_pool = arrayCInit(2, 0x3, ds_in);

    long window_size = self->step_0*self->step_1;

    self->windows = r_pool;

    long weight = n/window_size;

    for(size_t i = 0; i < r_pool->dims[0]; i++){
        for(size_t j = 0; j < r_pool->dims[1]; j++){
            fastSet2D8(r_pool, i, j, &weight);
        }
    }

    self->n = n;

    return 0;
}

dihedral_matrix* dihedral_pool_forward(dihedral_pool* self, dihedral_matrix* input){

    if(input->r->nd == 3){

    }
    else if (input->r->nd == 2){
        uint64_t val;
        uint64_t weight;
        ndarray* r_temp = arrayCInitCopy(input->r);
        // Scale everything in all windows to sum to less than 1 full rotation
        for(size_t i = 0; i < self->in_size[0]; i++){
            for(size_t j = 0; j < self->in_size[1]; j++){
                fastGet2D8(input->r, i, j, &val);
                fastGet2D8(self->windows, i, j, &weight);

                val *= weight;
                // this could also be done at the end for slightly different results, significant on small windows, would likely need testing
                val /= self->n; // can be changed for a bit shift

                //long window_size = self->step_0*self->step_1;

                //val /= window_size; // if window size is a power of 2, ie 4 it works and it can be forced to work.

                fastSet2D8(r_temp, i, j, &val);
            }
        }

        ndarray* out_r = arrayCInit(2, 0x3, self->out_size);
        ndarray* out_s = arrayCInit(2, 0x3, self->out_size);

        long r_count;
        long s_count;
        long temp;
        // Collapse everything into one
        for(size_t i = 0; i < self->out_size[0]; i++){
            for(size_t j = 0; j < self->out_size[1]; j++){
                // This could be swapped with iterative addition outside
                size_t x_offset = i*self->step_0;
                size_t y_offset = j*self->step_1;

                r_count = 0;
                s_count = 0;
                for(size_t k = 0; k < self->step_0; k++){
                    for(size_t l = 0; l < self->step_1; l++){
                        if(true){
                            fastGet2D8(r_temp, x_offset + k, y_offset + l, &temp);
                            r_count += temp;

                            fastGet2D8(input->s, x_offset + k, y_offset + l, &temp);
                            s_count += temp;
                        }
                    }
                }
                if(s_count > (long)(self->step_0*self->step_1)/2){
                    s_count = 1;
                }
                else{
                    s_count = 0;
                }

                fastSet2D8(out_s, i, j, &s_count);
                fastSet2D8(out_r, i, j, &r_count);
            }
        }

        dihedral_matrix* output = dihedralMatrixCInit(out_r, self->n, out_s);
        return output;
    }
    else{
        return nullptr;
    }
}

static dihedral_matrix* PyDihedralPool_forward(dihedral_pool *self, PyObject *arg){

    static char *kwlist[] = { (char*)"x", NULL };
    if (!PyObject_TypeCheck(arg, &dihedral_matrixType)) {
        PyErr_SetString(PyExc_TypeError, "Dihedral forward pass requires a DihedralMatrix");
        return NULL;
    }
    dihedral_matrix* input = (dihedral_matrix*)arg;
    try {
        dihedral_matrix* y = dihedral_pool_forward(self, input);
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

PyMethodDef dihedral_pool_methods[] = {
    {"forward", (PyCFunction)PyDihedralPool_forward, METH_O, "forward(x)->y"},
    {NULL, NULL, 0, NULL}
};



PyTypeObject dihedral_poolType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pypearl.DihedralPool",               // tp_name
    sizeof(dihedral_pool),         // tp_basicsize
    0,                               // tp_itemsize
    (destructor)dihedral_pool_dealloc,   // tp_dealloc
    0,                               // tp_vectorcall_offset / tp_print
    0,                               // tp_getattr
    0,                               // tp_setattr
    0,                               // tp_reserved / tp_compare
    (reprfunc)dihedral_pool_str,         // tp_repr
    0,              // tp_as_number
    0,                               // tp_as_sequence
    0,                               // tp_as_mapping
    0,                               // tp_hash
    0,                               // tp_call
    (reprfunc)dihedral_pool_str,         // tp_str
    0,                               // tp_getattro
    0,                               // tp_setattro
    0,                               // tp_as_buffer
    Py_TPFLAGS_DEFAULT,              // tp_flags
    "Dihedral Pool",              // tp_doc
    0,                               // tp_traverse
    0,                               // tp_clear
    0,                               // tp_richcompare
    0,                               // tp_weaklistoffset
    0,                               // tp_iter
    0,                               // tp_iternext
    dihedral_pool_methods,               // tp_methods
    0,                               // tp_members
    0,                // tp_getset
    0,                               // tp_base
    0,                               // tp_dict
    0,                               // tp_descr_get
    0,                               // tp_descr_set
    0,                               // tp_dictoffset
    (initproc)dihedral_pool_init,        // tp_init
    0,                               // tp_alloc
    dihedral_pool_new                    // tp_new
};

#endif
