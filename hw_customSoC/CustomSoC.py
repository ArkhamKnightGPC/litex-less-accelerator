from migen import *

from litex.soc.cores.uart import UARTWishboneBridge
from litex.soc.cores.timer import Timer
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.common import get_mem_data
from litex.soc.interconnect import wishbone
from litex.build.generic_platform import GenericPlatform
from litex.build.sim import SimPlatform

from CommProtocol import CommProtocol

class CustomSoC(SoCCore):
    def __init__(
        self,
        platform: GenericPlatform,
        sys_clk_freq: int,
        comm_protocol: CommProtocol,
        integrated_rom_path: str = None,
    ):
        # Define some base attributes that are useful
        self.bus_data_width = 32
        self.platform = platform
        self.is_simulated = isinstance(self.platform, SimPlatform)
        self.sys_clk_freq = sys_clk_freq
        self.comm_protocol = comm_protocol
        self.setup_buffer_allocator()

        # write compiled custom SW to ROM
        integrated_rom_data = (
            get_mem_data(
                integrated_rom_path, data_width=self.bus_data_width, endianness="little"
            )
            if integrated_rom_path
            else []
        )

        print("ROM words:", len(integrated_rom_data))

        SoCCore.__init__(
            self,
            platform=platform,
            # CPU
            cpu_type="vexriscv",
            cpu_variant="minimal",
            bus_data_width=self.bus_data_width,
            clk_freq=sys_clk_freq,
            # Memory
            integrated_rom_size=0x80000,
            integrated_main_ram_size=0x40000,
            integrated_sram_size=0x40000,
            integrated_rom_init=integrated_rom_data,
            # UART
            with_uart=False,
        )

        print("ROM base:", hex(self.mem_map["rom"]))

        self.add_io()
        timer0 = Timer(width=32)
        timer0.add_uptime()
        self.submodules += timer0
        self.add_csr("timer0")

        # Clock domain
        self.clock_domains.cd_sys = ClockDomain()
        self.comb += [
            self.cd_sys.clk.eq(platform.request("sys_clk")),
            self.cd_sys.rst.eq(platform.request("cpu_reset"))
        ]

    def setup_buffer_allocator(self):
        # Simple IO memory bump-allocator
        self._io_base = 0x4100_0000  # start of your IO window
        self._io_limit = 0x4200_0000  # optional safety limit (64 KiB here)
        self._io_cur = self._io_base

    def add_io(self):
        # If we are simulating, we can have a UART for interacting with the terminal
        if self.is_simulated:
            if self.comm_protocol == CommProtocol.UART:
                # NOTE: needs to be named uart so we can use Litex stdio
                self.add_uart(
                    name="uart",
                    uart_name="sim",
                    uart_pads=self.platform.request("serial"),
                )
        else:
            raise RuntimeError()
