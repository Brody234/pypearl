#ifndef MATRIXBINDING
#define MATRIXBINDING

#include "matrixbinding.hpp"

static void
PyArrayD1_dealloc(PyArrayD1Object *self)
{
    // 1) delete the C++ array
    delete self->cpp_obj;
    // 2) free the Python wrapper
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
PyArrayD1_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyArrayD1Object *self = (PyArrayD1Object*)type->tp_alloc(type, 0);
    if (self) {
        self->cpp_obj = nullptr;
    }
    return (PyObject*)self;
}

static int
PyArrayD1_init(PyArrayD1Object *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t size;
    static char *kwlist[] = { (char*)"size", nullptr };
    // require one integer argument: size
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "n", kwlist, &size))
        return -1;

    try {
        // allocate your C++ object
        self->cpp_obj = new ArrayD1((size_t)size);
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return -1;
    }
    return 0;
}

static PyObject *
PyArrayD1_set(PyArrayD1Object *self, PyObject *args){

    Py_ssize_t idx;
    double newval;
    if(!PyArg_ParseTuple(args, "nd", &idx, &newval))
        return NULL;
    size_t size = self->cpp_obj->len;
    if (!self->cpp_obj) {
        PyErr_SetString(PyExc_AttributeError,
                        "ArrayD1 object not initialized");
        return NULL;
    }

    if(idx < 0 || (size_t)idx >= size){
        PyErr_SetString(PyExc_IndexError, "index out of bounds");
        return NULL;
    }
    (*(self->cpp_obj))[(size_t)idx] = newval;
    Py_RETURN_NONE;
}
static Py_ssize_t
PyArrayD1_length(PyArrayD1Object *self, PyObject *args)
{
    // assuming your C++ object has a .len member
    return (Py_ssize_t) self->cpp_obj->len;
}

static PyObject *
PyArrayD1_get(PyArrayD1Object *self, PyObject *args)
{
    Py_ssize_t idx;
    // parse one integer argument
    if (!PyArg_ParseTuple(args, "n", &idx))
        return NULL;

    // bounds‐check
    size_t size = self->cpp_obj->len;  
    if (idx < 0 || (size_t)idx >= size) {
        PyErr_SetString(PyExc_IndexError, "index out of range");
        return NULL;
    }

    // fetch from your C++ array
    double value = (*(self->cpp_obj))[(size_t)idx];
    return PyFloat_FromDouble(value);
}

static PyObject *
PyArrayD1_item(PyObject *self, Py_ssize_t idx)
{
    // Build a one-element tuple (idx,) so we can call your existing get
    PyObject *args = Py_BuildValue("(n)", idx);
    if (!args) return NULL;

    PyObject *result = PyArrayD1_get((PyArrayD1Object*)self, args);
    Py_DECREF(args);
    return result;
}

// adapter for __setitem__
static int
PyArrayD1_ass_item(PyObject *self, Py_ssize_t idx, PyObject *value)
{
    // Build a two-element tuple (idx, value) for your existing set
    PyObject *args = Py_BuildValue("(nO)", idx, value);
    if (!args) return -1;

    PyObject *res = PyArrayD1_set((PyArrayD1Object*)self, args);
    Py_DECREF(args);
    if (!res) return -1;
    Py_DECREF(res);
    return 0;
}

static void
PyArrayD2_dealloc(PyArrayD2Object *self)
{
    delete self->cpp_obj;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
PyArrayD2_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyArrayD2Object *self = (PyArrayD2Object*)type->tp_alloc(type, 0);
    if (self) {
        self->cpp_obj = nullptr;
    }
    return (PyObject*)self;
}

static int
PyArrayD2_init(PyArrayD2Object *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t rows, cols;
    static char *kwlist[] = { (char*)"rows", (char*)"cols", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "nn", kwlist, &rows, &cols))
        return -1;

    // build a two-element size array
    std::size_t dims[2] = { static_cast<std::size_t>(rows),
                            static_cast<std::size_t>(cols) };
    try {
        // call your Array<const size_t*> constructor
        self->cpp_obj = new ArrayD2(dims);
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return -1;
    }
    return 0;
}

static bool
get_two_indices(PyObject *key, Py_ssize_t &i, Py_ssize_t &j) {
    if (!PyTuple_Check(key) || PyTuple_Size(key) != 2) {
        PyErr_SetString(PyExc_TypeError, "Index must be a tuple of two integers");
        return false;
    }
    PyObject *i_obj = PyTuple_GetItem(key, 0);
    PyObject *j_obj = PyTuple_GetItem(key, 1);
    i = PyNumber_AsSsize_t(i_obj, PyExc_IndexError);
    j = PyNumber_AsSsize_t(j_obj, PyExc_IndexError);
    if ((i == -1 && PyErr_Occurred()) || (j == -1 && PyErr_Occurred()))
        return false;
    return true;
}

// __getitem__ for 2D
static PyObject *
PyArrayD2_subscript(PyObject *self, PyObject *key)
{
    auto *obj = (PyArrayD2Object*)self;
    Py_ssize_t i, j;
    if (!get_two_indices(key, i, j))
        return NULL;

    // bounds check
    if (i < 0 || i >= (Py_ssize_t)obj->cpp_obj->shape[0] ||
        j < 0 || j >= (Py_ssize_t)obj->cpp_obj->shape[1]) {
        PyErr_SetString(PyExc_IndexError, "ArrayD2 index out of range");
        return NULL;
    }

    // assuming operator[](i)[j] works in your C++
    double v = (*obj->cpp_obj)[i][j];
    return PyFloat_FromDouble(v);
}


static Py_ssize_t
PyArrayD2_length(PyArrayD2Object *self, PyObject *args, PyObject *value)
{
    auto *obj = (PyArrayD2Object*)self;
    Py_ssize_t i, j;

    // assuming your C++ object has a .len member
    return (Py_ssize_t) self->cpp_obj->shape[0];
        if (!get_two_indices(args, i, j))
        return -1;

    if (i < 0 || i >= (Py_ssize_t)obj->cpp_obj->shape[0] ||
        j < 0 || j >= (Py_ssize_t)obj->cpp_obj->shape[1]) {
        PyErr_SetString(PyExc_IndexError, "ArrayD2 index out of range");
        return -1;
    }

    double dv = PyFloat_AsDouble(value);
    if (PyErr_Occurred()) return -1;
    (*obj->cpp_obj)[i][j] = dv;
    return 0;

}


#endif