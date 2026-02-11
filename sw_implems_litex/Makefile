BUILD_DIR = ../build/sim
SOC_DIRECTORY = ../litex/litex/soc

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

TARGET  := firmware
OBJECTS := main.o isr.o

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
		-lc -lbase -lcompiler_rt

%.o: %.c
	$(compile)

%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) \
	      $(TARGET).elf $(TARGET).bin \
	      .*~ *~

.PHONY: all clean load
