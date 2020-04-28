#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int v_size;
    int size = atoi(argv[1]);
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;
    
    __m512 vec_a, vec_b, vec_c;
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_c = (float*) aligned_alloc (32, v_size*sizeof (float));
    //srand (time(NULL));
    for (int x = 0; x < v_size; x++){
        data_a[x] = rand() % 10 + 1;
        data_b[x] = rand() % 10 + 1;
    }
    /*for (int x = 0; x < v_size; x++) printf ("%f ", data_a[x]);
    printf ("\n");
    for (int x = 0; x < v_size; x++) printf ("%f ", data_b[x]);
    printf ("\n");*/
    for (int i = 0; i < v_size; i += 16) {
        vec_a = _mm512_load_ps (&data_a[i]);
        vec_b = _mm512_load_ps (&data_b[i]);
        vec_c = _mm512_add_ps(vec_a, vec_b);
        /*printf("a: ");
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
