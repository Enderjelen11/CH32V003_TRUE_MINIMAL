#pragma once
#include "reg_access.h"

/* -------- External Interrupt Controller (EXTI) -------- */
#define EXTI_BASE        0x40010400UL
#define EXTI_INTENR      REG(EXTI_BASE + 0x00)
#define EXTI_RTENR       REG(EXTI_BASE + 0x08)
#define EXTI_FTENR       REG(EXTI_BASE + 0x0C)
#define EXTI_SWIEVR      REG(EXTI_BASE + 0x10)
#define EXTI_INTFR       REG(EXTI_BASE + 0x14)

#define EXTI_ENABLE(line)        (EXTI_INTENR |= (1U << (line)))
#define EXTI_DISABLE(line)       (EXTI_INTENR &= ~(1U << (line)))

#define EXTI_TRIGGER_RISING(line)   (EXTI_RTENR |= (1U << (line)))
#define EXTI_TRIGGER_FALLING(line)  (EXTI_FTENR |= (1U << (line)))
#define EXTI_TRIGGER_BOTH(line)     do { \
	EXTI_RTENR |= (1U << (line));        \
	EXTI_FTENR |= (1U << (line));        \
} while (0)

#define EXTI_CLEAR(line)         (EXTI_INTFR = (1U << (line)))
#define EXTI_SW_TRIGGER(line)    (EXTI_SWIEVR |= (1U << (line)))