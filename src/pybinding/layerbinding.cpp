#ifndef LAYERBINDING
#define LAYERBINDING
#include "layerbinding.hpp"

static void
PyLayerD_dealloc(PyLayerDObject *self)
{
    delete self->cpp_obj;

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
PyLayerD_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyLayerDObject *self = (PyLayerDObject*)type->tp_alloc(type, 0);
    if (self) {
        self->cpp_obj = nullptr;
    }
    return (PyObject*)self;
}

static int
PyLayerD_init(PyLayerDObject *self, PyObject *args, PyObject *kwds)
{
    Py_ssize_t prev, cur;
    static char *kwlist[] = { (char*)"prev_layer_size", (char*)"layer_size", nullptr };
    // require one integer argument: size
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "nn", kwlist, &prev, &cur))
        return -1;

    try {
        // allocate your C++ object
        self->cpp_obj = new LayerD((size_t)prev, (size_t)cur, false);
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return -1;
    }
    return 0;
}

static PyObject* PyLayerD_forward(PyLayerDObject *self, PyObject *arg){
    PyLayerDObject *layer_obj = (PyLayerDObject*) self;

    std::cout <<"43" << std::endl;
    static char *kwlist[] = { (char*)"x", NULL };
    if (!PyObject_TypeCheck(arg, &PyArrayD2Type)) {
        PyErr_SetString(PyExc_TypeError, "forward() expects an ArrayD2");
        return NULL;
    }
    PyArrayD2Object *input_obj = (PyArrayD2Object*)arg;

    std::cout <<"55" << std::endl;
    ArrayD2 out_cpp;
    try {
        out_cpp = layer_obj->cpp_obj->forward(*input_obj->cpp_obj);
    } catch (const std::exception &e) {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return NULL;
    }
    std::cout <<"63" << std::endl;

    // Allocate a new Python ArrayD2 object
    PyObject *out_py = PyArrayD2Type.tp_new(&PyArrayD2Type, NULL, NULL);
    if (!out_py) return NULL;
    std::cout <<"68" << std::endl;

    // Steal the C++ result into its cpp_obj
    ((PyArrayD2Object*)out_py)->cpp_obj = new ArrayD2(std::move(out_cpp));

    return out_py;

}

PyMethodDef PyLayerD_methods[] = {
    /*{"get", (PyCFunction)PyArrayD1_get, METH_VARARGS,
     "get(index) -> float\n\n"
     "Return the element at position `index` (0-based)."},
     {"set", (PyCFunction)PyArrayD1_set, METH_VARARGS, "set(index)->float\n\n"
     "Change the element at position index"},*/
    {"forward", (PyCFunction)PyLayerD_forward, METH_O, "forward(x)->y"},
    {NULL, NULL, 0, NULL}
};

PyGetSetDef PyLayerD_getset[] = {
    {NULL, NULL, NULL, NULL, NULL}
};

PyTypeObject PyLayerDType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "pypearl.Layer",
    .tp_basicsize = sizeof(PyLayerDObject),
    .tp_dealloc   = (destructor)PyLayerD_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = "Neural Network Layer",
    .tp_methods   = PyLayerD_methods,
    .tp_getset    = PyLayerD_getset,
    .tp_new       = PyLayerD_new,
    .tp_init      = (initproc)PyLayerD_init,
};


#endif