#
# Copyright (c) 2015, Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
#
# Author: Frank Wang <frank.wang@rock-chips.com>
# Date  : 15-Sep-2015
#
# This makefile is determined to compile CoOS code in the Linux environment.
#
# GCC cross compiation tool: gcc-arm-none-eabi-4_8-2014q3
#

ifneq ($(wildcard .config),)
include .config
endif

#
# Default values for build configurations
#

# Build verbosity
V			:= 0
# Debug build
DEBUG			:= 1
# Cross Compile
CROSS_COMPILE := ../prebuilts/gcc-arm-none-eabi-4_8-2014q3/bin/arm-none-eabi-
# Build architecture
ARCH 			?= cortex-m3
# Build platform
PLATFORMS		:= "Rk3368Mcu,Rk3366Mcu"
DEFAULT_PLAT		:= Rk3368Mcu
PLAT			?= ${DEFAULT_PLAT}
HELP_PLATFORMS	:= $(shell echo ${PLATFORMS} | sed 's/ /|/g')

# Build ARCH
ifeq (${PLAT},Rk3368Mcu)
ARCH 			:= cortex-m3
LOCAL_CFLAGS		+= -DRK3368 -DCPU_FREQ=96
endif

ifeq (${PLAT},Rk3366Mcu)
ARCH 			:= cortex-m3
LOCAL_CFLAGS		+= -DRK3366 -DCPU_FREQ=96
endif


# Flag used to indicate if ASM_ASSERTION should be enabled for the build.
# This defaults to being present in DEBUG builds only.
ASM_ASSERTION		:= ${DEBUG}

ifeq (${V},0)
	Q=@
	CHECKCODE_ARGS	+= --no-summary --terse
else
	Q=
endif
export Q

ifneq (${DEBUG}, 0)
	BUILD_TYPE	:= Debug
else
	BUILD_TYPE	:= Release
endif

BUILD_BASE		:= ./Project
BUILD_PLAT		:= ${BUILD_BASE}/${PLAT}/${BUILD_TYPE}

VER_BIN_DIR		:= ./Bin/RK33
ifeq (${PLAT},Rk3368Mcu)
VER_BIN_PREFIX		:= rk3368bl30_
endif
ifeq (${PLAT},Rk3366Mcu)
VER_BIN_PREFIX		:= rk3366bl30_
endif

VER = 

# Convenience function for adding build definitions
# $(eval $(call add_define,FOO)) will have:
# -DFOO if $(FOO) is empty; -DFOO=$(FOO) otherwise
define add_define
DEFINES			+= -D$(1)$(if $(value $(1)),=$(value $(1)),)
endef

# Convenience function for verifying option has a boolean value
# $(eval $(call assert_boolean,FOO)) will assert FOO is 0 or 1
define assert_boolean
$(and $(patsubst 0,,$(value $(1))),$(patsubst 1,,$(value $(1))),$(error $(1) must be boolean))
endef

# Check the platform which user inputed is supported or not.
ifeq (${PLAT},)
  $(error "Error: Unknown platform. Please use PLAT=<platform name> to specify the platform.")
endif
ifeq ($(findstring ${PLAT},${PLATFORMS}),)
  $(error "Error: Invalid platform. The following platforms are available: ${PLATFORMS}")
else
  $(shell echo "PLAT=${PLAT}" > .config)
endif

# All PHONY definition
.PHONY: all clean distclean ${ARCH} msg_start version
all: msg_start ${ARCH}

.SUFFIXES:

INCLUDES	+= -IPeripherals/Common/Inc			\
		-IPeripherals/Core/Inc				\
		-IPeripherals/Peri.rk/Inc			\
		-ICoOS						\
		-ICoOS/kernel					\
		-ICoOS/portable					\
		-IPeripherals/Peri.rk\Inc			\
		-IApp/Inc

CoOS_SOURCES	:= CoOS/portable/arch.c				\
		CoOS/portable/GCC/port.c			\
		CoOS/kernel/kernelHeap.c 			\
		CoOS/kernel/core.c				\
		CoOS/kernel/timer.c 				\
		CoOS/kernel/utility.c 				\
		CoOS/kernel/task.c				\
		CoOS/kernel/serviceReq.c 			\
		CoOS/kernel/mbox.c				\
		CoOS/kernel/mm.c 				\
		CoOS/kernel/time.c				\
		CoOS/kernel/event.c 				\
		CoOS/kernel/queue.c 				\
		CoOS/kernel/mutex.c 				\
		CoOS/kernel/flag.c				\
		CoOS/kernel/sem.c				\
		CoOS/kernel/hook.c

