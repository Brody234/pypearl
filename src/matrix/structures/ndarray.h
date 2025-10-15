#ifndef NDARRAY
#define NDARRAY

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
//#include <Python.h> 

/*
 * Data Types
 * 0x0: float32
 * 0x1: float64
 * 0x2: int32
 * 0x3: int64
 */

typedef struct {
    //PyObject_HEAD;
    size_t nd;
    size_t* dims;
    size_t* strides;
    char* data;
    u_int8_t dtype;
} ndarray;

typedef void (*func)(void* elem, const size_t* idx, size_t nd);



ndarray arrayInit(size_t nd, u_int8_t dtype, size_t* dims);
int arrayGetElement(ndarray arr, void* out, size_t* idx);
int arraySetElement(ndarray arr, void* in, size_t* idx);

#include "ndarray.c"
#endif