/*
 * Copyright (C) 2010-2017 CS Systemes d'Information
 * Christophe Leroy <christophe.leroy@c-s.fr>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*-----------------------------------------------------------------------
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_MPC866			1

#define CONFIG_BOARD_EARLY_INIT_F	1	/* Call board_early_init_f 		*/
#define CONFIG_BOARD_EARLY_INIT_R	1	/* Call board_early_init_r 		*/
#define CONFIG_MISC_INIT_R		1	/* Call misc_init_r			*/

#define	CONFIG_8xx_CONS_SMC1		1	/* Console is on SMC1 			*/
#undef	CONFIG_8xx_CONS_SMC2
#undef	CONFIG_8xx_CONS_NONE
#define CONFIG_BAUDRATE			115200

#define CONFIG_EXTRA_ENV_SETTINGS 	"sdram_type=SDRAM\0"								\
					"flash_type=AM29LV160DB\0"							\
					"loadaddr=0x400000\0"								\
					"filename=uImage.lzma\0"							\
					"nfsroot=/opt/ofs\0"								\
					"dhcp_ip=ip=:::::eth0:dhcp\0"							\
					"console_args=console=ttyCPM0,115200N8\0"					\
					"flashboot=setenv bootargs "							\
						"${console_args} "							\
						"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:mcr3k:eth0:off "	\
						"${ofl_args}; "								\
						"bootm 0x04060000 - 0x04050000\0"					\
					"tftpboot=setenv bootargs "							\
						"${console_args} "							\
						"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:mcr3k:eth0:off "	\
						"${ofl_args}; "								\
						"tftp ${loadaddr} ${filename};"						\
						"tftp 0xf00000 mcr3000.dtb;"						\
						"bootm ${loadaddr} - 0xf00000\0"					\
					"netboot=dhcp ${loadaddr} ${filename};"						\
						"tftp 0xf00000 mcr3000.dtb;"						\
						"setenv bootargs "							\
						"root=/dev/nfs rw "							\
						"${console_args} "							\
						"${dhcp_ip};"								\
						"bootm ${loadaddr} - 0xf00000\0"					\
					"nfsboot=setenv bootargs "							\
						"root=/dev/nfs rw nfsroot=${serverip}:${nfsroot} "			\
						"${console_args} "							\
						"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:mcr3k:eth0:off;"	\
						"bootm 0x04060000 - 0x04050000\0"					\
					"dhcpboot=dhcp ${loadaddr} ${filename};"					\
						"tftp 0xf00000 mcr3000.dtb;"						\
						"setenv bootargs "							\
						"${console_args} "							\
						"${dhcp_ip} "								\
						"${ofl_args}; "								\
						"bootm ${loadaddr} - 0xf00000\0"

#define CONFIG_BOOTDELAY		5	/* autoboot. Put -1 to disabled */

#define CONFIG_IPADDR			192.168.0.3
#define CONFIG_SERVERIP			192.168.0.1
#define CONFIG_NETMASK			255.0.0.0

#define CONFIG_BOOTCOMMAND		"run flashboot"
#define CONFIG_BOOTARGS			"ubi.mtd=4 root=ubi0:rootfs rw rootfstype=ubifs rootflags=sync "	\
					"console=ttyCPM0,115200N8 "						\
					"ip=${ipaddr}:::${netmask}:mcr3k:eth0:off"

#define CONFIG_LOADS_ECHO		1	/* echo on for serial download	*/
#undef	CONFIG_LOADS_BAUD_CHANGE		/* don't allow baudrate change	*/

#define CONFIG_WATCHDOG                 1	/* watchdog enabled            */



/*-----------------------------------------------------------------------
 * Password configuration.
 */
#define CONFIG_AUTOBOOT_KEYED		1		/* Enable "password" protection		*/
#define CONFIG_AUTOBOOT_PROMPT		"\nEnter password - autoboot in %d sec...\n"
#define CONFIG_AUTOBOOT_DELAY_STR	"root"		/* 1st password				*/



/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory		*/
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size */

#define CONFIG_CMDLINE_EDITING		1		/* add command line history	*/
#ifdef	CONFIG_HUSH_PARSER
#define	CONFIG_SYS_PROMPT_HUSH_PS2	"S3K> "
#endif