PER_SOURCES	:= Peripherals/Boot/startup.c			\
		Peripherals/Common/Syscalls/syscalls.c 		\
		Peripherals/Common/Stdio/printf.c 		\
		Peripherals/Peri.rk/Src/uart.c 			\
		Peripherals/Peri.rk/Src/sram.c 			\
		Peripherals/Peri.rk/Src/irq.c			\
		Peripherals/Peri.rk/Src/mailbox.c 		\
		Peripherals/Peri.rk/Src/rkjtag_mux.c		\
		Peripherals/Peri.rk/Src/cru.c

APP_SOURCES	:= App/main.c

ifeq (${PLAT},Rk3368Mcu)
PER_SOURCES	+= Peripherals/Peri.rk/Src/ddr_memSet.S 	\
		   Peripherals/Peri.rk/Src/ddr_rk3368.c

APP_SOURCES	+= App/tsadc.c					\
		   App/ddr.c
endif

# Process DEBUG flag
$(eval $(call assert_boolean,DEBUG))
$(eval $(call add_define,DEBUG))
ifeq (${DEBUG},0)
  $(eval $(call add_define,NDEBUG))
else
CFLAGS			+= -g
ASFLAGS			+= -g -Wa,--gdwarf-2
endif

ASFLAGS			+= -mcpu=$(ARCH) -mthumb -Wall -ffunction-sections -O3
CFLAGS			+= -mcpu=$(ARCH) -mthumb -Wall -ffunction-sections -O3

LDFLAGS			+= -mcpu=$(ARCH) -mthumb -g -nostartfiles -O3
LDFLAGS			+= -Wl,--gc-sections

CC			:= ${CROSS_COMPILE}gcc
CPP			:= ${CROSS_COMPILE}cpp
AS			:= ${CROSS_COMPILE}gcc
AR			:= ${CROSS_COMPILE}ar
LD			:= ${CROSS_COMPILE}ld
OC			:= ${CROSS_COMPILE}objcopy
OD			:= ${CROSS_COMPILE}objdump
NM			:= ${CROSS_COMPILE}nm
PP			:= ${CROSS_COMPILE}gcc -E ${CFLAGS}

# Function definition related compilation
define MAKE_C
$(eval OBJ := $(1)/$(patsubst %.c,%.o,$(notdir $(2))))

$(OBJ) : $(2)
	@echo "  CC      $$<"
	$$(Q)$$(CC) $$(CFLAGS) $$(LOCAL_CFLAGS) $$(INCLUDES) -c $$< -o $$@
endef

define MAKE_S
$(eval OBJ := $(1)/$(patsubst %.S,%.o,$(notdir $(2))))

$(OBJ) : $(2)
	@echo "  AS      $$<"
	$$(Q)$$(AS) $$(ASFLAGS) $$(LOCAL_CFLAGS) -c $$< -o $$@
endef

define MAKE_LD
$(1) :
	@echo "  PP      $$<"
	$$(AS) $$(ASFLAGS) -P -E -D__LINKER__ -o $$@ $$<
endef

define MAKE_OBJS
	$(eval C_OBJS := $(filter %.c,$(2)))
	$(eval REMAIN := $(filter-out %.c,$(2)))
	$(eval $(foreach obj,$(C_OBJS),$(call MAKE_C,$(1),$(obj),$(3))))

	$(eval S_OBJS := $(filter %.S,$(REMAIN)))
	$(eval REMAIN := $(filter-out %.S,$(REMAIN)))
	$(eval $(foreach obj,$(S_OBJS),$(call MAKE_S,$(1),$(obj),$(3))))

	$(and $(REMAIN),$(error Unexpected source files present: $(REMAIN)))
endef

# NOTE: The line continuation '\' is required in the next define otherwise we
# end up with a line-feed characer at the end of the last c filename.
# Also bare this issue in mind if extending the list of supported filetypes.
define SOURCES_TO_OBJS
	$(notdir $(patsubst %.c,%.o,$(filter %.c,$(1)))) \
	$(notdir $(patsubst %.S,%.o,$(filter %.S,$(1))))
