PREFIX   = riscv64-unknown-elf
CC       = $(PREFIX)-gcc
OC       = $(PREFIX)-objcopy
SZ       = $(PREFIX)-size

PROJECT_DIR := $(CURDIR)

MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

LINKER = $(MAKEFILE_DIR)/Link.ld

BUILD_DIR = $(PROJECT_DIR)/build

$(shell mkdir -p $(BUILD_DIR))

ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = $(BUILD_DIR)/$(TARGET).bin
MAP = $(BUILD_DIR)/$(TARGET).map

FLAGS = -Os -march=rv32ec_zicsr_zifencei -mabi=ilp32e -nostdlib -ffreestanding -Wall -ffunction-sections \
        -Wl,-Map,$(MAP),-T$(LINKER)

SRC += $(MAKEFILE_DIR)/startup.S

all: clean build flash

build: $(BIN)
	@echo
	@$(SZ) --format=berkeley $(ELF)
	@echo
	@$(eval FLASH_USED=$(shell $(SZ) -A $(ELF) | awk '/\.text|\.data/{sum+=$$2} END{print sum}'))
	@$(eval RAM_USED=$(shell $(SZ) -A $(ELF) | awk '/\.data|\.bss/{sum+=$$2} END{print sum}'))
	@echo "Flash: $(FLASH_USED) bytes (≈$$(echo $$(($(FLASH_USED)*100/16384)))%)"
	@echo "RAM:   $(RAM_USED) bytes (≈$$(echo $$(($(RAM_USED)*100/2048)))%)"
	@echo

$(ELF): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) -o $@ $^

$(BIN): $(ELF)
	$(OC) -O binary $< $@

flash: $(BIN)
	wlink flash $(BIN)

clean:
	$(RM) -r $(BUILD_DIR)

