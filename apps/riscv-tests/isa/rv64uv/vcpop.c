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

uint32_t paradiddle_32    = 0b01001011010010110100101101001011;
uint32_t paradiddle_inv32 = 0b10110100101101001011010010110100;

uint64_t paradiddle_64    = 0b0100101101001011010010110100101101001011010010110100101101001011;
uint64_t paradiddle_inv64 = 0b1011010010110100101101001011010010110100101101001011010010110100;
uint64_t double_64        = 0b0011001100110011001100110011001100110011001100110011001100110011;

void BENCHMARK1_1e32(void) {
  VSET(1, e32, m1);
  VLOAD_32(v2, 0x0001);
  VSET(1, e32, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 1);
}

void BENCHMARK8_1e8(void) {
  VSET(1, e8, m1);
  VLOAD_8(v2, 0b01001011);
  VSET(8, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 4);
}

void BENCHMARK32_4e8(void) {
  VSET(4, e8, m1);
  VLOAD_8(v2, 0b01001011, 0b10110100, 0b00110011, 0b11001101);
  VSET(32, e8, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 17);
}

void BENCHMARK32_1e32(void) {
  VSET(1, e32, m1);
  VLOAD_32(v2, paradiddle_32);
  VSET(32, e32, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 16);
}

void BENCHMARK64_1e64(void) {
  VSET(1, e64, m1);
  VLOAD_64(v2, paradiddle_64);
  VSET(64, e64, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 32);
}

void BENCHMARK128_8e16(void) {
  VSET(8, e16, m1);
  VLOAD_16(v2, 0b0100101101001011, 0b1011010010110100, 0b0011001100110011, 0b1100110011001100,
               0b0101010101010101, 0b1010101010101010, 0b0000000011111111, 0b1111111100000001);
  VSET(128, e16, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 65);
}

// impossible, too many elements for LMUL=1
void BENCHMARK256_4e64(void) {
  VSET(4, e64, m4);
  VLOAD_64(v2, paradiddle_64, paradiddle_inv64, double_64, 0x0f0f0f0f0f0f0f0e);
  VSET(256, e64, m4);   // m4 is needed here since vlmax=64 for e64
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 127);
}

// the largest EW that is supported for 256 elements
void BENCHMARK256_16e16(void) {
  VSET(16, e16, m1);
  VLOAD_16(v2, 0x0000, 0xffff, 0x0000, 0xffff, 
               0x00ff, 0x00ff, 0xff00, 0xff00,
               0x0f0f, 0x0f0f, 0xf0f0, 0xf0f0,
               0xaaaa, 0xaaaa, 0x5555, 0x5557);
  VSET(256, e16, m1);
  volatile uint32_t scalar = 1337;
  volatile uint32_t OUP[] = {0, 0, 0, 0};
  __asm__ volatile("vpopc.m %[A], v2 \n"
                   "sw %[A], (%1) \n"
                   :
                   : [A] "r"(scalar), "r"(OUP));
  XCMP(1, OUP[0], 129);
}


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

  BENCHMARK1_1e32();
  BENCHMARK8_1e8();
  BENCHMARK32_4e8();
  BENCHMARK32_1e32();
  BENCHMARK64_1e64();
  BENCHMARK128_8e16();
  BENCHMARK256_16e16();
  // BENCHMARK256_4e64();
  BENCHMARK256_32e8();

  // TEST_CASE1();
  // TEST_CASE2();
  // TEST_CASE3();
  // TEST_CASE4();
  // TEST_CASE5();
  EXIT_CHECK();
}
