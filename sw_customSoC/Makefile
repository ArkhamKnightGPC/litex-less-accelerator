BUILD_DIR = ../hw_customSoC/build/sim
SOC_DIRECTORY = ../litex/litex/soc

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

# --- LESS PARAMETERS ---
# We do not define CATEGORY yet to use toy parameters during early development/tests
# -----------------------

CFLAGS += -g -O3 -I./include
CODE_SRC := $(wildcard lib/*.c)
CODE_OBJ := $(CODE_SRC:.c=.o)

TARGET  := program
OBJECTS := main.o isr.o $(CODE_OBJ)

all: $(TARGET).bin

-include $(OBJECTS:.o=.d)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) \
		-T linker.ld \
		-N \
		-o $@ \
		$(BUILD_DIR)/software/bios/crt0.o \
		$(OBJECTS) \
		-L$(BUILD_DIR)/software/libbase \
		-L$(BUILD_DIR)/software/libc \
		-L$(BUILD_DIR)/software/libcompiler_rt \
		-L$(BUILD_DIR)/software/lgcc \
		-lc -lbase -lcompiler_rt -lgcc

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) \
	      $(TARGET).elf $(TARGET).bin \
	      .*~ *~

.PHONY: all clean load
