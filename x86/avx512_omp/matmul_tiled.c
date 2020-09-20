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
        int blocks = m_size*m_size/2, idx = 0;
        __m512 vec_a, vec_b, aux_vec;
        for (int i = 0; i < m_size * m_size; i += blocks) {
            idx = i;
            for (int j = 0; j < m_size; j += m_size/2) {
                for (int k = (i + j); k < (i + j) + blocks; k += m_size) {
                    for (int l = (i + j); l < (i + j) + blocks; l += m_size) {
                        for (int m = 0; m < m_size/2; m += 16) {
                            vec_a = _mm512_load_ps(&matrix_a[k + m]);
                            vec_b = _mm512_load_ps(&matrix_b[l + m]);
                            aux_vec = _mm512_mul_ps (vec_a, vec_b);
                            matrix_c[idx] += _mm512_reduce_add_ps(aux_vec);
                        }
                        idx++;
                    }
                }
            }
        }
    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}