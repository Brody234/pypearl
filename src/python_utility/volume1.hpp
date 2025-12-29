#ifndef VOLUME1HPP
#define VOLUME1HPP

#include <cstddef>
#include <Python.h> 

int tuple_to_size_t_arr(PyObject* tuple, size_t* size, size_t* vals);
#include "volume1.cpp"
#endif