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
dihedral* dihedralCInit(int64_t r, int64_t n, int64_t s);

// Python Facing Methods
static PyObject* PyDihedral_add_new(PyObject *a, PyObject *b);
static PyObject* PyDihedral_add(PyObject *a, PyObject *b);
static PyObject* PyDihedral_richcompare(PyObject* a, PyObject* b, int op);

#include "dihedral.cpp"

#endif