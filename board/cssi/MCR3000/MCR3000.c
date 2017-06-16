/*
 * Copyright (C) 2010-2017 CS Systemes d'Information
 * Florent Trinh Thai <florent.trinh-thai@c-s.fr>
 * Christophe Leroy <christophe.leroy@c-s.fr>
 *
 * Board specific routines for the MCR3000 board
 *
 * - initialisation
 * - memory controller
 * - serial io initialisation
 * - ethernet io initialisation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <hwconfig.h>
#include <mpc8xx.h>
#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#endif

DECLARE_GLOBAL_DATA_PTR;


/* -------------------------------------------------------------------------
 * constant
 */
static const uint cs1_dram_table_66[] = {
	/* DRAM - single read. (offset 0 in upm RAM)
	 */
	0x0F3DFC04, 0x0FEFBC04, 0x00BE7804, 0x0FFDF400,
	0x1FFFFC05, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	/* DRAM - burst read. (offset 8 in upm RAM)
	 */
	0x0F3DFC04, 0x0FEFBC04, 0x00BF7C04, 0x00FFFC00,
	0x00FFFC00, 0x00FEF800, 0x0FFDF400, 0x1FFFFC05,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	/* DRAM - single write. (offset 18 in upm RAM)
	 */
	0x0F3DFC04, 0x0FEFB800, 0x00BF7404, 0x0FFEF804,
	0x0FFDF404, 0x1FFFFC05, 0xFFFFFFFF, 0xFFFFFFFF,

	/* DRAM - burst write. (offset 20 in upm RAM)
	 */
	0x0F3DFC04, 0x0FEFB800, 0x00BF7400, 0x00FFFC00,
	0x00FFFC00, 0x00FFFC04,	0x0FFEF804, 0x0FFDF404,
	0x1FFFFC05, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	/* refresh  (offset 30 in upm RAM)
	 */
	0x0FFDF404, 0x0FFEBC04, 0x0FFD7C84, 0x0FFFFC04,
	0x0FFFFC04, 0x0FFFFC04, 0x1FFFFC85, 0xFFFFFFFF,

	/* init
	 */
	0x0FEEB874, 0x0FBD7474, 0x1FFFFC45, 0xFFFFFFFF,

	/* exception. (offset 3c in upm RAM)
	 */
	0xFFFFFC05, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

/* -------------------------------------------------------------------------
 * Device Tree Support
 */
#if defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT)
int fdt_set_node_and_value (void *blob,	char *nodename,	char *regname, void *var, int size)
{
	int ret = 0;
	int nodeoffset = 0;

	nodeoffset = fdt_path_offset (blob, nodename);
	if (nodeoffset >= 0) {
		ret = fdt_setprop (blob, nodeoffset, regname, var,
					size);
		if (ret < 0) {
			printf("ft_blob_update(): cannot set %s/%s property; err: %s\n", nodename, regname, fdt_strerror (ret));
		}
	} else {
		printf("ft_blob_update(): cannot find %s node err:%s\n", nodename, fdt_strerror (nodeoffset));
	}
	return ret;
}

int fdt_del_node_name (void *blob, char *nodename)
{
	int ret = 0;
	int nodeoffset = 0;

	nodeoffset = fdt_path_offset (blob, nodename);
	if (nodeoffset >= 0) {
		ret = fdt_del_node (blob, nodeoffset);
		if (ret < 0) {
			printf("%s: cannot delete %s; err: %s\n", __func__, nodename, fdt_strerror (ret));
		}
	} else {
		printf("%s: cannot find %s node err:%s\n", __func__, nodename, fdt_strerror (nodeoffset));
	}
	return ret;
}

int fdt_del_prop_name (void *blob, char *nodename, char *propname)
{
	int ret = 0;
	int nodeoffset = 0;

	nodeoffset = fdt_path_offset (blob, nodename);
	if (nodeoffset >= 0) {
		ret = fdt_delprop (blob, nodeoffset, propname);
		if (ret < 0) {
			printf("%s: cannot delete %s %s; err: %s\n", __func__, nodename, propname, fdt_strerror (ret));
		}
	} else {
		printf("%s: cannot find %s node err:%s\n", __func__, nodename, fdt_strerror (nodeoffset));
	}
	return ret;
}

/*
 * update "brg" property in the blob
 */
void ft_blob_update (void *blob, bd_t *bd)
{
	uchar enetaddr[6];
	ulong brg_data = 0;

	/* BRG */
	brg_data = cpu_to_be32(bd->bi_busfreq);
	fdt_set_node_and_value(blob, "/soc/cpm", "brg-frequency", &brg_data, sizeof(brg_data));

	/* MAC addr */
	if (eth_getenv_enetaddr("ethaddr", enetaddr)) {
		fdt_set_node_and_value(blob, "ethernet0", "local-mac-address", enetaddr, sizeof(u8) * 6);
	}

	/* Bus Frequency for CPM */
	fdt_set_node_and_value(blob, "/soc", "bus-frequency", &bd->bi_busfreq, sizeof(u32));
}

void ft_e1_setup (void *blob)
{
	ulong data_rate = 2;
	ulong channel_phase = 0;
	char *sync = "receive";

	/* Set data rate */
	fdt_set_node_and_value(blob, "/localbus/e1-wan", "data-rate", &data_rate,
	 sizeof(u32));

	/* Set channel phase to 0 */
	fdt_set_node_and_value(blob, "/localbus/e1-wan", "channel-phase",
		&channel_phase, sizeof(u32));

	/* rising edge sync pulse transmit */
	fdt_set_node_and_value(blob, "/localbus/e1-wan", "rising-edge-sync-pulse",
		 sync, sizeof(u8) * strlen(sync));
}

void ft_board_setup(void *blob, bd_t *bd)
{
	ft_cpu_setup(blob, bd);
	ft_blob_update(blob, bd);
	ft_e1_setup(blob);
}
#endif /* defined(CONFIG_OF_BOARD_SETUP) && defined(CONFIG_OF_LIBFDT) */



/* -------------------------------------------------------------------------
 * return CPU frequency
 */
/*static unsigned int board_get_cpufreq (void)
{
	return (CONFIG_SYS_CLK_FREQ);
}*/


/* -------------------------------------------------------------------------
 * Check Board Identity:
 */
int checkboard (void)
{
	serial_putc ('B');
	serial_putc ('O');
	serial_putc ('A');
	serial_putc ('R');
	serial_putc ('D');
	serial_putc (':');
	serial_putc (' ');
	serial_putc ('M');
	serial_putc ('C');
	serial_putc ('R');
	serial_putc ('3');
	serial_putc ('0');
	serial_putc ('0');
	serial_putc ('0');
	serial_putc (' ');
	serial_putc ('C');
	serial_putc ('S');
	serial_putc ('S');
	serial_putc ('I');
	serial_putc ('\n');

	return 0;
}


/* -------------------------------------------------------------------------
 * Initialize SDRAM
 */
int dram_init(void)
{
	volatile immap_t *immap 	= (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile memctl8xx_t *memctl 	= &immap->im_memctl;
	long int ram_sz 		= 0;

	/*
	 * Only initialize memory controller when running from FLASH.
	 * When running from RAM, don't touch it.
	 */
	if ( ((ulong) dram_init & CONFIG_SYS_FLASH_BASE) == CONFIG_SYS_FLASH_BASE) {
		printf("UPMA init for SDRAM (CAS latency 2), ");
		printf("init address 0x%08x, size ",(int)dram_init);
		/* Configure UPMA for cs1 */
		upmconfig (UPMA, (uint *) cs1_dram_table_66, sizeof (cs1_dram_table_66) / sizeof (uint));
		udelay(10);
		memctl->memc_mptpr 	= 0x0200;
		memctl->memc_mamr 	= 0x14904000;
		udelay(10);
		memctl->memc_or1 	= CONFIG_SYS_OR1_PRELIM;
		memctl->memc_br1 	= CONFIG_SYS_BR1_PRELIM;
		udelay(10);
		memctl->memc_mcr 	= 0x80002830;
		memctl->memc_mar 	= 0x00000088;
		memctl->memc_mcr 	= 0x80002038;
		udelay(200);
		/* now map other option and base registers */
		memctl->memc_or2 = CONFIG_SYS_OR2_PRELIM;
		memctl->memc_br2 = CONFIG_SYS_BR2_PRELIM;
		memctl->memc_or3 = CONFIG_SYS_OR3_PRELIM;
		memctl->memc_br3 = CONFIG_SYS_BR3_PRELIM;
		memctl->memc_or4 = CONFIG_SYS_OR4_PRELIM;
		memctl->memc_br4 = CONFIG_SYS_BR4_PRELIM;
		memctl->memc_or5 = CONFIG_SYS_OR5_PRELIM;
		memctl->memc_br5 = CONFIG_SYS_BR5_PRELIM;
		memctl->memc_or6 = CONFIG_SYS_OR6_PRELIM;
		memctl->memc_br6 = CONFIG_SYS_BR6_PRELIM;
		memctl->memc_or7 = CONFIG_SYS_OR7_PRELIM;
		memctl->memc_br7 = CONFIG_SYS_BR7_PRELIM;

		ram_sz = get_ram_size(CONFIG_SYS_SDRAM_BASE, SDRAM_MAX_SIZE);
	}

	gd->ram_size = ram_sz;

	return 0;
}

/* -------------------------------------------------------------------------
 * Specific board pre-initialization
 */
int board_early_init_r (void)
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;
	char *s;

#if defined(CONFIG_WATCHDOG)
	s = getenv("watchdog");
	/* watchdog disabled */
	if (strcmp(s, "off") == 0) {
		printf("       Watchdog disabled\n");
		immr->im_siu_conf.sc_sypcr = CONFIG_SYS_SYPCR & ~(SYPCR_SWE  | SYPCR_SWRI);

	/* watchdog is enabled */
	} else {

		/* NMI mode */
		if (strcmp(s, "nmi") == 0) {
			printf("       Watchdog enabled nmi\n");
			immr->im_siu_conf.sc_sypcr = (CONFIG_SYS_SYPCR & ~(SYPCR_SWRI)) | SYPCR_SWE;

		/* normal mode by default */
		} else {
			printf("       Watchdog enabled\n");
			immr->im_siu_conf.sc_sypcr = CONFIG_SYS_SYPCR | SYPCR_SWE  | SYPCR_SWRI;
		}
	}
#else
	immr->im_siu_conf.sc_sypcr = CONFIG_SYS_SYPCR & ~(SYPCR_SWE  | SYPCR_SWRI);
#endif
	return 0;
}

/* -------------------------------------------------------------------------
 * Miscelaneous init
 */

#define ADDR_CPLD_R_ETAT 	((unsigned short *)0x10000806)
#define R_ID_CPLD_MASK		0xFF00

#define ADDR_FLASH_ENV_AREA	((unsigned short *)0x04040000)

struct environment {
	uint32_t	crc;
	uint8_t		data[];
};


#define LEN_STR 80

int misc_init_r (void)
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile iop8xx_t *iop = (iop8xx_t *) &immr->im_ioport;
	char str[LEN_STR];
	char s_ethaddr[LEN_STR];
	char s_id_cpld[LEN_STR];
	char s_num_serie[LEN_STR];
	char s_password[LEN_STR];
	unsigned short version_cpld;
	struct environment *env;
	uint32_t crc;
	char *s;
	int i, j;

	/* --------------------------------------------
	 * GPIO or per. Function
	 * PCPAR[13] = 0 [0x0004] 	-> GPIO: (BTN_ACQ_AL)
	 * -------------------------------------------- */
	iop->iop_pcpar |=  0x0000;	/* set  bits	*/
	iop->iop_pcpar &= ~0x0004;	/* reset bit	*/

	/* ----------------------------------------------
	 * In/Out or per. Function 0/1
	 * PCDIR[13] = 1 [0x0004] 	-> GPIO: IN (CS_POT1_5V)
	 * ---------------------------------------------- */
	iop->iop_pcdir |=  0x0000;
	iop->iop_pcdir &= ~0x0004;

	/* if BTN_ACQ_IN is pressed then bootdelay is changed to 60 second */
	if ((iop->iop_pcdat & 0x0004) == 0) {
		setenv("bootdelay", "60");
	}

	/* verifying environment variable area */
	env = (struct environment *)CONFIG_ENV_ADDR;
	crc = crc32(0, env->data, (CONFIG_ENV_SIZE - sizeof(uint32_t)));
	if (crc != env->crc) {
		/* It can be an update request */
		for (i=0 ; i<8 ; i++) {str[i] = *(((uint8_t *)CONFIG_ENV_ADDR) + i); str[(i+1)] = 0;}
		if (strcmp(str, "ETHADDR=") == 0) {
			/* getting saved value */
			i = 0;
			for (j=0 ; j<4 ; j++) {
				while (*(((uint8_t *)CONFIG_ENV_ADDR) + i) != '=') {
					i++;
				}
				switch (j) {
					case 0:	s = s_ethaddr;		break;
					case 1:	s = s_num_serie;	break;
					case 2:	s = s_id_cpld;		break;
					case 3:	s = s_password;		break;
				}
				do {
					i++;
					*s = *(((uint8_t *)CONFIG_ENV_ADDR) + i);
					s++;
				} while (*(((uint8_t *)CONFIG_ENV_ADDR) + i) != 0x00);
			}

			/* creating or updating environment variable */
			if (s_ethaddr[0] != 0x00) {
				setenv("ethaddr", s_ethaddr);
			}
			if (s_num_serie[0] != 0x00) {
				setenv("num_serie", s_num_serie);
			}
			if (s_id_cpld[0] != 0x00) {
				setenv("id_cpld", s_id_cpld);
			}
			if (s_password[0] != 0x00) {
				setenv("password", s_password);
			}
			saveenv();
		}
	}

	/* we do not modify environment variable area if CRC is false */
	crc = crc32(0, env->data, (CONFIG_ENV_SIZE - sizeof(uint32_t)));
	if (crc == env->crc) {
		/* getting version value in CPLD register */
		for (i=0 ; i<LEN_STR ; i++) str[i] = 0;
		version_cpld = *ADDR_CPLD_R_ETAT & R_ID_CPLD_MASK;
		if (((version_cpld >> 12) & 0x000f) < 0x000a) {
			str[0] = ((version_cpld >> 12) & 0x000f) + 0x30;
		} else {
			str[0] = (((version_cpld >> 12) & 0x000f) - 0x000a) + 0x41;
		}
		if (((version_cpld >> 8) & 0x000f) < 0x000a) {
			str[1] = ((version_cpld >> 8) & 0x000f) + 0x30;
		} else {
			str[1] = (((version_cpld >> 8) & 0x000f) - 0x000a) + 0x41;
		}
		str[2] = 0x30;
		str[3] = 0x30;

		/* updating "id_cpld" variable if not corresponding with the value in register */
		s = getenv("id_cpld");
		if ((s == NULL) || (strcmp(s, str) != 0)) {
			setenv("id_cpld", str);
			saveenv();
		}
	}

	return 0;
}

int board_early_init_f (void)
{
	volatile immap_t *immr = (volatile immap_t *)CONFIG_SYS_IMMR;

	/*
	 * Erase FPGA(s) for reboot
	 */
	immr->im_cpm.cp_pbdat &= ~0x00020000;	/* PROGFPGA down */
	immr->im_cpm.cp_pbdir |= 0x00020000;	/* PROGFPGA output */
	udelay(1);				/* Wait more than 300ns */
	immr->im_cpm.cp_pbdat |= 0x00020000;	/* PROGFPGA up */
	return 0;
}

