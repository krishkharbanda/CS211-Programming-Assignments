#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <complex.h>

/* Markers used to bound trace regions of interest */
volatile char MARKER_START, MARKER_END;

int main(int argc, char* argv[])
{

    /* Record marker addresses */
    FILE* marker_fp = fopen(".marker","w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx\n%llx",
        (unsigned long long int) &MARKER_START,
        (unsigned long long int) &MARKER_END );
    fclose(marker_fp);

    if (argc!=3) {
        printf("Usage: ./matMul <matrix_a_file> <matrix_b_file>\n");
        exit(EXIT_FAILURE);
    }

    FILE* matrix_a_fp = fopen(argv[1], "r");
    if (!matrix_a_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t n;
    int ret = fscanf(matrix_a_fp, "%ld\n", &n);
    assert (ret==1);
    complex* a = calloc( n*n, sizeof(complex) );
    for ( size_t i=0; i<n; i++ ) {
        for ( size_t k=0; k<n; k++ ) {
            double real, imag;
            ret = fscanf(matrix_a_fp, "(%lf%lfj) ", &real, &imag);
            assert (ret==2);
            // printf("real=%lf\n",real);
            // printf("imag=%lf\n",imag);
            a[i*n+k] = CMPLX(real, imag);
        }
        ret = fscanf(matrix_a_fp, "\n");
    }
    fclose(matrix_a_fp);

    FILE* matrix_b_fp = fopen(argv[2], "r");
    if (!matrix_b_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    size_t m;
    ret = fscanf(matrix_b_fp, "%ld\n", &m);
    assert (ret==1);
    assert( n==m );
    complex* b = calloc( n*n, sizeof(complex) );
    for ( size_t k=0; k<n; k++ ) {
        for ( size_t j=0; j<n; j++ ) {
            double real, imag;
            ret = fscanf(matrix_b_fp, "(%lf%lfj) ", &real, &imag);
            assert (ret==2);
            // printf("real=%lf\n",real);
            // printf("imag=%lf\n",imag);
            b[k*n+j] = CMPLX(real, imag);
        }
        ret = fscanf(matrix_b_fp, "\n");
    }
    fclose(matrix_b_fp);

    complex* c = calloc( n*n, sizeof(complex) );
    MARKER_START = 211;

    int block = 4;

    if(n % 2 == 1){
        block = 3;
    }
     
    for (int j = 0; j < n; j += block) {
        for (int k = 0; k < n; k += block) {
            for (int i = 0; i < n; i++) {
                int end = j + block;
                if(end > n){
                    end = n;
                }
                for(int jj = j; jj < end; jj++){
                    complex double r = 0.0;
                    int end2 = k + block;
                    if(end2 > n){
                        end2 = n;
                    }
                    for(int kk = k; kk < end2; kk++){
                        r += a[i*n+kk] * b[kk*n+jj];
                    }
                    c[i*n+jj] += r;
                }
            }
        }
    }

    MARKER_END = 211;

    for ( size_t i=0; i<n; i++ ) {
        for ( size_t j=0; j<n; j++ ) {
            if (cimag(c[i*n+j])<0) {
                printf( "(%.12lf%.12lfj) ", creal(c[i*n+j]), cimag(c[i*n+j]) );
            } else {
                printf( "(%.12lf+%.12lfj) ", creal(c[i*n+j]), cimag(c[i*n+j]) );
            }
        }
        printf( "\n" );
    }

    free(c);
    free(b);
    free(a);
    exit(EXIT_SUCCESS);

}