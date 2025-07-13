#include <Python.h>
#include "../matrix/matrix.hpp"
#include "matrixbinding.hpp"
#include "layerbinding.hpp"

PyObject *add(PyObject *self, PyObject *args){
    int x;
    int y;

    PyArg_ParseTuple(args, "ii", &x, &y);

    return PyLong_FromLong(((long)(x+y)));
};

static PyMethodDef methods[] {
    {"add", add, METH_VARARGS, "Adds two numbers together"},
    {NULL, NULL, 0, NULL}
}; 
  
static struct PyModuleDef pypearl = {
    PyModuleDef_HEAD_INIT,
    "pypearl",
    "Documentation: To be implemented, as a safeguard if this is somehow still in place August 2025 or later, please contact me.",
    -1,
    methods
};

PyMODINIT_FUNC
PyInit__pypearl(void)
{
    PyObject *m = PyModule_Create(&pypearl);
    if (!m) return NULL;
    PyArrayD1Type.tp_as_sequence = &PyArrayD1_as_sequence;

    // --- register ArrayD1 ---
    if (PyType_Ready(&PyArrayD1Type) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(&PyArrayD1Type);
    PyModule_AddObject(m, "ArrayD1", (PyObject*)&PyArrayD1Type);
 
    PyArrayD2Type.tp_as_mapping = &PyArrayD2_as_mapping;
    // --- register ArrayD2 ---
    if (PyType_Ready(&PyArrayD2Type) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(&PyArrayD2Type);
    PyModule_AddObject(m, "ArrayD2", (PyObject*)&PyArrayD2Type);

    // --- register ArrayD2 ---
    if (PyType_Ready(&PyLayerDType) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(&PyLayerDType);
    PyModule_AddObject(m, "Layer", (PyObject*)&PyLayerDType);

    return m;
}  
 
  