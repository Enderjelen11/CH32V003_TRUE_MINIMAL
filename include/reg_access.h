#pragma once
#include <stdint.h>

/* -------- Register access macro -------- */
#define REG(regAddres) (*((volatile uint32_t *)(regAddres)))
