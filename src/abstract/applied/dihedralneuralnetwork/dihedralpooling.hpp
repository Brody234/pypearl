#ifndef DIHEDRALPOOLINGH
#define DIHEDRALPOOLINGH

#include "abstract/groups/dihedral_matrix.hpp"
#include <stdexcept>
typedef struct {
    PyObject_HEAD

    size_t* in_size;

    size_t* out_size;

    ndarray* windows;

    size_t step_0;

    uint64_t n;

    size_t step_1;

} dihedral_pool;

// Python handling
static void dihedral_pool_dealloc(dihedral_pool *self);
static PyObject * dihedral_pool_str(dihedral_pool *self);
static PyObject * dihedral_pool_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int dihedral_pool_init(dihedral_pool *self, PyObject *args, PyObject *kwds);
extern PyMethodDef dihedral_pool_methods[];
extern PyTypeObject dihedral_poolType;

#include "dihedralpooling.cpp"


#endif