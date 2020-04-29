#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int v_size;
    int size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;

    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    
    __m512 vec_a;
    for (int i = 0; i < v_size; i += 16) {
        vec_a = _mm512_load_ps (&data_a[i]);
        vec_a = _mm512_set1_ps(1.0);
        /*printf("after  a: ");
        for (int x = 0; x < 16; x++) printf ("%f ", vec_a[x]);
        printf ("\n");
        printf("b: ");
        for (int x = 0; x < 16; x++) printf ("%f ", vec_b[x]);
        printf ("\n");
        printf("c: ");
        for (int x = 0; x < 16; x++) printf ("%f ", vec_c[x]);
        printf ("\n");*/
    }
    return 0;
}
