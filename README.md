# Hardware accelerator for the LESS signature scheme

This repository contains the development of a loosely-coupled hardware accelerator for **LESS - Linear Equivalence Signature Scheme**, a code-based digital signature algorithm currently under evaluation in the NIST Post-Quantum Cryptography (PQC) standardization process.

The project goal is to use hardware/software co-design techniques to address the computational bottlenecks of the LESS algorithm, specifically targeting the matrix Gaussian elimination process. The matrix row reduction (RREF) function accounts for over **60%** of the execution time in Valgrind profiling.

## Table of contents
    - [Environment setup](#environment-setup)
        - [Experimentation with litex_sim](#experimentation-with-litex_sim)
    - [Custom python SoC](#custom-python-soc)

---

## Environment setup

First, we install the RISC-V toolchain

```
chmod +x install_riscv_toolchain.sh
source install_riscv_toolchain.sh
```

then we create a python venv and build our base LiteX environment

```
python3 -m venv venv
source venv/bin/activate
sudo apt install gcc-riscv64-unknown-elf
source litex_setup.sh
```

and finally, we can run Verilator simulations of Litex SoCs.

```
sudo apt install libevent-dev libjson-c-dev verilator
litex_sim --cpu-type=vexriscv --integrated-main-ram-size=0x40000 --integrated-sram-size=0x40000
```

We explicitly define memory sizes because before compilation of our custom C firmware it's necessary to generate *build/* with memory regions that are big enough for the linker to allocate our code.

### Experimentation with litex_sim

First we compile our C code to a binary **program.bin**

```
cd sw_litexsim/
make clean
make
```

now we launch *litex_sim* initializing RAM with our compiled binary. The *--ram-init* option ensures that our custom firmware is available at boot, and the CPU will execute it immediately.

```
litex_sim --cpu-type=vexriscv --integrated-main-ram-size=0x40000 --integrated-sram-size=0x40000 --ram-init=sw_litexsim/program.bin
```

**OBS:** with the *--trace* option, simulation generates a GTKWave file for visualization:

```
cd build/sim/gateware/
gtkwave sim.gtkw
```

If you have issues with *gtkwave* on VSCode, check [this](https://askubuntu.com/questions/1462295/ubuntu-22-04-both-eye-of-gnome-and-gimp-failing-with-undefined-symbol-error/1462494#1462494).

---

## Custom python SoC

In order to integrate our accelerator, we need to explicitly define a custom SoC using Migen and the Litex package:
- the code for the custom SoC HW is in the **hw_customSoC** folder;
- and the modified SW implementation is in the **sw_customSoC** folder.

Please refer to these folders for my detailed design information. Now, we first need to generate the **hw_customSoC/build** folder. To do this, run:
```
cd hw_customSoC/
python3 main.py --build_only --sim
```

The *build_only* option will skip the section of the code where we load our custom firmware (which is not compiled at the time of the first run) and does not run simulation. So, to compile **sw_customSoC** run:

```
cd sw_customSoC/
make clean
make
```

and now we can simulate on our custom SoC:

```
cd hw_customSoC/
python3 customCore.py --sim
```
