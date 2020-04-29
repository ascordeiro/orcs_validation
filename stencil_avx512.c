#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

int main(int argc, char const *argv[]) {
    int size = atoi(argv[1]);
    int v_size;
    if (size != 0 && (size & (size - 1)) == 0) v_size = (1024 * 1024 * size) / sizeof(float);
    else return 0;
    
    float* data_a = (float*) aligned_alloc (32, v_size*sizeof (float));
    float* data_b = (float*) aligned_alloc (32, v_size*sizeof (float));
    //for (int x = 0; x < v_size; x++) data_a[x] = rand() % 10 + 1;
    int elem = sqrt (v_size);
    while (elem % 16 != 0) elem++;
    /*for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            printf ("%.0f ", data_a[x+y]);
        }
        printf ("\n");
    }*/
    
    __m512 elem_a1, elem_a2, elem_a3, elem_a4, elem_a5, elem_b;
    __m512 mul = {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0};
    for (int i = 0; i < v_size; i += 16) {
        elem_a1 = _mm512_load_ps (&data_a[i]);
        elem_a2 = _mm512_loadu_ps (&data_a[i+elem-1]);
        elem_a3 = _mm512_load_ps (&data_a[i+elem]);
        elem_a4 = _mm512_loadu_ps (&data_a[i+elem+1]);
        elem_a5 = _mm512_load_ps (&data_a[i+elem*2]);
        elem_b = _mm512_add_ps(elem_a1, elem_a2);
        elem_b = _mm512_add_ps(elem_b, elem_a3);
        elem_b = _mm512_add_ps(elem_b, elem_a4);
        elem_b = _mm512_add_ps(elem_b, elem_a5);
        elem_b = _mm512_mul_ps(elem_b, mul);
        _mm512_stream_ps (&data_b[i], elem_b);
    }
    /*printf ("\n\n\n");
    for (int x = 0; x < elem; x++){
        for (int y = 0; y < elem; y++){
            printf ("%.0f ", data_b[x+y]);
        }
        printf ("\n");
    }*/
    return 0;
}
