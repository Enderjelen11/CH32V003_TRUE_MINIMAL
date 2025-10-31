PREFIX   = riscv64-unknown-elf
CC       = $(PREFIX)-gcc
OC       = $(PREFIX)-objcopy
SZ       = $(PREFIX)-size

MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

LINKER   = $(MAKEFILE_DIR)/Link.ld

FLAGS = -Os -march=rv32ec -mabi=ilp32e -nostdlib -ffreestanding -Wall -ffunction-sections -Wl,-Map,$(TARGET).map,-T$(LINKER)

SRC += $(MAKEFILE_DIR)/startup.S

all: clean build flash

build: $(TARGET).bin
	@echo
	@$(SZ) --format=berkeley $(TARGET).elf
	@echo
	@$(eval FLASH_USED=$(shell $(SZ) -A $(TARGET).elf | awk '/\.text|\.data/{sum+=$$2} END{print sum}'))
	@$(eval RAM_USED=$(shell $(SZ) -A $(TARGET).elf | awk '/\.data|\.bss/{sum+=$$2} END{print sum}'))
	@echo "Flash: $(FLASH_USED) bytes (≈$$(echo $$(($(FLASH_USED)*100/16384)))%)"
	@echo "RAM:   $(RAM_USED) bytes (≈$$(echo $$(($(RAM_USED)*100/2048)))%)"
	@echo

$(TARGET).elf: $(SRC)
	$(CC) $(FLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
	$(OC) -O binary $< $@

flash: $(TARGET).bin
	wlink flash $(TARGET).bin

clean:
	$(RM) *.elf *.bin *.map
