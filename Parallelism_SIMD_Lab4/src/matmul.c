/**
 * SSE matrix multiplication. Bonus assignment.
 *
 *
 * Course: Advanced Computer Architecture, Uppsala University
 * Course Part: Lab assignment 4
 *
 * Author: Andreas Sandberg <andreas.sandberg@it.uu.se>
 *
 * $Id: matmul.c 70 2011-11-22 10:07:10Z ansan501 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <smmintrin.h>

#ifndef __SSE4_1__
#error This example requires SSE4.1
#endif

#include "util.h"

/* Size of the matrices to multiply */
#define SIZE 4

#define SSE_BLOCK_SIZE 4

#ifndef L1_BLOCK_SIZE
#define L1_BLOCK_SIZE 4
#endif

#ifndef L2_BLOCK_SIZE
#define L2_BLOCK_SIZE 4
#endif

/* A mode that controls how the matrix multiplication is optimized may
 * be specified at compile time. The following modes are defined:
 *
 * MODE_SSE - A simple, non-blocked, implementation of the matrix
 * multiplication.
 *
 * MODE_SSE_BLOCKED - A blocked matrix multiplication with implemented
 * using a 4x4 SSE block.
 *
 * MODE_BLOCKED - Blocked matrix mutliplication using ordinary
 * floating point math.
 */
#define MODE_SSE_BLOCKED 1
#define MODE_SSE 2
#define MODE_BLOCKED 3

#ifndef MODE
#define MODE MODE_SSE
#endif


#define XMM_ALIGNMENT_BYTES 16 

static float mat_a[SIZE][SIZE] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float mat_b[SIZE][SIZE] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float mat_c[SIZE][SIZE] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));
static float mat_ref[SIZE][SIZE] __attribute__((aligned (XMM_ALIGNMENT_BYTES)));

/**
 * Blocked matrix multiplication, SSE block (4x4 matrix). Implement
 * your solution to the bonus assignment here.
 */
static inline void
matmul_sse_block(int i, int j, int k)
{
        /* BONUS TASK: Implement your SSE 4x4 matrix multiplication
         * block here. */
        /* HINT: You might find at least the following instructions
         * useful:
         *  - _mm_dp_ps
         *  - _MM_TRANSPOSE4_PS
         *
         * HINT: The result of _mm_dp_ps is scalar. The third
         * parameter can be used to restrict to which elements the
         * result is stored, all other elements are set to zero.
         */

//printf("hello :(\n\n");
for ( int i = 0; i<SIZE; i+=4){
    for ( int k = 0; k<SIZE; k+=4){
        
        __m128 rowb0 = _mm_load_ps(&mat_b[i][k]);
        __m128 rowb1 = _mm_load_ps(&mat_b[i+1][k]);
        __m128 rowb2 = _mm_load_ps(&mat_b[i+2][k]);
        __m128 rowb3 = _mm_load_ps(&mat_b[i+3][k]);

        _MM_TRANSPOSE4_PS (rowb0, rowb1, rowb2, rowb3);

        for (int j = 0; j <4; j++){
            __m128 rowa_int = _mm_load_ps(&mat_a[i+j][k]);

            __m128 res_0 = _mm_mul_ps(rowa_int , rowb0);
            res_0 = _mm_hadd_ps(res_0, res_0);
            res_0 = _mm_hadd_ps(res_0, res_0);
            _mm_store_ss(&mat_c[i+j][k+0], res_0);

            __m128 res_1 = _mm_mul_ps(rowa_int, rowb1);
            res_1 = _mm_hadd_ps(res_1, res_1);
            res_1 = _mm_hadd_ps(res_1, res_1);
            _mm_store_ss(&mat_c[i+j][k+1], res_1);

            __m128 res_2 = _mm_mul_ps(rowa_int , rowb2);
            res_2 = _mm_hadd_ps(res_2, res_2);
            res_2 = _mm_hadd_ps(res_2, res_2);
            _mm_store_ss(&mat_c[i+j][k+2], res_2);

            __m128 res_3 = _mm_mul_ps(rowa_int , rowb3);
            res_3 = _mm_hadd_ps(res_3, res_3);
            res_3 = _mm_hadd_ps(res_3, res_3);
            _mm_store_ss(&mat_c[i+j][k+3], res_3);
    
        
        }
    }
}

}

/**
 * Blocked matrix multiplication, SSE block (4x4 matrix) implemented
 * using ordinary floating point math.
 */
