#ifndef NDARRAY_C
#define NDARRAY_C

#include "./ndarray.h"

void zero4(void* elem, const size_t* idx, size_t nd){
    (void)nd;
    (void)idx;
    *(int32_t*)elem = 0x0;
}

void ndForeach(ndarray* arr, func visit){
    char* cur_elem = arr->data;

    size_t* cur_idx = (size_t*)malloc(arr->nd*sizeof(size_t));
    for(size_t i = 0; i < arr->nd; i++) cur_idx[i] = 0;

    for(;;){
        (*visit)(cur_elem, cur_idx, arr->nd);
        for(ssize_t k = (ssize_t)arr->nd-1; k >=0; k--){
            cur_idx[k]++;
            cur_elem+=arr->strides[k];

            if(cur_idx[k]<arr->dims[k]){
                goto next_element;
            }
            cur_elem -= arr->strides[k]*arr->dims[k];
            cur_idx[k] = 0;
        }
        break;
        next_element:
        ;
    }
}

// WARNING DOES NOT WORK IF YOU EVER ADD A TYPE OF DATA THAT IS NOT 4n BYTES n \in \Z_{>0}
ndarray arrayInit(size_t nd, u_int8_t dtype, size_t* shape){
    // Initialize dims
    size_t* dims;
    dims = (size_t*) malloc(nd * sizeof(size_t));

    size_t* strides;
    strides = (size_t*) malloc(nd * sizeof(size_t));


    size_t datalength = 0x0;
    if(dtype == 0x0 || dtype == 0x2){
        datalength = 0x4;
    }
    else if (dtype == 0x1 || dtype == 0x3){
        datalength = 0x8;
    }
    char* data;

    if(datalength == 0x0){
        fprintf(stderr, "65 dimensions is max!\n");
        exit(EXIT_FAILURE); 
    }

    size_t size = datalength;

    // Let's my LCV be a long, old numpy docs say max dims is 64 so I am one upping NumPy
    if(nd > 66){
        fprintf(stderr, "65 dimensions is max!\n");
        exit(EXIT_FAILURE); 
    }
    if(nd == 0){
        fprintf(stderr, "You wrote 0 dimensions!\n");
        exit(EXIT_FAILURE); 
    }
    // Initialize dims and strides
    for(long i = nd-1; i >= 0; i--){
        dims[i] = shape[i];
        strides[i] = size;
        size *= shape[i];
    }
    // If your fuzzer brought you here the bug is that even negative dimensions can pass this check. I don't want nd many checks.
    if(size <= 0){
        fprintf(stderr, "All dimensions must be positive!\n");
        exit(EXIT_FAILURE); 
    }    
    
    data = (char*) malloc(size * datalength * sizeof(char));

    ndarray obj = {nd, dims, strides, data, dtype};

    ndForeach(&obj, zero4);

    return obj;
    /*ndarray *obj = (ndarray *)NdArrayType.tp_alloc(&NdArrayType, 0);

    obj->nd = nd;
    obj->dims = dims;
    obj->strides = strides;
    obj->data = data;
    obj->dtype = dtype;
    return (*obj);*/
}

int arraySetElement(ndarray arr, void* in, size_t* idx){
    char* start_idx = arr.data;
    for(size_t i = 0; i < arr.nd; i++){
        if(idx[i] >= arr.dims[i]){
            return -1;
        }
        start_idx += idx[i]*arr.strides[i];
    }
    if(arr.dtype == 0x0 || arr.dtype == 0x2){
        memcpy(start_idx, in, 4);
        return 0;
    }
    else if(arr.dtype == 0x1 || arr.dtype == 0x3){
        memcpy(start_idx, in, 8);
        return 0;
    }

    else{
        return -1;
    }
}

int arrayGetElement(ndarray arr, void* out, size_t* idx){
    char* start_idx = arr.data;
    for(size_t i = 0; i < arr.nd; i++){
        if(idx[i] >= arr.dims[i]){
            return -1;
        }

        start_idx += idx[i]*arr.strides[i];
    }

    if(arr.dtype == 0x0 || arr.dtype == 0x2){
        memcpy(out, start_idx, 4);
        return 0;
    }
    else if(arr.dtype == 0x1 || arr.dtype == 0x3){
        memcpy(out, start_idx, 8);
        return 0;
    }

    else{
        return -1;
    }
}

// Unsafe fast get
inline void fastGet2D4(ndarray arr, size_t i, size_t j, void* out){
    memcpy(out,arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j], 4);
}

// Unsafe fast get
inline void fastGet2D8(ndarray arr, size_t i, size_t j, void* out){
    memcpy(out, arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j], 8);
}

// Unsafe fast set
inline void fastSet2D4(ndarray arr, size_t i, size_t j, void* in){
    memcpy(arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j], in, 4);
}

// Unsafe fast set
inline void fastSet2D8(ndarray arr, size_t i, size_t j, void* in){
    memcpy(arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j], in, 8);
}

// int32 fast inc
inline void fastIncInt32(ndarray arr, size_t i, size_t j, int32_t val){
    int32_t x = (*(int32_t*) (arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j]));
    x+=val;
    fastSet2D4(arr, i, j, &x);
}

// Unsafe fast get
inline void fastIncInt64(ndarray arr, size_t i, size_t j, void* out){
    memcpy(out, arr.data+arr.strides[i]*arr.dims[i]+arr.strides[j]+arr.dims[j], 8);
}

void printElemI32(void* elem, const size_t* idx, size_t nd){
    (void)idx;
    int32_t* val = (int32_t*) elem;
    printf("[");
    for(size_t i = 0; i < nd; i++){
        printf(" %lu ", idx[i]);
    }
    printf("]: %d\n", (*val));
}

void ndPrint(ndarray* arr){
    ndForeach(arr, printElemI32);
}

#endif