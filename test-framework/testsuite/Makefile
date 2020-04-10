ifeq ($(CONFIG_TARGET_RPI_2),y)
obj-$(CONFIG_BOARDDIAG) += test_ping.o
obj-$(CONFIG_BOARDDIAG) += test_mmc.o
obj-$(CONFIG_BOARDDIAG) += test_memory.o
else ifeq ($(CONFIG_ARCH_OMAP2PLUS),y)
obj-$(CONFIG_BOARDDIAG) += test_ping.o
obj-$(CONFIG_BOARDDIAG) += test_mmc.o
obj-$(CONFIG_BOARDDIAG) += test_i2cprobe.o
obj-$(CONFIG_BOARDDIAG) += test_memory.o
obj-$(CONFIG_BOARDDIAG) += test_gpio.o
#obj-$(CONFIG_BOARDDIAG) += test_memmap.o
obj-$(CONFIG_BOARDDIAG) += test_eth.o
obj-$(CONFIG_BOARDDIAG) += test_tps65217_powerrail.o
obj-$(CONFIG_BOARDDIAG) += test_at24c_eeprom.o
obj-$(CONFIG_BOARDDIAG) += test_w25q32_norflash.o
obj-$(CONFIG_BOARDDIAG) += test_ds2321_rtc.o

else

endif

