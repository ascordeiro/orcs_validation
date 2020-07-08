#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

int main(int argc, char const *argv[]) {
    int v_size;
    int size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;
    
    __m256 vec_a, vec_b, vec_c;
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_c = (float*) aligned_alloc (32, v_size*sizeof (float));
    
    int i;
    #pragma omp parallel shared (data_a, data_b, data_c) private (i, vec_a, vec_b, vec_c)
    {
        #pragma omp for schedule (dynamic)
        for (i = 0; i < v_size; i += 8) {
            vec_a = _mm256_load_ps (&data_a[i]);
            vec_b = _mm256_load_ps (&data_b[i]);
            vec_c = _mm256_add_ps(vec_a, vec_b);
            _mm256_store_ps (&data_c[i], vec_c);
        }
    }

    printf ("%f\n", data_c[v_size-1]);
    return 0;
}
