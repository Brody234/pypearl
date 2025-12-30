#ifndef DIHEDRALLAYERH
#define DIHEDRALLAYERH

#include "abstract/groups/dihedral_matrix.hpp"
#include <iostream>
#include <random>
#include <chrono>

typedef struct {
    PyObject_HEAD

    dihedral_matrix* weights;

} dihedral_layer;

// Python handling
static void dihedral_layer_dealloc(dihedral_layer *self);
static PyObject * dihedral_layer_str(dihedral_layer *self);
static PyObject * dihedral_layer_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int dihedral_layer_init(dihedral_layer *self, PyObject *args, PyObject *kwds);
extern PyMethodDef dihedral_layer_methods[];
extern PyTypeObject dihedral_layerType;

#include "dihedrallayer.cpp"

#endif