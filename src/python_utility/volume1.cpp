#ifndef VOLUME1CPP
#define VOLUME1CPP

#include "volume1.hpp"

// send a stack allocated 50 length vals and a stack allocated size variable
int tuple_to_size_t_arr(PyObject* tuple, size_t* size, size_t* vals){
    if(!tuple || tuple == Py_None){
        PyErr_SetString(PyExc_TypeError, "dims must be a tuple of non-negative integers");
        size[0] = 0;
        return -1;
    }

    if (!PyTuple_Check(tuple)) {
        PyErr_SetString(PyExc_TypeError, "dims must be a tuple of non-negative integers");
        size[0] = 0;
        return -1;
    }
    
    size[0] = PyTuple_GET_SIZE(tuple);
    if(size[0]>50){
        // This is totally not because I want the tuple parsing to be a stack allocation with no requirement for malloc (and therefore people not needing to remember to free the output).
        // Like I personally would obviously always remember to free the value returned by this function but it's much easier to just send it a size 50 array on stack.
        PyErr_SetString(PyExc_TypeError, "Max dims size is 50. You do not need more than 50 dimensions. If you make any nontrivial use of these 50, you will crash your computer.");
        return -1;
    }  

    for(size_t i = 0; i < size[0]; i++){
        PyObject* obj = PyTuple_GET_ITEM(tuple, i);
        Py_INCREF(obj);

        if(!PyLong_Check(obj)){
            PyErr_SetString(PyExc_TypeError, "dims must be a tuple of non-negative integers");
            return -1;
        }

        if (PyLong_AsLongLong(obj) < 0 && !PyErr_Occurred()) {
            Py_DECREF(obj);
            PyErr_SetString(PyExc_ValueError, "dims must contain non-negative integers");
            return -1;
        }
        if (PyErr_Occurred()) {
            Py_DECREF(obj);
            return -1;
        }


        unsigned long long ull = PyLong_AsUnsignedLongLong(obj);
        Py_DECREF(obj);

        vals[i] = ull;

    }

    return 0;

}

#endif