# Hardware accelerator for the LESS signature scheme

This repository contains the development of a loosely-coupled hardware accelerator for **LESS - Linear Equivalence Signature Scheme**, a code-based digital signature scheme currently under evaluation in the NIST Post-Quantum Cryptography (PQC) standardization process.

The project goal is to use hardware/software co-design techniques to speed-up the main computational bottlenecks of the LESS algorithm: the matrix Gaussian elimination process. The matrix row reduction (RREF) function accounts for over **60%** of the execution time during *Sign* and *Verify* in Valgrind profiling.

The table below shows profiling results for a trial run of the LESS *Sign* function with Valgrind.

| function | self-time | total-time | number of calls |
| :--- | :---: | :---: | :---: |
| `generator_RREF_pivot_reuse` | 61,94% | 61,94% | 45 |
| `compute_canonical_form_type4` | 7,20% | 19,77% | 196 |
| `KeccakF1600_StatePermute` | 5,67% | 5,67% | 6040 |
| `matrix_transpose_opt` | 5,01% | 5,01% | 388 |



## Table of contents
- [Environment setup](#environment-setup)
    - [Experimentation with litex_sim](#experimentation-with-litex_sim)
- [Simulation with custom python SoC](#simulation-with-custom-python-soc)
    - [Debugging with gdb](#debugging-with-gdb)

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

## Simulation with custom python SoC

In order to integrate our accelerator, we need to explicitly define a custom SoC using Migen and the Litex package:
- the code for the custom SoC HW is in the **hw_customSoC** folder;
- and the modified SW implementation is in the **sw_customSoC** folder.

Please refer to these folders for more details. First, we first need to generate the **hw_customSoC/build** folder. To do this, run:
```
cd hw_customSoC/
python3 main.py --first_build --sim
```

The *first_build* option will skip the section of the code where we load our custom firmware (which is not compiled at the time of the first build). The *sim* option selects the simulation platform instead of a board specific platform, meant for some specific target FPGA (to come...).

So, to compile **sw_customSoC** run:

```
cd sw_customSoC/
make clean
make
```

and now we can re-build and simulate on our custom SoC:

```
cd hw_customSoC/
python3 main.py --sim
```

### Debugging with gdb

After the proper build, you can launch the simulation directly:
```
cd build/sim/gateware
./obj_dir/Vsim
```

and you can also debug this simulation executable with gdb:
```
gdb ./obj_dir/Vsim
run
bt
```
