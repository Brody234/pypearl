#ifndef DIHEDRALPINCHERH
#define DIHEDRALPINCHERH

#include "abstract/groups/dihedral_matrix.hpp"
#include <stdexcept>

/*
 * This function lowkey came to me in a dream. 
 * It's literally just one bitshift, I'm not Gauss, but geometrically, I think it's overcomplicated.
 * I don't want people to just think of it as oh reduce it like relu, but rather as a well defined non injective geometric operation.
 * 
 * Imagine the dihedral element, d, as the symmetries of a shape, s1, in R2.
 * Now, create a shape s2 with fewer vertices whose vertices are an integer factor,m, of s1, where s1 has n*m vertices.
 * Draw s1. Draw s2 such that every nth vertex lines up with each nth vertex of s1.
 * Apply the d to s1 and have it move s2 with it.
 * If d is not also a symmetry of s2, keep applying r^{-1} until it is. (Or maybe r if reflected i don't know)
 * Once you are at this position, figure out what was applied to s2, and make that symmetry the new element.
 * 
 * I like to imagine this as literally pinching the element into a lower n.
 */

typedef struct {
    PyObject_HEAD

    int64_t n_start; // the n to start with
    int64_t n_end; // the n to end with

    int64_t factor; // n_start/n_end, yes it can be recomputed, but n_start is only useful for checks so it will (probably) be an extra division without saving a read. Plus it'll be a l1 cache read because this is an argument to forward.
} dihedral_pincher;

// Python handling
static void dihedral_pincher_dealloc(dihedral_pincher *self);
static PyObject * dihedral_pincher_str(dihedral_pincher *self);
static PyObject * dihedral_pincher_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int dihedral_pincher_init(dihedral_pincher *self, PyObject *args, PyObject *kwds);
extern PyMethodDef dihedral_pincher_methods[];
extern PyTypeObject dihedral_pincherType;

#include "dihedralpincher.cpp"


#endif