endef

# NOTE: The Main compilation entry
# MAKE_MCU macro defines the targets and options to build the image.
define MAKE_MCU
	$(eval BUILD_DIR  := ${BUILD_PLAT}/obj)
	$(eval BIN_DIR   := ${BUILD_PLAT}/bin)
	$(eval SOURCES    := $(CoOS_SOURCES) $(PER_SOURCES) $(APP_SOURCES))
	$(eval OBJS       := $(addprefix $(BUILD_DIR)/,$(call SOURCES_TO_OBJS,$(SOURCES))))
	$(eval LINKERFILE := $(BUILD_BASE)/arm-gcc-link.ld)
	$(eval MAPFILE    := $(BIN_DIR)/$(PLAT).map)
	$(eval ELF        := $(BIN_DIR)/$(PLAT).elf)
	$(eval DUMP       := $(BIN_DIR)/$(PLAT).dump)
	$(eval BIN        := $(BIN_DIR)/$(PLAT).bin)

	$(eval $(call MAKE_OBJS,$(BUILD_DIR),$(SOURCES),$(1)))

$(BIN_DIR) :
	$$(Q)mkdir -p "$$@"

$(BUILD_DIR) : $(BIN_DIR)
	$$(Q)mkdir -p "$$@"

$(ELF) : $(OBJS)
	@echo "  LD      $$@"
	$$(Q)$$(CC) -o $$@ $$(LDFLAGS) -Wl,-Map=$(MAPFILE) -Wl,-T$(LINKERFILE) \
		$(OBJS) -lm -lgcc -lc

$(DUMP) : $(ELF)
	@echo "  OD      $$@"
	$${Q}$${OD} -dx $$< > $$@

$(BIN) : $(ELF)
	@echo "  BIN     $$@"
	$$(Q)$$(OC) -O binary $$< $$@
	@echo
	@echo "Built $$@ successfully"
	@echo

.PHONY : ${ARCH}
${ARCH} : $(BUILD_DIR) $(DUMP) $(BIN)
endef

$(eval $(call MAKE_MCU))

# Other common compilation entries
msg_start:
	@echo "  Building ${PLAT}"
	@echo

# Ensure that the BIN version is not NULL.
ifneq (${VER},)
version: _version
else
version: _err_verion
endif

_err_verion:
	@echo "Error: Please use 'make version VER=<version num>' to do version control."

_version:
	@echo "  Version control"
	@echo
	${Q}cp -r $(BIN) $(ELF) ${VER_BIN_DIR}/
	${Q}git rm -f ${VER_BIN_DIR}/$(VER_BIN_PREFIX)*
	${Q}mv ${VER_BIN_DIR}/$(PLAT).bin \
		${VER_BIN_DIR}/$(VER_BIN_PREFIX)$(VER).bin
	${Q}mv ${VER_BIN_DIR}/$(PLAT).elf \
		${VER_BIN_DIR}/$(VER_BIN_PREFIX)$(VER).elf
	${Q}git add ${VER_BIN_DIR}/$(VER_BIN_PREFIX)*
	@echo
	@echo "Version changed successfully"
	@echo

clean:
	@echo "  CLEAN"
	${Q}rm -rf ${BUILD_BASE}/${PLAT}
	${Q}rm -rf ${VER_BIN_DIR}/$(PLAT)*

distclean:
	@echo "  DISTCLEAN"
	${Q}rm -rf ${BUILD_BASE}/${PLAT}
	${Q}rm -rf ${VER_BIN_DIR}/$(PLAT)*
	${Q}rm -rf .config

help:
	@echo "usage: make PLAT=<${HELP_PLATFORMS}> <all|clean|distclean|version VER=[version num]>"
	@echo ""
	@echo "PLAT is used to specify which platform you wish to build."
	@echo "If no platform is specified in first time, PLAT defaults to: ${DEFAULT_PLAT}"
	@echo ""
	@echo "Supported Targets:"
	@echo "  all            		Build all the project"
	@echo "  clean          		Clean the current platform project"
	@echo "  distclean			Clean the current project and delete .config"
	@echo "  version VER=[version num]	Add one new BIN for git version control"
	@echo ""
	@echo "example: build the targets for the Rk3368Mcu project:"
	@echo "  make PLAT=Rk3368Mcu"
