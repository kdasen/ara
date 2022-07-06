// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matheus Cavalcante <matheusd@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "vector_macros.h"

//////////////////
//  Benchmarks  //
//////////////////

uint64_t paradiddle_64    = 0b0100101101001011010010110100101101001011010010110100101101001011;
uint64_t paradiddle_inv64 = 0b1011010010110100101101001011010010110100101101001011010010110100;
uint64_t double_64        = 0b0011001100110011001100110011001100110011001100110011001100110011;

// 32 bits stored in EW8 encoding, unmasked
void BENCHMARK32_4e8(void) {
  VSET(4, e8, m1);
  VLOAD_8(v8, 0b01001011, 0b10110100, 0b00110011, 0b11001101);
  VSET(32, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v8 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 17);
}

// 32 bits stored in EW8 encoding, masked
void BENCHMARK32m_4e8(void) {
  VSET(4, e8, m1);
  VLOAD_8(v8, 0b01001011, 0b10110100, 0b00110011, 0b11001101);
  VLOAD_8(v0, 0x00, 0x00, 0xff, 0xff);
  VSET(32, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v8, v0.t \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 9);
}

// 256 bits stored in EW64 encoding, unmasked
void BENCHMARK256_4e64(void) {
  VSET(4, e64, m4);
  VLOAD_64(v8, paradiddle_64, paradiddle_inv64, double_64, 0x0f0f0f0f0f0f0f0e);
  VSET(256, e64, m4);   // m4 is needed here since vlmax=64 for e64
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v8 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 127);
}

// 256 bits stored in EW64 encoding, masked
void BENCHMARK256m_4e64(void) {
  VSET(4, e64, m4);
  VLOAD_64(v8, paradiddle_64, paradiddle_inv64, double_64, 0x0f0f0f0f0f0f0f0e);
  VLOAD_64(v0, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff);
  VSET(256, e64, m4);   // m4 is needed here since vlmax=64 for e64
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v8, v0.t \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 63);
}

// 256 bits stored in EW8 encoding, unmasked
void BENCHMARK256_32e8(void) {
  VSET(32, e8, m1);
  VLOAD_8(v2, 0x00, 0xff, 0x00, 0xff, 0x0f, 0xf0, 0x0f, 0xf0, 
              0xc3, 0x3c, 0xc3, 0x3c, 0xaa, 0xaa, 0x55, 0x55,
              0x00, 0xff, 0x00, 0xff, 0x0f, 0xf0, 0x0f, 0xf0,
              0xc3, 0x3c, 0xc3, 0x3c, 0xaa, 0xaa, 0x55, 0x5f);
  VSET(256, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 130);
}

// 256 bits stored in EW8 encoding, masked
void BENCHMARK256m_32e8(void) {
  VSET(32, e8, m1);
  VLOAD_8(v2, 0x00, 0xff, 0x00, 0xff, 0x0f, 0xf0, 0x0f, 0xf0, 
              0xc3, 0x3c, 0xc3, 0x3c, 0xaa, 0xaa, 0x55, 0x55,
              0x00, 0xff, 0x00, 0xff, 0x0f, 0xf0, 0x0f, 0xf0,
              0xc3, 0x3c, 0xc3, 0x3c, 0xaa, 0xaa, 0x55, 0x5f);
  VLOAD_8(v0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
              0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
  VSET(256, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2, v0.t \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 66);
}


//////////////////
//  Unit Tests  //
//////////////////

// masked
void TEST_CASE1(void) {
  VSET(4, e32, m1);
  VLOAD_32(v2, 7, 0, 0, 0);
  VLOAD_32(v0, 5, 0, 0, 0);
  // vcpop interprets the vl as bits, not as e32 elements
  VSET(128, e32, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2, v0.t \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 2);
}

// unmasked
void TEST_CASE2(void) {
  VSET(4, e32, m1);
  VLOAD_32(v2, 9, 7, 5, 3);
  VSET(128, e32, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 9);
}

// more elements, smaller elements, masked
void TEST_CASE3(void) {
  VSET(6, e8, m1);
  VLOAD_8(v2, 0xf, 0x0, 0xf, 0x0, 0x3, 0x0);
  VLOAD_8(v0, 0x1, 0x0, 0x0, 0x0, 0x7, 0x0);
  VSET(48, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2, v0.t \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 3);
}

// unmasked
void TEST_CASE4(void) {
  VSET(6, e8, m1);
  VLOAD_8(v2, 0xf, 0x0, 0xf, 0x0, 0x3, 0x0);
  VSET(48, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 10);
}

// no active elements
void TEST_CASE5(void) {
  VSET(2, e32, m1);
  VLOAD_32(v2, 0, 0);
  VSET(64, e32, m1);
  volatile uint32_t scalar = 1234;
  volatile uint32_t OUP[] = {0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 0);
}

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();

  BENCHMARK32_4e8();
  BENCHMARK32m_4e8();
  BENCHMARK256_4e64();
  BENCHMARK256m_4e64();
  BENCHMARK256_32e8();
  BENCHMARK256m_32e8();

  // TEST_CASE1();
  // TEST_CASE2();
  // TEST_CASE3();
  // TEST_CASE4();
  // TEST_CASE5();
  EXIT_CHECK();
}
