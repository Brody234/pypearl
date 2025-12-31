#ifndef DIHEDRALPOOLINGH
#define DIHEDRALPOOLINGH

#include "dihedral_matrix.hpp"

typedef struct {
    PyObject_HEAD

    size_t* in_size;

    size_t* out_size;

    ndarray* windows;
} dihedral_pool;

#endif