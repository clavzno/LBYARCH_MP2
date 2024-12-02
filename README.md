# LBYARCH_MP2
MP GRP 13 LBYARCH S15
CLAVANO, ANGELICA THERESE IGNACIO (Jack)
ALEJANDRINO, VINCE ALEJSON VIERNES

sdot = \sum_{i=1}^{n} a_i b_i = a_1*b_1 + a_2*b_2 + ... + a_n*b_n

## NOTE
latest code is in branch w/-c

## How to run - CLEAN
nasm -f elf64 -o dot_product.o dot_product.asm
gcc -std=c99 -c -o main.o main.c
- -std=c99: uses c99 standard
gcc -std=c99 -o main.exe main.o dot_product.o -lm
- -lm: links math library to the executable
main.exe

## How to run - DEBUG
nasm -f elf64 -o dot_product.o dot_product.asm
gcc -std=c99 -g -O0 -c -o main.o main.c
- O0: no optimization
gcc -std=c99 -g -O0 -o main_debug.exe main.o dot_product.o -lm
./main_debug.exe

## How to run - RELEASE
nasm -f elf64 -o dot_product.o dot_product.asm
gcc -std=c99 -O2 -s -c -o main.o main.c
- O2: optimization level 2, high optimization
gcc -std=c99 -O2 -s -o main_release.exe main.o dot_product.o -lm
./main_release.exe

## Commands that don't work
nasm -f m64 dot_product.o dot_product.asm
gcc -c main.c -o main.obj -m64
gcc -c -o main.exe main.o dot_product.o -lm

## Comparative Execution time and short analysis of the performance of the kernels

## Screenshot of the program output and correctness check (C)

## Screenshot of the program output and correctness check (x86-64)

## Short videos (5-10 minutes) showing the source code, compilation, execution of the C and x86-64 program
