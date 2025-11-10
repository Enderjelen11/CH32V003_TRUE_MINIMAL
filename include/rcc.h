#pragma once
#include "reg_access.h"

/* -------- Reset and Clock Control (RCC) -------- */
#define RCC_BASE       0x40021000
#define RCC_APB2PCENR  REG(RCC_BASE + 0x18)

/* APB2 peripheral clock enable bits */
#define RCC_IOPAEN     (1 << 2)
#define RCC_IOPCEN     (1 << 4)
#define RCC_IOPDEN     (1 << 5)
#define RCC_AFIOEN     (1 << 0)

/* Enable peripheral clocks */
#define RCC_ENABLE_GPIOA() (RCC_APB2PCENR |= RCC_IOPAEN)
#define RCC_ENABLE_GPIOC() (RCC_APB2PCENR |= RCC_IOPCEN)
#define RCC_ENABLE_GPIOD() (RCC_APB2PCENR |= RCC_IOPDEN)
#define RCC_ENABLE_ALL_GPIO() (RCC_APB2PCENR |= (RCC_IOPAEN | RCC_IOPCEN | RCC_IOPDEN))
