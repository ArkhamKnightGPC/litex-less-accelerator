from migen import *

from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder

from litex.build.sim import SimPlatform
from litex.build.sim.config import SimConfig

from platform_io import load_io_from_json
from accelerator import Accelerator

class customCore(SoCCore):
    def __init__(
            self,
            sys_clk_freq,
        ):

        platform = SimPlatform(
            device = "SIM",
            io = load_io_from_json("io_sim.json")
        )

        SoCCore.__init__(self,
            platform,
            # CPU specs
            cpu_type="vexriscv",
            bus_data_width=32,
            clk_freq=sys_clk_freq,
            # Memory specs
            integrated_rom_size=0x8000,
            integrated_main_ram_size=0x40000,
            integrated_sram_size=0x40000,
            # UART
            with_uart = False,
            uart_name = "sim",
        )

        self.clock_domains.cd_sys = ClockDomain()
        self.comb += [
            self.cd_sys.clk.eq(platform.request("sys_clk")),
            self.cd_sys.rst.eq(platform.request("cpu_reset"))
        ]
        self.add_uart(
            name="uart",
            uart_name="sim",
            uart_pads=self.platform.request("serial"),
        )
        self.submodules.mock = Accelerator()
        self.add_csr("accelerator")

def main():
    sys_clk_freq = int(1e6)

    soc = customCore(sys_clk_freq)
    builder = Builder(soc)

    sim_config = SimConfig()
    sim_config.add_clocker("sys_clk", freq_hz=sys_clk_freq)
    sim_config.add_module("serial2console", "serial")

    builder.build(
        run=True,
        sim_config=sim_config
    )

if __name__ == "__main__":
    main()
