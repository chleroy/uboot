/*
 * Copyright (C) 2010-2013 CSSI, Inc.
 * Florent TRINH THAI (florent.trinh-thai@c-s.fr)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>

#if defined(CONFIG_CMD_NAND)
#include <nand.h>

#define BIT_CLE			((unsigned short)0x0800)
#define BIT_ALE			((unsigned short)0x0400)
#define BIT_NCE			((unsigned short)0x1000)

static void nand_hwcontrol(struct mtd_info *mtdinfo, int cmd, unsigned int ctrl)
{
	struct nand_chip *this 	= mtdinfo->priv;
	volatile immap_t *immr 	= (immap_t *) CONFIG_SYS_IMMR;
	unsigned short pddat 	= 0;


	/* The hardware control change */
	if (ctrl & NAND_CTRL_CHANGE) {

		/* saving current value */
		pddat = immr->im_ioport.iop_pddat;

		/* Clearing ALE and CLE */
		pddat &= ~(BIT_CLE | BIT_ALE);

		/* Driving NCE pin */
		if (ctrl & NAND_NCE) {
			pddat &= ~BIT_NCE;
		} else {
			pddat |= BIT_NCE;
		}

		/* Driving CLE and ALE pin */
		if (ctrl & NAND_CLE) {
			pddat |= BIT_CLE;
		}
		if (ctrl & NAND_ALE) {
			pddat |= BIT_ALE;
		}

		/* applying new value */
		immr->im_ioport.iop_pddat = pddat;
	}

	/* Writing the command */
	if (cmd != NAND_CMD_NONE) {
		*((unsigned char *)this->IO_ADDR_W) = (unsigned char)cmd;
	}
}

int board_nand_init(struct nand_chip *nand)
{
	volatile immap_t *immr 	= (immap_t *) CONFIG_SYS_IMMR;

	/* Set GPIO Port */
	immr->im_ioport.iop_pddir |=  0x1c00;
	immr->im_ioport.iop_pdpar &= ~0x1c00;
	immr->im_ioport.iop_pddat |=  0x1000;	/* au repos CE doit etre à 1 	*/
	immr->im_ioport.iop_pddat &= ~0x0c00;	/* au repos ALE et CLE sont à 0 */

	nand->chip_delay 	= 60;
	nand->ecc.mode 		= NAND_ECC_SOFT;
	nand->cmd_ctrl 		= nand_hwcontrol;

	return 0;
}

#endif	/* defined(CONFIG_CMD_NAND) */
