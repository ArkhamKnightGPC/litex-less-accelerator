from migen import *
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder
from litex.build.sim import SimPlatform
from litex.build.sim.config import SimConfig

from accelerator import Accelerator

class MySimSoC(SoCCore):
    def __init__(self):
        platform = SimPlatform("SIM", [])

        sys_clk_freq = int(1e6)

        SoCCore.__init__(self,
            platform,
            sys_clk_freq,
            cpu_type="vexriscv",
            integrated_rom_size=0x8000,
            integrated_main_ram_size=0x40000,
            integrated_sram_size=0x40000,
            with_uart=False,
        )

        self.submodules.mock = Accelerator()
        self.add_csr("accelerator")

def main():
    soc = MySimSoC()
    builder = Builder(soc)
    builder.build(
        run=True,
        sim_config=SimConfig(default_clk="sys_clk")
    )

if __name__ == "__main__":
    main()