#define CONFIG_SYS_MEMTEST_START	0x00002000	/* memtest works on		*/
#define CONFIG_SYS_MEMTEST_END		0x00800000	/* 8 KB ... 8 MB in DRAM	*/

#define	CONFIG_SYS_LOAD_ADDR		0x200000	/* default load address		*/

#define	CONFIG_SYS_HZ			1000		/* decrementer freq: 1 ms ticks	*/

#define CONFIG_SYS_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}


#define CONFIG_SYS_CLK_FREQ		132000000
#define CONFIG_8xx_GCLK_FREQ    	132000000



/*-----------------------------------------------------------------------
 * Physical memory map of the MCR3000 board
 */
#define CONFIG_SYS_IMMR			0xFF000000 /* Internal Memory Mapped Register*/

#define CONFIG_SYS_OR0_PRELIM		0xFFC00926
#define CONFIG_SYS_BR0_PRELIM		0x04000801
#define CONFIG_SYS_OR1_PRELIM		0xFE000E00
#define CONFIG_SYS_BR1_PRELIM		0x00000081
#define CONFIG_SYS_OR2_PRELIM		0xFFFF8F2A
#define CONFIG_SYS_BR2_PRELIM		0x08000801
#define CONFIG_SYS_OR3_PRELIM		0xFFFF8142
#define CONFIG_SYS_BR3_PRELIM		0x0C000401
#define CONFIG_SYS_OR4_PRELIM		0xFFFF8D08
#define CONFIG_SYS_BR4_PRELIM		0x10000801
#define CONFIG_SYS_OR5_PRELIM		0xFFFF8916
#define CONFIG_SYS_BR5_PRELIM		0x14000801
#define CONFIG_SYS_OR6_PRELIM		0xFFFF0908
#define CONFIG_SYS_BR6_PRELIM		0x18000801
#define CONFIG_SYS_OR7_PRELIM		0xFFFF810A
#define CONFIG_SYS_BR7_PRELIM		0x1C000001

#define ADDR_FLASH		( CONFIG_SYS_BR0_PRELIM & 0xFFFF0000)
#define ADDR_SDRAM		( CONFIG_SYS_BR1_PRELIM & 0xFFFF0000)
#define ADDR_RAMDP		( CONFIG_SYS_BR2_PRELIM & 0xFFFF0000)
#define ADDR_NAND		( CONFIG_SYS_BR3_PRELIM & 0xFFFF0000)
#define ADDR_MTSL1              ((CONFIG_SYS_BR4_PRELIM & 0xFFFF0000) | 0x00000000)
#define ADDR_MTSL2              ((CONFIG_SYS_BR4_PRELIM & 0xFFFF0000) | 0x00000200)
#define ADDR_E1                 ((CONFIG_SYS_BR4_PRELIM & 0xFFFF0000) | 0x00000400)
#define ADDR_SEM                ((CONFIG_SYS_BR4_PRELIM & 0xFFFF0000) | 0x00000600)
#define ADDR_CPLD               ((CONFIG_SYS_BR4_PRELIM & 0xFFFF0000) | 0x00000800)



/*-----------------------------------------------------------------------
 * Definitions for initial stack pointer and data area (in DPRAM)
 */
#define CONFIG_SYS_INIT_RAM_ADDR	CONFIG_SYS_IMMR
#define	CONFIG_SYS_INIT_RAM_SIZE		0x2f00		/* End of used area in DPRAM		*/
#define	CONFIG_SYS_GBL_DATA_SIZE	64		/* size (bytes) reserved for initial data */
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - CONFIG_SYS_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET



/*-----------------------------------------------------------------------
 * RAM configuration (note that CONFIG_SYS_SDRAM_BASE must be zero)
 */
#define	CONFIG_SYS_SDRAM_BASE		0x00000000
#define CONFIG_SYS_SDRAM_MAX_SIZE	(32*1024*1024)	/* 32 Mega Bytes 			*/



/*-----------------------------------------------------------------------
 * FLASH organization
 */
