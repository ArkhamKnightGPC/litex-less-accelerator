import argparse

from litex.soc.integration.builder import Builder
from litex.build.sim.config import SimConfig

from CommProtocol import CommProtocol

from CustomPlatform import CustomPlatform
from CustomSoC import CustomSoC

def main():
    parser = argparse.ArgumentParser(description="Build or simulate Custom SoC")
    parser.add_argument("--build_only", action="store_true", help="Perform only build, not simulation")
    parser.add_argument("--sim", action="store_true", help="Define platform as sim or FPGA")
    args = parser.parse_args()

    # hardcoded args (TODO: pass all args via argparse)
    sys_clk_freq = int(1e6)
    comm = CommProtocol.UART
    integrated_rom_path = None if args.build_only else "../sw_customSoC/program.bin"

    # TODO: add platform option for the target FPGA
    platform = CustomPlatform() if args.sim else None

    soc = CustomSoC(
        platform=platform,
        sys_clk_freq=sys_clk_freq,
        comm_protocol=comm,
        integrated_rom_path=integrated_rom_path,
    )

    builder = Builder(
        soc=soc,
    )

    if args.sim:
        sim_config = SimConfig()
        sim_config.add_clocker(
            clk="sys_clk",
            freq_hz=sys_clk_freq
        )
        sim_config.add_module("serial2console", "serial_term")

        builder.build(
            sim_config=sim_config,
            run=not args.build_only,
            # Verilator optimizations
            threads=8,
            jobs=8,
            opt_level="O3",
            interactive=True,
            coverage=False,
            video=False,
        )
    else:
        raise RuntimeError()

if __name__ == "__main__":
    main()
