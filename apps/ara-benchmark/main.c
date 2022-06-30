#include <riscv_vector.h>   // keep this

#include <stdint.h>
#include <string.h>
#include "printf.h"

#include "rvv_reduce_n.h"

uint16_t prand() {
  uint16_t lfsr = 0xACE1u;
  uint16_t bit;
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr = (lfsr >> 1) | (bit << 15);
}

int main() {
    printf("\n\n//////////  vector length: 1  //////////\n");
    if (rvv_reduce_1() != 0) return 1;

    printf("\n\n//////////  vector length: 8  //////////\n");
    if (rvv_reduce_8() != 0) return 1;

    // TODO: ...

    printf("\n\nFinished rvv_reduce successfully for all vector lengths.\n");
    return 0;
}
