#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    int v_size = size * size;
    
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    
    int elem = sqrt (v_size);
    while (elem % 16 != 0) elem++;
    
    __m256 elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b;
    __m256 mul = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
    for (int i = 0; i < v_size; i += 8) {
        elem_a1 = _mm256_load_ps (&data_a[i]);
        elem_a2 = _mm256_loadu_ps (&data_a[i+elem-1]);
        elem_a3 = _mm256_load_ps (&data_a[i+elem]);
        elem_a4 = _mm256_loadu_ps (&data_a[i+elem+1]);
        elem_a5 = _mm256_load_ps (&data_a[i+elem*2]);
        elem_b = _mm256_add_ps(elem_a1, elem_a2);
        elem_b = _mm256_add_ps(elem_b, elem_a3);
        elem_b = _mm256_add_ps(elem_b, elem_a4);
        elem_b = _mm256_add_ps(elem_b, elem_a5);
        elem_b = _mm256_mul_ps(elem_b, mul);
        _mm256_stream_ps (&data_b[i+elem], elem_b);        
    }

    printf ("%f\n", data_b[v_size-1]);
    
    return 0;
}
