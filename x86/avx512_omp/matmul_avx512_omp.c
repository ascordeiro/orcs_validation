#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);

    if (size != 0 && (size & (size - 1)) == 0) {
        int m_size = sqrt((1024 * 1024 * size) / sizeof(float));
        while (m_size % 256 != 0) m_size++;
        int n_vectors = ceil(((double) m_size/(double) 16));
        float *matrix_a = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float *matrix_b = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float *matrix_c = (float *) aligned_alloc(32, sizeof (float) * m_size * (16 * n_vectors));
        float sum;

        int i, j, k;
        __m512 vec_a, vec_b, aux_vec;
        #pragma omp parallel shared (vec_a, vec_b) private (i, j, k, sum, aux_vec)
        {
            for (i = 0; i < m_size; ++i) {
                for (j = 0; j < m_size; ++j) {
                    sum = 0;
                    #pragma omp for schedule (dynamic)
                    for (k = 0; k < n_vectors; ++k) {
                        vec_a = _mm512_load_ps (&matrix_a[(i * 16 * n_vectors) + (k * 16)]);
                        vec_b = _mm512_load_ps (&matrix_b[(j * 16 * n_vectors) + (k * 16)]);
                        aux_vec = _mm512_mul_ps (vec_a, vec_b);
                        
                        for (int i = 0; i < 16; i++) sum += aux_vec[i];
                    }
                    matrix_c[(i * m_size) + j] = sum;
                }
            }
        }

        printf ("%f\n", matrix_c[m_size-1]);

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);

    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}