#define CONFIG_SYS_FLASH_BASE		CONFIG_SYS_TEXT_BASE	/* FLASH address			*/
#define CONFIG_SYS_FLASH_CFI		1		/* The flash is CFI compatible		*/
#define CONFIG_FLASH_CFI_DRIVER		1		/* Use common CFI driver		*/
#define CONFIG_SYS_MAX_FLASH_BANKS	1		/* max number of memory banks		*/
#define CONFIG_SYS_MAX_FLASH_SECT	35		/* max number of sectors on one chip	*/
#define CONFIG_SYS_FLASH_ERASE_TOUT	120000		/* Timeout for Flash Erase (in ms)	*/
#define CONFIG_SYS_FLASH_WRITE_TOUT	500		/* Timeout for Flash Write (in ms)	*/



/*-----------------------------------------------------------------------
 * For booting Linux, the board info and command line data
 * have to be in the first 8 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define	CONFIG_SYS_BOOTMAPSZ		(8    << 20)	/* Initial Memory map for Linux		*/
#define	CONFIG_SYS_MONITOR_LEN		(256  << 10)	/* Reserve 256 KB for Monitor		*/
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MALLOC_LEN		(4096 << 10)	/* Reserve 4096 KB for malloc()		*/



/*-----------------------------------------------------------------------
 * Environment Configuration
 */
/* environment is in FLASH */
#define CONFIG_ENV_IS_IN_FLASH		1
#define CONFIG_ENV_SECT_SIZE		(64 * 1024)	/* Total size of Environment Sector	*/
#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_SYS_MONITOR_LEN)
#define CONFIG_ENV_OFFSET		(CONFIG_ENV_ADDR - CONFIG_SYS_FLASH_BASE)
#define CONFIG_ENV_OVERWRITE		1		/* Allow user to modify environment	*/



/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_CACHELINE_SIZE	16		/* For all MPC8xx CPUs			*/



/*-----------------------------------------------------------------------
 * Configuration registers
 */
#if defined(CONFIG_WATCHDOG)
#define CONFIG_SYS_SYPCR		(SYPCR_SWTC | SYPCR_BMT  | SYPCR_BME  | SYPCR_SWF  | SYPCR_SWE  | SYPCR_SWRI | SYPCR_SWP)

#else
#define CONFIG_SYS_SYPCR		(SYPCR_SWTC | SYPCR_BMT | SYPCR_BME | SYPCR_SWF | SYPCR_SWP)
#endif

#define CONFIG_SYS_SIUMCR		(SIUMCR_DBGC11 | SIUMCR_MLRC01)

/* TBSCR - Time Base Status and Control Register */
#define CONFIG_SYS_TBSCR 		(TBSCR_REFA | TBSCR_REFB | TBSCR_TBF | TBSCR_TBE)

/* PISCR - Periodic Interrupt Status and Control */
#define CONFIG_SYS_PISCR		0x0000

/* PLPRCR */
#define CONFIG_SYS_PLPRCR		(0x00460004)

/* SCCR - System Clock and reset Control Register */
#define SCCR_MASK			SCCR_COM00
#define CONFIG_SYS_SCCR			(SCCR_RTSEL | SCCR_CRQEN | SCCR_EBDF01)

#define CONFIG_SYS_DER			(0x2002000F)



/*-----------------------------------------------------------------------
 * Ethernet configuration part
 */
#define CONFIG_MII			1
#define CONFIG_SYS_DISCOVER_PHY		1
#define CONFIG_MII_INIT			1
#define FEC_ENET			1



/*-----------------------------------------------------------------------
 * NAND configuration part
 */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_MAX_CHIPS	1
#define CONFIG_SYS_NAND_BASE		0x0C000000	/* Base address of the NAND FLASH	*/



/*-----------------------------------------------------------------------
 * Pass open firmware flat tree
 */
#define CONFIG_OF_LIBFDT		1
#define CONFIG_OF_BOARD_SETUP		1



/*-----------------------------------------------------------------------
 * Internal Definitions
 *
 * Boot Flags
 */
#define	BOOTFLAG_COLD			0x01		/* Normal Power-On: Boot from FLASH	*/
#define BOOTFLAG_WARM			0x02		/* Software reboot			*/



/*-----------------------------------------------------------------------
 * Misc Settings
 */
#define CONFIG_LZMA
#define CONFIG_SHA256



#endif /* __CONFIG_H */
