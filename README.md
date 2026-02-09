# Hardware accelerator for the LESS signature scheme

This repository contains the development of a hardware accelerator for the **Linear Equivalence Signature Scheme (LESS)**, a code-based digital signature algorithm currently under evaluation in the NIST Post-Quantum Cryptography (PQC) standardization process.

The project goal is to use hardware/software co-design techniques to address the computational bottlenecks of the LESS algorithm, specifically targeting the matrix Gaussian elimination process. The matrix row reduction (RREF) function accounts for over **60%** of the execution time in Valgrind profiling.

---

## Setup

Build base LiteX environment:

´´´
sudo apt install gcc-riscv64-unknown-elf
source litex_setup.sh
´´´

Setup Verilator:
´´´
sudo apt install libevent-dev libjson-c-dev verilator
litex_sim --cpu-type=vexriscv
´´´
