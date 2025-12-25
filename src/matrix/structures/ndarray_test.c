#include "ndarray.hpp"

int main(){
    size_t* dims;
    dims = (size_t*) malloc(2 * sizeof(size_t));
    dims[0] = 3;
    dims[1] = 2;
    size_t idx[] = {1, 0};
    int32_t newval = 0x13;
    int32_t curval = 0x2;
    ndarray arr = arrayCInit(2, 0x0, dims);
    ndPrint(&arr);
    arrayGetElement(arr, &curval, idx);
    printf("%d\n", curval);

    arraySetElement(arr, &newval, idx);
    arrayGetElement(arr, &curval, idx);
    printf("%d\n", curval);

    ndPrint(&arr);
    return 1;
}