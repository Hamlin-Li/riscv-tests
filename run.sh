#!/bin/bash

# riscv64-linux-gnu-as -march=rv64gcv -o add.o add.s
# riscv64-linux-gnu-gcc -static -o main main.cc add.o
# riscv64-linux-gnu-gcc -march=rv64gv -O2 -static -o main c_wrapper.c riscv-vclmul.s
# gcc -march=rv64gv_zvbc -O2 -static -o main c_wrapper.c riscv-vclmul.s
# gcc -march=rv64gv_zvbc_zicboz_zbb -O3 -static -o main c_wrapper.c riscv-zeroing.s riscv-vclmul.s riscv-vse8.s misc.s
# gcc -march=rv64gv_zvbc_zicboz_zbb -O3 -static -o main c_wrapper.c riscv-zeroing.s # riscv-vclmul.s riscv-vse8.s misc.s
gcc -march=rv64gv_zvbc_zicboz_zbb -O3 -static -o main c_wrapper.c vset+ops.c riscv-vset+ops.s cbo-zero.c cache-tests.c riscv-zeroing.s riscv-vclmul.s riscv-vse8.s misc.c misc.s

	./main

