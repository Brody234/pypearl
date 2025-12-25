#ifndef DIHEDRALGROUP
#define DIHEDRALGROUP

#include <Python.h> 
#include <structmember.h> 
#include "../../matrix/structures/ndarray.hpp"

typedef struct {
    PyObject_HEAD
    
    // Sign
    int64_t s;
    
    // Rotations
    int64_t r;

    int64_t n;
    
} dihedral;

// Python handling
static void dihedral_dealloc(dihedral *self);
static PyObject * dihedral_str(dihedral *self);
static PyObject * dihedral_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int dihedral_init(dihedral *self, PyObject *args, PyObject *kwds);
extern PyMethodDef dihedral_methods[];
extern PyTypeObject dihedralType;

// Methods
dihedral* dihedralCInit(uint64_t r, uint64_t n, uint8_t s);
static PyObject* PyDihedral_add_new(PyObject *Pyself, PyObject *arg);

#include "dihedral.cpp"

#endif