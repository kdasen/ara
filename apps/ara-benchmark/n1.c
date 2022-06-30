#include <riscv_vector.h>   // keep this

#include <stdint.h>
#include <string.h>

#include "printf.h"
#include "runtime.h"

#include "rvv_reduce_n.h"


#define N 1

int64_t A[N] __attribute__((aligned(32 * NR_LANES), section(".l2")));
int64_t B[N] __attribute__((aligned(32 * NR_LANES), section(".l2")));

#ifdef DEBUG
int64_t A[257] = {0, 1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 5, 1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 0, 1, 2, 3, 4, 0, 5, 6, 7, 8, 0, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 5, 1, 2, 3, 4, 0, 5, 6, 7, 8, 0, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 0, 1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 5, 1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 0, 1, 2, 3, 4, 0, 5, 6, 7, 8, 0, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 5, 1, 2, 3, 4, 0, 5, 6, 7, 8, 0, 10, 11, 12, 0, 13, 14, 0, 0, 0, 15, 16, 0, 17, 18, 19, 0, 20, 21, 22, 0, 17, 18, 19, 0, 20, 21, 22, 0, 1};
//int64_t A[N] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};

int64_t B[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 11, 24, 3, 4, 0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 341, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 11, 24, 3, 4, 0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 341, 4, 25, 26, 27, 28, 29, 30, 341, 2};
//int64_t B[N] = {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};
#endif

uint16_t lfsr = 0xACE1u;
uint16_t bit;

uint16_t prand() {
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr = (lfsr >> 1) | (bit << 15);
}

// accumulate and reduce
void reduce_scalar(int64_t *a, int64_t *b, int64_t *result_sum,
                     int *result_count, int n) {
  int count = 0;
  int64_t s = 0;
  for (int i = 0; i < n; ++i) {
    if (a[i] != 0) {
      s += a[i] * b[i];
      count++;
    }
  }

  *result_sum = s;
  *result_count = count;
}

void reduce_intrinsics(int64_t *a, int64_t *b, int64_t *result_sum, int *result_count,
              int n) {
  int count = 0;
  // set vlmax and initialize variables
  size_t vlmax = vsetvlmax_e64m1();
  vint64m1_t vec_zero = vmv_v_x_i64m1(0, vlmax);
  vint64m1_t vec_s = vmv_v_x_i64m1(0, vlmax);
  for (size_t vl; n > 0; n -= vl, a += vl, b += vl) {
    vl = vsetvl_e64m1(n);

    vint64m1_t vec_a = vle64_v_i64m1(a, vl);
    vint64m1_t vec_b = vle64_v_i64m1(b, vl);

    vbool64_t mask = vmsne_vv_i64m1_b64(vec_a, vec_zero, vl);

    vec_s = vmacc_vv_i64m1_m(mask, vec_s, vec_a, vec_b, vl);
    count = count + vpopc_m_b64(mask, vl);
  }
  vint64m1_t vec_sum;
  vec_sum = vredsum_vs_i64m1_i64m1(vec_zero, vec_s, vec_zero, vlmax);
  vse64_v_i64m1(result_sum, vec_sum, 1);

  *result_count = count;
}

void reduce_assembly(int64_t *a, int64_t *b, int64_t *result_sum, int *result_count,
              int n) {
  int count = 0;
  int temp;
  size_t vlmax, trash;
  // set vlmax and initialize variables
  asm volatile("vsetvli %0, %1, e64, m1, ta, ma" : "=r"(vlmax) : "r"(n));
  // vec zero
  asm volatile("vmv.v.i v30,  0");
  // vec s
  asm volatile("vmv.v.i v5,  0");

  for (size_t vl; n > 0; n -= vl, a += vl, b += vl) {
    asm volatile("vsetvli %0, %1, e64, m1, ta, ma" : "=r"(vl) : "r"(n));

    // vec a
    asm volatile("vle64.v v10, (%0);" ::"r"(a));
    // vec b
    asm volatile("vle64.v v11, (%0);" ::"r"(b));

    // mask
    asm volatile("vmsne.vv v0, v10, v30");

    asm volatile("vmacc.vv v5, v10, v11, v0.t");
    asm volatile("vpopc.m %0, v0" :"=r"(temp):);
    count += temp;
  }
  asm volatile("vsetvli %0, %1, e64, m1, ta, ma" : "=r"(trash) : "r"(vlmax));
  asm volatile("vredsum.vs v30, v5, v30");

  asm volatile("vsetvli %0, %1, e64, m1, ta, ma" : "=r"(temp) : "r"(1));
  asm volatile("vse64.v v30, (%0);" ::"r"(result_sum));

  *result_count = count;
}

int rvv_reduce_1() {

#ifndef DEBUG
  // Initialize the vectors
  for (int i = 0; i < N; ++i) {
    A[i] = (i%3) ? (int64_t) prand() : 0;
    B[i] = (i%7) ? (int64_t) prand() : 0;
  }
#endif

  // Compute
  int64_t scalar_sum, intrinsics_sum, assembly_sum;
  int32_t scalar_count, intrinsics_count, assembly_count;

  // Reduce Scalar
  printf("Reduction in scalar processor...\n");
  start_timer();
  reduce_scalar(A, B, &scalar_sum, &scalar_count, N);
  stop_timer();
  // Scalar metrics Metrics
  int64_t runtime = get_timer();
  printf("The execution in the scalar processor took %d cycles.\n", runtime);

  reduce_intrinsics(A, B, &intrinsics_sum, &intrinsics_count, N);

  // Reduce in Ara, written in assembly
  printf("Reduction in Ara (assembly)...\n");
  start_timer();
  reduce_assembly(A, B, &assembly_sum, &assembly_count, N);
  stop_timer();
  // Ara Metrics
  runtime = get_timer();
  printf("The execution in Ara (assembly) took %d cycles.\n\n", runtime);

  printf("Sums:\nScalar:     %d\nIntrinsics: %d\nAssembly:   %d\n\n", scalar_sum, intrinsics_sum, assembly_sum);
  printf("Counts:\nScalar:     %d\nIntrinsics: %d\nAssembly:   %d\n", scalar_count, intrinsics_count, assembly_count);

  // compare
  if(scalar_sum == intrinsics_sum && scalar_count == intrinsics_count && scalar_sum == assembly_sum && scalar_count == assembly_count) {
    printf("\n\n\nPASS\n\n\n");
    return 0;
  }
  else {
    printf("\n\n\nFAIL\n\n\n");
    return 1;
  }
}
