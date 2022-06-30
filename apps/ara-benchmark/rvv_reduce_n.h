#include <stdint.h>

int64_t A[N] __attribute__((aligned(32 * NR_LANES), section(".l2")));
int64_t B[N] __attribute__((aligned(32 * NR_LANES), section(".l2")));

uint16_t prand();

// int rvv_reduce_n(): one function, one file for each vector length
int rvv_reduce_1();
int rvv_reduce_8();
// int rvv_reduce_16();
// int rvv_reduce_32();
// int rvv_reduce_50();
// int rvv_reduce_64();
// int rvv_reduce_100();
// int rvv_reduce_128();
// int rvv_reduce_200();
// int rvv_reduce_256();
// int rvv_reduce_400();
