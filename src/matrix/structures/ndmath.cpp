#ifndef NDMATH_C
#define NDMATH_C

#ifdef __cplusplus
extern "C" {
#endif

#include "ndarray.hpp"

/*
 * Layout of the file:
 * - Helper functions for scalar math
 * - Helper functions for matrix to matrix math
 * - Middle functions for matrix to matrix math (don't do the work but call loops)
 * - Python Functions to be called from Python
 * 
 * None of the functions here are not supposed to be extremely efficient,
 *      as it just doesn't really matter. All this stuff is O(n) where n is 
 *      number of dims.
 */


/*
 * SECTION 1: Matrix/Scalar Helper functions
 * write to original matrix
 */

// The following functions disgust me
void ndadd(void* data, uint8_t dtype ,double val){
    if(dtype == 0x0){
        float temp = (float)val;
        float* tdata = (float*) data;
        tdata[0] += temp;
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        tdata[0] += val;
    }

    if(dtype == 0x2){
        int32_t temp = (int32_t)val;
        int32_t* tdata = (int32_t*) data;
        tdata[0] += temp;
    }

    if(dtype == 0x3){
        int64_t temp = (int64_t)val;
        int64_t* tdata = (int64_t*) data;
        tdata[0] += temp;
    }

    return;
}

void ndsub(void* data, uint8_t dtype, double val){
    if(dtype == 0x0){
        float temp = (float)val;
        float* tdata = (float*) data;
        tdata[0] -= temp;
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        tdata[0] -= val;
    }

    if(dtype == 0x2){
        int32_t temp = (int32_t)val;
        int32_t* tdata = (int32_t*) data;
        tdata[0] -= temp;
    }

    if(dtype == 0x3){
        int64_t temp = (int64_t)val;
        int64_t* tdata = (int64_t*) data;
        tdata[0] -= temp;
    }

    return;
}

void ndmult(void* data, uint8_t dtype, double val){
    if(dtype == 0x0){
        float temp = (float)val;
        float* tdata = (float*) data;
        tdata[0] *= temp;
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        tdata[0] *= val;
    }

    if(dtype == 0x2){
        int32_t temp = (int32_t)val;
        int32_t* tdata = (int32_t*) data;
        tdata[0] *= temp;
    }

    if(dtype == 0x3){
        int64_t temp = (int64_t)val;
        int64_t* tdata = (int64_t*) data;
        tdata[0] *= temp;
    }

    return;
}

void nddiv(void* data, uint8_t dtype, double val){
    if(dtype == 0x0){
        float temp = (float)val;
        float* tdata = (float*) data;
        tdata[0] /= temp;
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        tdata[0] /= val;
    }

    if(dtype == 0x2){
        int32_t temp = (int32_t)val;
        int32_t* tdata = (int32_t*) data;
        tdata[0] /= temp;
    }

    if(dtype == 0x3){
        int64_t temp = (int64_t)val;
        int64_t* tdata = (int64_t*) data;
        tdata[0] /= temp;
    }

    return;
}

/*
 * SECTION 2: Matrix/Matrix helper functions
 * write to original matrix
 */

void ndaddnd(void* data, void* other, uint8_t dtype){
    if(dtype == 0x0){
        float* odata = (float*)other;
        float* tdata = (float*) data;
        tdata[0] += odata[0];
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        double* odata = (double*)other;
        tdata[0] += odata[0];
    }

    if(dtype == 0x2){
        int32_t* odata = (int32_t*)other;
        int32_t* tdata = (int32_t*) data;
        tdata[0] += odata[0];
    }

    if(dtype == 0x3){
        int64_t* odata = (int64_t*)other;
        int64_t* tdata = (int64_t*) data;
        tdata[0] += odata[0];
    }

    return;
}

void ndsubnd(void* data, void* other, uint8_t dtype){
    if(dtype == 0x0){
        float* odata = (float*)other;
        float* tdata = (float*) data;
        tdata[0] -= odata[0];
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        double* odata = (double*)other;
        tdata[0] -= odata[0];
    }

    if(dtype == 0x2){
        int32_t* odata = (int32_t*)other;
        int32_t* tdata = (int32_t*) data;
        tdata[0] -= odata[0];
    }

    if(dtype == 0x3){
        int64_t* odata = (int64_t*)other;
        int64_t* tdata = (int64_t*) data;
        tdata[0] -= odata[0];
    }

    return;
}

void ndmultnd(void* data, void* other, uint8_t dtype){
    if(dtype == 0x0){
        float* odata = (float*)other;
        float* tdata = (float*) data;
        tdata[0] *= odata[0];
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        double* odata = (double*)other;
        tdata[0] *= odata[0];
    }

    if(dtype == 0x2){
        int32_t* odata = (int32_t*)other;
        int32_t* tdata = (int32_t*) data;
        tdata[0] *= odata[0];
    }

    if(dtype == 0x3){
        int64_t* odata = (int64_t*)other;
        int64_t* tdata = (int64_t*) data;
        tdata[0] *= odata[0];
    }

    return;
}

void nddivnd(void* data, void* other, uint8_t dtype){
    if(dtype == 0x0){
        float* odata = (float*)other;
        float* tdata = (float*) data;

        if(odata[0] == 0){
            PyErr_SetString(PyExc_ValueError, "Divide by 0 error");
            return;
        }

        tdata[0] /= odata[0];
    }

    if(dtype == 0x1){
        double* tdata = (double*) data;
        double* odata = (double*)other;

        if(odata[0] == 0){
            PyErr_SetString(PyExc_ValueError, "Divide by 0 error");
            return;
        }

        tdata[0] /= odata[0];
    }

    if(dtype == 0x2){
        int32_t* odata = (int32_t*)other;
        int32_t* tdata = (int32_t*) data;

        if(odata[0] == 0){
            PyErr_SetString(PyExc_ValueError, "Divide by 0 error");
            return;
        }

        tdata[0] /= odata[0];
    }

    if(dtype == 0x3){
        int64_t* odata = (int64_t*)other;
        int64_t* tdata = (int64_t*) data;

        if(odata[0] == 0){
            PyErr_SetString(PyExc_ValueError, "Divide by 0 error");
            return;
        }

        tdata[0] /= odata[0];
    }

    return;
}


/*
 * SECTION 3: Matrix/Matrix Middle Functions
 * write to original matrix
 */

void ndarray_add_array(ndarray* self, ndarray* other){
    if(self->nd != other->nd){
        PyErr_SetString(PyExc_ValueError, "operands must have the same number of dimensions");
        return;
    }

    if(self->nd == 0){
        ndaddnd(self->data, other->data, self->dtype);
        return;
    }

    for(size_t i = 0; i < self->nd; i++){
        if(self->dims[i] != other->dims[i]){
            PyErr_SetString(PyExc_ValueError, "Shape Error!!!");
            return;
        }
    }

    ndForeachND(self, other, ndaddnd);
    return;
}

void ndarray_sub_array(ndarray* self, ndarray* other){
    if(self->nd != other->nd){
        PyErr_SetString(PyExc_ValueError, "operands must have the same number of dimensions");
        return;
    }

    if(self->nd == 0){
        ndsubnd(self->data, other->data, self->dtype);
        return;
    }

    for(size_t i = 0; i < self->nd; i++){
        if(self->dims[i] != other->dims[i]){
            PyErr_SetString(PyExc_ValueError, "Shape Error!!!");
            return;
        }
    }

    ndForeachND(self, other, ndsubnd);
    return;
}

void ndarray_mult_array(ndarray* self, ndarray* other){
    if(self->nd != other->nd){
        PyErr_SetString(PyExc_ValueError, "operands must have the same number of dimensions");
        return;
    }

    if(self->nd == 0){
        ndmultnd(self->data, other->data, self->dtype);
        return;
    }

    for(size_t i = 0; i < self->nd; i++){
        if(self->dims[i] != other->dims[i]){
            PyErr_SetString(PyExc_ValueError, "Shape Error!!!");
            return;
        }
    }

    ndForeachND(self, other, ndmultnd);
    return;
}

void ndarray_div_array(ndarray* self, ndarray* other){
    if(self->nd != other->nd){
        PyErr_SetString(PyExc_ValueError, "operands must have the same number of dimensions");
        return;
    }

    if(self->nd == 0){
        nddivnd(self->data, other->data, self->dtype);
        return;
    }

    for(size_t i = 0; i < self->nd; i++){
        if(self->dims[i] != other->dims[i]){
            PyErr_SetString(PyExc_ValueError, "Shape Error!!!");
            return;
        }
    }

    ndForeachND(self, other, nddivnd);
    return;
}

/*
 * SECTION 3: Python Functions
 * write to original matrix
 */


static PyObject* PyNDArray_add(ndarray *self, PyObject *arg){

    if (PyObject_TypeCheck(arg, &ndarrayType)) {
        ndarray *other = (ndarray *)arg;
        ndarray_add_array(self, other);
        Py_RETURN_NONE;
    }


    PyObject *float_obj = PyNumber_Float(arg);
    if (float_obj == NULL) {
        PyErr_SetString(PyExc_TypeError, "expected int or float");
        return NULL;
    }

    double val = PyFloat_AsDouble(float_obj);
    Py_DECREF(float_obj);

    if (PyErr_Occurred()) {
        return NULL;
    }

    ndForeachED(self, ndadd, val);

    Py_RETURN_NONE;
}

static PyObject* PyNDArray_sub(ndarray *self, PyObject *arg){

    PyObject *float_obj = PyNumber_Float(arg);
    if (float_obj == NULL) {
        PyErr_SetString(PyExc_TypeError, "expected int or float");
        return NULL;
    }

    double val = PyFloat_AsDouble(float_obj);
    Py_DECREF(float_obj);

    if (PyErr_Occurred()) {
        return NULL;
    }

    ndForeachED(self, ndsub, val);

    Py_RETURN_NONE;
}

static PyObject* PyNDArray_mult(ndarray *self, PyObject *arg){

    PyObject *float_obj = PyNumber_Float(arg);
    if (float_obj == NULL) {
        PyErr_SetString(PyExc_TypeError, "expected int or float");
        return NULL;
    }

    double val = PyFloat_AsDouble(float_obj);
    Py_DECREF(float_obj);

    if (PyErr_Occurred()) {
        return NULL;
    }

    ndForeachED(self, ndmult, val);

    Py_RETURN_NONE;
}

static PyObject* PyNDArray_div(ndarray *self, PyObject *arg){

    PyObject *float_obj = PyNumber_Float(arg);
    if (float_obj == NULL) {
        PyErr_SetString(PyExc_TypeError, "expected int or float");
        return NULL;
    }

    double val = PyFloat_AsDouble(float_obj);
    Py_DECREF(float_obj);

    if(val == 0.0){
        PyErr_SetString(PyExc_TypeError, "cannot divide by 0");
        return NULL;
    }

    if (PyErr_Occurred()) {
        return NULL;
    }

    ndForeachED(self, nddiv, val);

    Py_RETURN_NONE;
}

#ifdef __cplusplus
}
#endif

#endif