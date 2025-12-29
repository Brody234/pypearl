#ifndef DIRECT
#define DIRECT

#include <Python.h> 
#include <structmember.h> 
#include "symmetric.hpp"
#include "dihedral.hpp"
#include "znz.hpp"
#include "dihedral_matrix.hpp"
#include "../../matrix/structures/ndarray.hpp"

/*
 * My overall plan for direct products is to make an array of elements, each element will be an arbitrary group element. 
 * This allows for the direct product of any groups to exist.
 * type represents the group pyobject is, reducing calls to Python's API for type checking, and simplifying compiler optimizations on branching.
 * type corresponds to:
 * 0x0: Z/nZ
 * 0x1: Dihedral
 * 0x2: Dihedral Matrix
 * 0x3: Symmetric
 */
// Note this is not a PyObject itself for easy allocation since it doesn't need to be seen from Python, but does contain PyObjects.
typedef struct {
    // The group element
    PyObject* component;
    // The group the element is from
    uint32_t type;
} direct_element;

typedef struct {
    PyObject_HEAD
    
    // Array of Elements
    direct_element* values;
    
    // Length
    int64_t len;
} direct;

// Python handling
static void direct_dealloc(direct *self);
static PyObject * direct_str(direct *self);
static PyObject * direct_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int direct_init(direct *self, PyObject *args, PyObject *kwds);
extern PyMethodDef direct_methods[];
extern PyTypeObject directType;

// Methods
direct* directCInit(int64_t r, int64_t n, int64_t s);

// Python Methods
static PyObject* PyDirect_add_new(PyObject *a, PyObject *b);
static PyObject* PyDirect_add_new(PyObject *a, PyObject *b);
static PyObject* PyDirect_richcompare(PyObject* a, PyObject* b, int op);
#include "directproduct.cpp"

#endif