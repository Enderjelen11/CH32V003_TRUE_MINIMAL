#pragma once
#include "reg_access.h"

/* -------- Alternate Function I/O (AFIO) -------- */
#define AFIO_BASE        0x40010000UL
#define AFIO_EXTICR_REG  REG(AFIO_BASE + 0x08)

/* Port mapping codes */
#define AFIO_EXTICR_PA  0x0
#define AFIO_EXTICR_PC  0x2
#define AFIO_EXTICR_PD  0x3

#define AFIO_CONFIG_EXTI(pin, portcode) do {                \
	uint32_t tmp = AFIO_EXTICR_REG;                          \
	tmp &= ~(0x3U << ((pin) * 2));                           \
	tmp |= ((uint32_t)((portcode) & 0x3U) << ((pin) * 2));   \
	AFIO_EXTICR_REG = tmp;                                   \
} while (0)

