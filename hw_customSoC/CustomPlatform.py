from litex.build.sim import SimPlatform
from platform_io import load_io_from_json

class CustomPlatform(SimPlatform):
    def __init__(self):
        SimPlatform.__init__(
            self,
            device="SIM",
            io=load_io_from_json("io_sim.json")
        )