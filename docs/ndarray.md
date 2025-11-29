# NDArray Class
Documention for ndarray class.

## Inspiration
Skip this section if you're just trying to solve a problem. This object is inspired by NumPy's ndarray and much of the structure is based directly on NumPy source code. I chose to do this in my own library as NumPy expects users to use their C API for libraries that extend NumPy in C or C++. This is obviously not ideal, as my library has a lot of functions where if nd != 2, the function won't work, like a layer forward pass, so from there I can directly influence the dot product with pointer arithmetic. Additionally, I can save more time on my dot products with branches for if strides suggest the array is not transposed and can be treated as a contiguous block of memory. This is actually huge because assuming strides[0] = 1 on a 2 dimensional matrix dot product removes 2 reads to memory (can be mitigated) and 2 floating point operations per step. Basically, I like to have exact control over everything, NumPy does not let me do this, so I made my own NumPy. Is this actually useful? Probably not, but it got me through a job interview one time so you also can't say it's useless.

## Functions
How to interact with ndarray. For these docs, assume objects A, B, C are ndarray objects. The phrase "x is a view of y" or any other phrase with the term view will always mean that x and y refer to the same data in memory. While they may have different shape, be transposed, be cut, or anything else, they access the same memory.

### dot 
C = A.dot(B), performs the dot product AB and creates new matrix C. Requires nd = 2 for all matrices, due to GEMM's being 2d. I will make an nd tensor dot and vector dot eventually but this is an ML library so I gotta get the core stuff in first.

### transpose
B = A.tranpose(). Performs the transpose operation on A, returns a view of A. A will still act the same, while B will be a transposed view of A's data.

### shape
Returns a Python tuple containing the shape of the array. If A is a 2x4 matrix, shape will be (2, 4). If B is a view of the first 3 columns of A transposed, it's shape will be (3, 2).