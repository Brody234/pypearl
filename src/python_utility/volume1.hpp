#ifndef VOLUME1HPP
#define VOLUME1HPP

#include <cstddef>
#include <Python.h> 

size_t* tuple_to_size_t_arr(PyObject* tuple);
#include "volume1.cpp"
#endif