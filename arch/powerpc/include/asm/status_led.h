/*
 * asm/status_led.h
 *
 * MPC8xx based status led support functions
 */

#ifndef __ASM_STATUS_LED_H__
#define __ASM_STATUS_LED_H__

/* if not overridden */
#ifndef CONFIG_LED_STATUS_BOARD_SPECIFIC
# if defined(CONFIG_8xx)
#  include <mpc8xx.h>
# else
#  error CPU specific Status LED header file missing.
#endif

/* led_id_t is unsigned long mask */
typedef unsigned long led_id_t;

static inline void __led_init (led_id_t mask, int state)
{
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;

#ifdef STATUS_LED_PAR
	immr->STATUS_LED_PAR &= ~mask;
#endif
#ifdef STATUS_LED_ODR
	immr->STATUS_LED_ODR &= ~mask;
#endif

#if (STATUS_LED_ACTIVE == 0)
	if (state == CONFIG_LED_STATUS_ON)
		immr->STATUS_LED_DAT &= ~mask;
	else
		immr->STATUS_LED_DAT |= mask;
#else
	if (state == CONFIG_LED_STATUS_ON)
		immr->STATUS_LED_DAT |= mask;
	else
		immr->STATUS_LED_DAT &= ~mask;
#endif
#ifdef STATUS_LED_DIR
	immr->STATUS_LED_DIR |= mask;
#endif
}

static inline void __led_toggle (led_id_t mask)
{
	((immap_t *) CONFIG_SYS_IMMR)->STATUS_LED_DAT ^= mask;
}

static inline void __led_set (led_id_t mask, int state)
{
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;

#if (STATUS_LED_ACTIVE == 0)
	if (state == CONFIG_LED_STATUS_ON)
		immr->STATUS_LED_DAT &= ~mask;
	else
		immr->STATUS_LED_DAT |= mask;
#else
	if (state == CONFIG_LED_STATUS_ON)
		immr->STATUS_LED_DAT |= mask;
	else
		immr->STATUS_LED_DAT &= ~mask;
#endif

}

#endif

#endif	/* __ASM_STATUS_LED_H__ */