static inline void
matmul_block(int i, int j, int k)
{
        mat_c[i][j] += 
                mat_a[i][k] * mat_b[k][j]
                + mat_a[i][k + 1] * mat_b[k + 1][j]
                + mat_a[i][k + 2] * mat_b[k + 2][j]
                + mat_a[i][k + 3] * mat_b[k + 3][j];

        mat_c[i][j + 1] += 
                mat_a[i][k] * mat_b[k][j + 1]
                + mat_a[i][k + 1] * mat_b[k + 1][j + 1]
                + mat_a[i][k + 2] * mat_b[k + 2][j + 1]
                + mat_a[i][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i][j + 2] += 
                mat_a[i][k] * mat_b[k][j + 2]
                + mat_a[i][k + 1] * mat_b[k + 1][j + 2]
                + mat_a[i][k + 2] * mat_b[k + 2][j + 2]
                + mat_a[i][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i][j + 3] += 
                mat_a[i][k] * mat_b[k][j + 3]
                + mat_a[i][k + 1] * mat_b[k + 1][j + 3]
                + mat_a[i][k + 2] * mat_b[k + 2][j + 3]
                + mat_a[i][k + 3] * mat_b[k + 3][j + 3];



        mat_c[i + 1][j] += 
                mat_a[i + 1][k] * mat_b[k][j]
                + mat_a[i + 1][k + 1] * mat_b[k + 1][j]
                + mat_a[i + 1][k + 2] * mat_b[k + 2][j]
                + mat_a[i + 1][k + 3] * mat_b[k + 3][j];

        mat_c[i + 1][j + 1] += 
                mat_a[i + 1][k] * mat_b[k][j + 1]
                + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 1]
                + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 1]
                + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 1][j + 2] += 
                mat_a[i + 1][k] * mat_b[k][j + 2]
                + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 2]
                + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 2]
                + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 1][j + 3] += 
                mat_a[i + 1][k] * mat_b[k][j + 3]
                + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 3]
                + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 3]
                + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 3];



        mat_c[i + 2][j] += 
                mat_a[i + 2][k] * mat_b[k][j]
                + mat_a[i + 2][k + 1] * mat_b[k + 1][j]
                + mat_a[i + 2][k + 2] * mat_b[k + 2][j]
                + mat_a[i + 2][k + 3] * mat_b[k + 3][j];

        mat_c[i + 2][j + 1] += 
                mat_a[i + 2][k] * mat_b[k][j + 1]
                + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 1]
                + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 1]
                + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 2][j + 2] += 
                mat_a[i + 2][k] * mat_b[k][j + 2]
                + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 2]
                + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 2]
                + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 2][j + 3] += 
                mat_a[i + 2][k] * mat_b[k][j + 3]
                + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 3]
                + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 3]
                + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 3];



        mat_c[i + 3][j] += 
                mat_a[i + 3][k] * mat_b[k][j]
                + mat_a[i + 3][k + 1] * mat_b[k + 1][j]
                + mat_a[i + 3][k + 2] * mat_b[k + 2][j]
                + mat_a[i + 3][k + 3] * mat_b[k + 3][j];

        mat_c[i + 3][j + 1] += 
                mat_a[i + 3][k] * mat_b[k][j + 1]
                + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 1]
                + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 1]
                + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 3][j + 2] += 
                mat_a[i + 3][k] * mat_b[k][j + 2]
                + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 2]
                + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 2]
                + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 3][j + 3] += 
                mat_a[i + 3][k] * mat_b[k][j + 3]
                + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 3]
                + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 3]
                + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 3];

/*
 * The code in this function can alternatively be expressed using macros:
 */
/*
#define BLOCK_SUB(n, m, l) ( mat_a[i + n][k + l] * mat_b[k + l][j + m] )

#define BLOCK_CELL(n, m) { \
        mat_c[i + n][j + m] += \
                BLOCK_SUB(n, m, 0) + \
                BLOCK_SUB(n, m, 1) + \
                BLOCK_SUB(n, m, 2) + \
                BLOCK_SUB(n, m, 3) ; \
}

#define BLOCK_ROW(n) { \
        BLOCK_CELL(n, 0); \
        BLOCK_CELL(n, 1); \
        BLOCK_CELL(n, 2); \
        BLOCK_CELL(n, 3); \
}

        BLOCK_ROW(0);
        BLOCK_ROW(1);
        BLOCK_ROW(2);
        BLOCK_ROW(3);
*/

}

#if MODE == MODE_SSE_BLOCKED || MODE == MODE_BLOCKED
/**
 * Blocked matrix multiplication, L1 block.
 */
static inline void
matmul_block_l1(int i, int j, int k)
{
        int ii, jj, kk;

        for (ii = i; ii < i + L1_BLOCK_SIZE; ii += SSE_BLOCK_SIZE)
                for (kk = k; kk < k + L1_BLOCK_SIZE; kk += SSE_BLOCK_SIZE)
                        for (jj = j; jj < j + L1_BLOCK_SIZE; jj += SSE_BLOCK_SIZE) {
#if MODE == MODE_SSE_BLOCKED
                                matmul_sse_block(ii, jj, kk);
#elif MODE == MODE_BLOCKED
                                matmul_block(ii, jj, kk);
#endif
                        }
}

/**
 * Blocked matrix multiplication, L2 block.
 */
