#ifndef DIHEDRALACTIVATIONH
#define DIHEDRALACTIVATIONH

#include "abstract/groups/dihedral_matrix.hpp"
#include <iostream>
#include <random>
#include <chrono>

/*
 * This is currently a test file. I'm going to make a few activation functions for dihedral neural networks.
 * 
 * Type:
 * 0x0: S activation: If s is odd (the rotation will flip a shape), it becomes the identity.
 * 0x1: R activation: If the power of r is greater than half of n, it becomes the identity.
 * 0x2: Bottom activation: With r^k if k>n/2 && s == 0 || k<n/2 && s == 0 then identity.
 * 
 */

typedef struct {
    PyObject_HEAD

    uint8_t type;

} dihedral_activation;

// Python handling
static void dihedral_activation_dealloc(dihedral_activation *self);
static PyObject * dihedral_activation_str(dihedral_activation *self);
static PyObject * dihedral_activation_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int dihedral_activation_init(dihedral_activation *self, PyObject *args, PyObject *kwds);
extern PyMethodDef dihedral_activation_methods[];
extern PyTypeObject dihedral_activationType;

#include "dihedralactivation.cpp"

#endif