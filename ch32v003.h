#pragma once
#include "reg_access.h"
#include "rcc.h"
#include "gpio.h"
#include "afio.h"
#include "exti.h"
#include "pfic.h"
#include "irq.h"

/* Composite helper for EXTI setup */
#define SETUP_EXTI_PIN(pin, portcode, rise, fall, irqnum) do { \
	AFIO_CONFIG_EXTI((pin), (portcode));                      \
	if (rise) EXTI_RTENR |= (1U << (pin));                    \
	if (fall) EXTI_FTENR |= (1U << (pin));                    \
	EXTI_INTENR |= (1U << (pin));                             \
	PFIC_IENR0 = (1U << (irqnum));                            \
} while (0)