static inline void
matmul_block_l2(int i, int j, int k)
{
        int ii, jj, kk;

        for (ii = i; ii < i + L2_BLOCK_SIZE; ii += L1_BLOCK_SIZE)
                for (kk = k; kk < k + L2_BLOCK_SIZE; kk += L1_BLOCK_SIZE)
                        for (jj = j; jj < j + L2_BLOCK_SIZE; jj += L1_BLOCK_SIZE)
                                matmul_block_l1(ii, jj, kk);
}

/**
 * Blocked matrix multiplication, entry function for multiplying two
 * matrices.
 */
static void
matmul_sse()
{
        int i, j, k;

        for (i = 0; i < SIZE; i += L2_BLOCK_SIZE)
                for (k = 0; k < SIZE; k += L2_BLOCK_SIZE)
                        for (j = 0; j < SIZE; j += L2_BLOCK_SIZE)
                                matmul_block_l2(i, j, k);
}

#elif MODE == MODE_SSE

/**
 * Matrix multiplication. This is the procedure you should try to
 * optimize.
 */
static void
matmul_sse()
{
            /* Assume that the data size is an even multiple of the 128 bit
         * SSE vectors (i.e. 4 floats) */
        assert(!(SIZE & 0x3));

        /* TASK: Implement your simple matrix multiplication using SSE
         * here. (Multiply mat_a and mat_b into mat_c.)
         */
    /*
    __m128 vA = _mm_setzero_ps();
    __m128 vB = _mm_setzero_ps();
    __m128 vR = _mm_setzero_ps();
    
    float mul_int;
    */

    for (int i = 0; i <SIZE; i++){
        for (int k = 0; k<SIZE; k++){
            __m128 int_a = _mm_load_ps1(&mat_a[i][k]);
                for(int j = 0; j<SIZE; j+=4){
                    __m128 int_b = _mm_load_ps(&mat_b[k][j]);
                    __m128 int_mul = _mm_mul_ps(int_a,int_b);
                    __m128 int_c = _mm_load_ps(&mat_c[i][j]);
                    int_c = _mm_add_ps(int_c,int_mul);
                    _mm_store_ps(&mat_c[i][j], int_c);
                }
        }
}


}

#else

#error Invalid mode

#endif

/**
 * Reference implementation of the matrix multiply algorithm. Used to
 * verify the answer from matmul_opt. Do NOT change this function.
 */
static void matmul_ref()
{
        int i, j, k;

        for (i = 0; i < SIZE; i++) {
                for (k = 0; k < SIZE; k++) {
                        for (j = 0; j < SIZE; j++) {
                                mat_ref[i][j] += mat_a[i][k] * mat_b[k][j];
                        }
                }
        }
}

/**
 * Function used to verify the result. No need to change this one.
 */
static int verify_result()
{
        float e_sum;
        int i, j;

        e_sum = 0;
        for (i = 0; i < SIZE; i++) {
                for (j = 0; j < SIZE; j++) {
                        e_sum += mat_c[i][j] < mat_ref[i][j] ?
                                mat_ref[i][j] - mat_c[i][j] :
                                mat_c[i][j] - mat_ref[i][j];
                }
        }

        printf("e_sum: %.e\n", e_sum);

        return e_sum < 1E-6;
}

/**
 * Initialize mat_a and mat_b with "random" data. Write to every
 * element in mat_c to make sure that the kernel allocates physical
 * memory to every page in the matrix before we start doing
 * benchmarking.
 */
static void
init_matrices()
{
        int i, j;

        for (i = 0; i < SIZE; i++) {
                for (j = 0; j < SIZE; j++) {
                        mat_a[i][j] = ((i + j) & 0x0F) * 0x1P-4F;
                        mat_b[i][j] = (((i << 1) + (j >> 1)) & 0x0F) * 0x1P-4F;
                }
        }

        memset(mat_c, 0, sizeof(mat_c));
        memset(mat_ref, 0, sizeof(mat_ref));
}

static int
run_multiply()
{
        struct timespec ts_start, ts_stop;
        double runtime_ref, runtime_sse;

        printf("Starting optimized run...\n");
        util_monotonic_time(&ts_start);
        /* mat_c = mat_a * mat_b */
        matmul_sse();
        util_monotonic_time(&ts_stop);
        runtime_sse = util_time_diff(&ts_start, &ts_stop);
        printf("Optimized run completed in %.2f s\n",
               runtime_sse);

        printf("Starting reference run...\n");
        util_monotonic_time(&ts_start);
        matmul_ref();
        util_monotonic_time(&ts_stop);
        runtime_ref = util_time_diff(&ts_start, &ts_stop);
        printf("Reference run completed in %.2f s\n",
               runtime_ref);

        printf("Speedup: %.2f\n",
               runtime_ref / runtime_sse);


        if (verify_result()) {
                printf("OK\n");
                return 0;
        } else {
                printf("MISMATCH\n");
                return 1;
        }
}

int
main(int argc, char *argv[])
{
        /* Initialize the matrices with some "random" data. */
        init_matrices();

        int rc = run_multiply();
        if (rc) return 1;

        return 0;
}


/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * compile-command: "make -k"
 * End:
 */
