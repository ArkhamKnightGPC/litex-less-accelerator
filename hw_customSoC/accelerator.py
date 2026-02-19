from migen import *
from litex.soc.interconnect.csr import *

class Accelerator(Module, AutoCSR):
    def __init__(self):
        # Registrador que o software escreve
        self.value = CSRStorage(32)

        # Registrador que o software lê
        self.result = CSRStatus(32)

        # Lógica simples: result = value + 1
        self.sync += self.result.status.eq(self.value.storage + 1)
