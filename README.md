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
litex_sim --cpu-type=vexriscv
```

## Verilator simulation with custom C firmware on Litex

First we compile our C code to run as firmware on bare metal
```
cd sw_implems_litex/
make
```

now load the **firmware.bin** binary to the Verilator simulation on launch

```
litex_sim --cpu-type=vexriscv --trace --integrated-rom-init sw_implems_litex/firmware.bin
```

finally, the trace option generated a GTKWave file we can use to visualize waveforms

```
cd build/sim/gateware/
gtkwave sim.gtkw
```

---


**OBS**: if you have issues with *gtkwave* on VSCode, check [this](https://askubuntu.com/questions/1462295/ubuntu-22-04-both-eye-of-gnome-and-gimp-failing-with-undefined-symbol-error/1462494#1462494).
