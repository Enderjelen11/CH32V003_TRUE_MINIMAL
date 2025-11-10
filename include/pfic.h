#pragma once
#include "reg_access.h"

/* -------- Programmable Fast Interrupt Controller (PFIC) -------- */
#define PFIC_BASE        0xE000E000UL
#define PFIC_IENR0       REG(PFIC_BASE + 0x100)
#define PFIC_IRER0       REG(PFIC_BASE + 0x180)

#define PFIC_ENABLE_IRQ(n)  (PFIC_IENR0 |= (1U << (n)))
#define PFIC_DISABLE_IRQ(n) (PFIC_IRER0 |= (1U << (n)))

#define ENABLE_GLOBAL_IRQS() do {                 \
	__asm__ volatile ("li t0, 0x188");            \
	__asm__ volatile ("csrw mstatus, t0");        \
	__asm__ volatile ("li t0, 0x3");              \
	__asm__ volatile ("csrw mtvec, t0");          \
} while (0)

