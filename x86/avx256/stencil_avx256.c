#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    //int v_size = (1024 * 1024 * size) / sizeof(float);
    int v_size = size * size;
    int i;
    
    __m256 vec_a;
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    for (i = 0; i < v_size; i += 8) {
        vec_a = _mm256_load_ps (&data_a[i]);
        vec_a = _mm256_set1_ps((float) 1.0);
        _mm256_store_ps (&data_a[i], vec_a);
    }
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    for (i = 0; i < v_size; i += 8) {
        vec_a = _mm256_load_ps (&data_b[i]);
        vec_a = _mm256_set1_ps((float) 0.0);
        _mm256_store_ps (&data_b[i], vec_a);
    }
    
    int elem = sqrt (v_size);
    int remainder = 0;
    
    __m256 elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b;
    __m256 mul = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
    //for (int i = elem; i < i+elem+8; i += 8) {
    for (int i = elem; i+elem+8 < v_size; i += 8) {
        elem_a1 = _mm256_load_ps (&data_a[i-elem]);
        elem_a2 = _mm256_loadu_ps (&data_a[i-1]);
        elem_a3 = _mm256_load_ps (&data_a[i]);
        elem_a4 = _mm256_loadu_ps (&data_a[i+1]);
        elem_a5 = _mm256_load_ps (&data_a[i+elem]);
        elem_b = _mm256_add_ps(elem_a1, elem_a2);
        elem_b = _mm256_add_ps(elem_b, elem_a3);
        elem_b = _mm256_add_ps(elem_b, elem_a4);
        elem_b = _mm256_add_ps(elem_b, elem_a5);
        elem_b = _mm256_mul_ps(elem_b, mul);
        _mm256_stream_ps (&data_b[i], elem_b);        
        remainder = i;
    }

    for (int i = 0; i < elem; i++){
        if (i-elem > 0) data_b[i] += data_a[i-elem];
        if (i-1 > 0) data_b[i] += data_a[i-1];
        data_b[i] += data_a[i];
        data_b[i] += data_a[i+1];
        data_b[i] += data_a[i+elem];
        data_b[i] *= mul[i];
    }

    /*for (int i = remainder+elem+8; i < v_size; i++){
        printf ("%d ", i);
        data_b[i] += data_a[i-elem];
        data_b[i] += data_a[i-1];
        data_b[i] += data_a[i];
        if (i+1 < v_size) data_b[i] += data_a[i+1];
        if (i+elem < v_size) data_b[i] += data_a[i+elem];
        data_b[i] *= mul[i];
    }*/

    printf ("\n-------------------------\n");
    for (int x = 0; x < v_size; x++){
        if (x % elem == 0) printf ("\n");
        printf ("%.0lf ", data_b[x]);
    }

    //printf ("%f\n", data_b[v_size-1]);
    
    return 0;
}
