#pragma once
#include "reg_access.h"

/* -------- General Purpose I/O (GPIO) -------- */
#define GPIOA_BASE 0x40010800
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400

/* Register offsets */
#define GPIO_CFGLR_OFFSET 0x00
#define GPIO_OUTDR_OFFSET 0x0C
#define GPIO_BSHR_OFFSET  0x10

/* Register access */
#define GPIOA_CFGLR REG(GPIOA_BASE + GPIO_CFGLR_OFFSET)
#define GPIOC_CFGLR REG(GPIOC_BASE + GPIO_CFGLR_OFFSET)
#define GPIOD_CFGLR REG(GPIOD_BASE + GPIO_CFGLR_OFFSET)

#define GPIOA_OUTDR REG(GPIOA_BASE + GPIO_OUTDR_OFFSET)
#define GPIOC_OUTDR REG(GPIOC_BASE + GPIO_OUTDR_OFFSET)
#define GPIOD_OUTDR REG(GPIOD_BASE + GPIO_OUTDR_OFFSET)

#define GPIOA_BSHR  REG(GPIOA_BASE + GPIO_BSHR_OFFSET)
#define GPIOC_BSHR  REG(GPIOC_BASE + GPIO_BSHR_OFFSET)
#define GPIOD_BSHR  REG(GPIOD_BASE + GPIO_BSHR_OFFSET)

/* Pin helper */
#define PIN(n) (1U << ((uint8_t)(n)))

/* GPIO configuration modes */
#define GPIO_MODE_IN_ANALOG       0b0000
#define GPIO_MODE_IN_FLOATING     0b0100
#define GPIO_MODE_IN_PUPD         0b1000

#define GPIO_MODE_OUT_PP_10MHZ    0b0001
#define GPIO_MODE_OUT_PP_2MHZ     0b0010
#define GPIO_MODE_OUT_PP_50MHZ    0b0011

#define GPIO_MODE_OUT_OD_10MHZ    0b0101
#define GPIO_MODE_OUT_OD_2MHZ     0b0110
#define GPIO_MODE_OUT_OD_50MHZ    0b0111

#define GPIO_MODE_AF_PP_10MHZ     0b1001
#define GPIO_MODE_AF_PP_2MHZ      0b1010
#define GPIO_MODE_AF_PP_50MHZ     0b1011

#define GPIO_MODE_AF_OD_10MHZ     0b1101
#define GPIO_MODE_AF_OD_2MHZ      0b1110
#define GPIO_MODE_AF_OD_50MHZ     0b1111

/* Configuration helpers */
#define GPIO_MODE_BITS(pin, mode) ((uint32_t)(mode) << ((pin) * 4))

#define GPIO_CONFIG_PORT(GPIO_CFGLR, pinmask, mode) do {     \
	uint32_t tmp = GPIO_CFGLR;                               \
	for (uint8_t pin = 0; pin < 8; pin++) {                  \
		if ((pinmask) & (1U << pin)) {                       \
			tmp &= ~(0xFU << (pin * 4));                     \
			tmp |= ((uint32_t)(mode & 0xF) << (pin * 4));    \
		}                                                    \
	}                                                        \
	GPIO_CFGLR = tmp;                                        \
} while (0)

#define GPIOA_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOA_CFGLR, pins, mode)
#define GPIOC_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOC_CFGLR, pins, mode)
#define GPIOD_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOD_CFGLR, pins, mode)