# Hardware accelerator for the LESS signature scheme

This repository contains the development of a hardware accelerator for the **Linear Equivalence Signature Scheme (LESS)**, a code-based digital signature algorithm currently under evaluation in the NIST Post-Quantum Cryptography (PQC) standardization process.

The project goal is to use hardware/software co-design techniques to address the computational bottlenecks of the LESS algorithm, specifically targeting the matrix Gaussian elimination process. The matrix row reduction (RREF) function accounts for over **60%** of the execution time in Valgrind profiling.

---

## Environment setup

First, install RISC-V toolchain

```
chmod +x install_riscv_toolchain.sh
source install_riscv_toolchain.sh
```

then we create a python venv and build our base LiteX environment

```
python -m venv venv
source venv/bin/activate
sudo apt install gcc-riscv64-unknown-elf
source litex_setup.sh
```

and finally, run a Verilator simulation of the Litex SoC.

```
sudo apt install libevent-dev libjson-c-dev verilator
litex_sim --cpu-type=vexriscv --integrated-main-ram-size=0x40000 --integrated-sram-size=0x40000
```

We explicitly define memory sizes because before compilation of our custom C firmware it's necessary to generate *build/* with memory regions that are big enough for the linker to allocate our code.

## Simulating code execution in Verilator

First we compile our C code to a binary **program.bin**

```
cd sw_implems_litex/
make
```

now we launch *litex_sim* initializing RAM with our compiled binary. The *--ram-init* option ensures that our custom firmware is available at boot, and the CPU will execute it immediately.

```
litex_sim --cpu-type=vexriscv --integrated-main-ram-size=0x40000 --integrated-sram-size=0x40000 --ram-init=sw_implems_litex/program.bin
```

finally, it's possible to simulate with the *--trace* option to generate a GTKWave file we can use to visualize waveforms

```
cd build/sim/gateware/
gtkwave sim.gtkw
```

---

**OBS**: if you have issues with *gtkwave* on VSCode, check [this](https://askubuntu.com/questions/1462295/ubuntu-22-04-both-eye-of-gnome-and-gimp-failing-with-undefined-symbol-error/1462494#1462494).
