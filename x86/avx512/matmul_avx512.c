#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int MIN (int a, int b){
    if (a < b) return a;
    else return b;
}

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);

    if (size != 0 && (size & (size - 1)) == 0) {
        int m_size = sqrt((1024 * 1024 * size) / sizeof(float));
        while (m_size % 256 != 0) m_size++;
        float *matrix_a = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);
        float *matrix_b = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);
        float *matrix_c = (float *) aligned_alloc(32, sizeof (float) * m_size * m_size);

        printf ("%d \n", m_size);

        /*for (int x = 0; x < m_size; x++){
            for (int y = 0; y < m_size; x++){
                matrix_a[x*y] = rand() % 10 + 1;
                matrix_b[x*y] = rand() % 10 + 1;
            }
        }*/

        /*printf ("a: \n");
        for (int i = 0; i < m_size*16*n_vectors; i++) {
            if (i != 0 && i % m_size == 0) printf ("\n");
            printf ("%.0f ", matrix_a[i]);
        }
        printf ("\n\nb: \n");
        for (int i = 0; i < m_size*16*n_vectors; i++) {
            if (i != 0 && i % m_size == 0) printf ("\n");
            printf ("%.0f ", matrix_b[i]);
        }

        printf ("\n");*/

        int BLOCKSIZE = 64 / sizeof (float);
        
        int i, j, k, i0, k0, j0;
        __m512 vec_a, vec_b, aux_vec;
        for(i0 = 0; i0 < m_size; i0 += BLOCKSIZE) {
            for(k0 = 0; k0 < m_size; k0 += BLOCKSIZE) {
                for(j0 = 0; j0 < m_size; j0 += BLOCKSIZE) {
                    for (i = i0; i < MIN(i0+BLOCKSIZE, m_size); i+=4) {
                        for (j = j0; j < MIN(j0+BLOCKSIZE, m_size); j+=4)  {
                            //printf ("C: i = %d, j = %d\n", i, j);
                            aux_vec = _mm512_load_ps(&matrix_c[i*j]);
                            for (k = k0; k < MIN(k0+BLOCKSIZE, m_size); k+=4) {
                                //printf ("A: i = %d, k = %d\n", i, k);
                                vec_a = _mm512_load_ps (&matrix_a[i*k]);
                                //printf ("\nvec_a  : ");
                                //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_a[i]);
                                //printf ("B: k = %d, j = %d\n", k, j);
                                vec_b = _mm512_load_ps (&matrix_b[k*j]);
                                //printf ("\nvec_b  : ");
                                //for (int i = 0; i < 16; i++) printf ("%.0f ", vec_b[i]);
                                aux_vec = _mm512_add_ps (aux_vec, _mm512_mul_ps (vec_a, vec_b));
                            }
                            //printf ("C: i = %d, j = %d\n", i, j);
                            _mm512_store_ps(&matrix_c[i*j], aux_vec);
                        }
                    }
                }
            }
        }

        printf ("\n\nc: ");
        for (int i = 0; i < m_size; i++) {
            for (int i = 0; i < m_size; i++) {
                printf ("%.0f ", matrix_c[i*j]);
            }
        }

        //printf ("%f\n", matrix_c[m_size-1]);

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);

    } else {
        printf("Error! Size is not power of two!\n");
        exit(1);
    }
    return 0;
}