#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

# ccflags-y += -DET_DEBUG

extra-y += start.o
extra-y += traps.o
obj-y	+= cpu.o
obj-y	+= cpu_init.o
obj-$(CONFIG_OF_LIBFDT) += fdt.o
obj-y	+= interrupts.o
obj-$(CONFIG_CMD_REGINFO) += reginfo.o
obj-y	+= speed.o
obj-y	+= plprcr_write.o
