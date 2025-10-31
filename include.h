#include <stdint.h>

/* ----------- Register Access Macro ----------- */
#define REG(x) (*((volatile uint32_t *)(x)))

/* ===================== RCC REGISTER DEFINITIONS ===================== */

#define RCC_BASE       0x40021000
#define RCC_APB2PCENR  REG(RCC_BASE + 0x18)

/* Bit definitions for RCC_APB2PCENR */
#define RCC_IOPAEN     (1 << 2)
#define RCC_IOPCEN     (1 << 4)
#define RCC_IOPDEN     (1 << 5)
#define RCC_AFIOEN     (1 << 0)

/* Macro to enable GPIO peripheral clocks */
#define RCC_ENABLE_GPIOA() (RCC_APB2PCENR |= RCC_IOPAEN)
#define RCC_ENABLE_GPIOC() (RCC_APB2PCENR |= RCC_IOPCEN)
#define RCC_ENABLE_GPIOD() (RCC_APB2PCENR |= RCC_IOPDEN)
#define RCC_ENABLE_ALL_GPIO() (RCC_APB2PCENR |= (RCC_IOPAEN | RCC_IOPCEN | RCC_IOPDEN))

/* ----------- Base Addresses ----------- */
#define GPIOA_BASE 0x40010800
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400

/* ----------- Register Offsets ----------- */
#define GPIO_CFGLR_OFFSET 0x00
#define GPIO_OUTDR_OFFSET 0x0C
#define GPIO_BSHR_OFFSET  0x10

/* ----------- Register Access ----------- */
#define GPIOA_CFGLR REG(GPIOA_BASE + GPIO_CFGLR_OFFSET)
#define GPIOC_CFGLR REG(GPIOC_BASE + GPIO_CFGLR_OFFSET)
#define GPIOD_CFGLR REG(GPIOD_BASE + GPIO_CFGLR_OFFSET)

#define GPIOA_OUTDR REG(GPIOA_BASE + GPIO_OUTDR_OFFSET)
#define GPIOC_OUTDR REG(GPIOC_BASE + GPIO_OUTDR_OFFSET)
#define GPIOD_OUTDR REG(GPIOD_BASE + GPIO_OUTDR_OFFSET)

#define GPIOA_BSHR  REG(GPIOA_BASE + GPIO_BSHR_OFFSET)
#define GPIOC_BSHR  REG(GPIOC_BASE + GPIO_BSHR_OFFSET)
#define GPIOD_BSHR  REG(GPIOD_BASE + GPIO_BSHR_OFFSET)

/* ============================================================
   GPIO MODES (4-bit field per pin)
   Compatible with WCH / STM32 CFGLR bit structure
   ============================================================ */

/* Input Modes */
#define GPIO_MODE_IN_ANALOG       0b0000  /* Analog input */
#define GPIO_MODE_IN_FLOATING     0b0100  /* Floating input */
#define GPIO_MODE_IN_PUPD         0b1000  /* Input with pull-up/pull-down */

/* Output Push-Pull */
#define GPIO_MODE_OUT_PP_10MHZ    0b0001
#define GPIO_MODE_OUT_PP_2MHZ     0b0010
#define GPIO_MODE_OUT_PP_50MHZ    0b0011

/* Output Open-Drain */
#define GPIO_MODE_OUT_OD_10MHZ    0b0101
#define GPIO_MODE_OUT_OD_2MHZ     0b0110
#define GPIO_MODE_OUT_OD_50MHZ    0b0111

/* Alternate Function Push-Pull */
#define GPIO_MODE_AF_PP_10MHZ     0b1001
#define GPIO_MODE_AF_PP_2MHZ      0b1010
#define GPIO_MODE_AF_PP_50MHZ     0b1011

/* Alternate Function Open-Drain */
#define GPIO_MODE_AF_OD_10MHZ     0b1101
#define GPIO_MODE_AF_OD_2MHZ      0b1110
#define GPIO_MODE_AF_OD_50MHZ     0b1111

/* ============================================================
   CONFIGURATION MACROS
   ============================================================ */

/* Internal bit helper */
#define GPIO_MODE_BITS(pin, mode) ((uint32_t)(mode) << ((pin) * 4))

/* Configure selected pins on a given port */
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

/* Convenience per-port macros */
#define GPIOA_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOA_CFGLR, pins, mode)
#define GPIOC_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOC_CFGLR, pins, mode)
#define GPIOD_CONFIG(pins, mode) GPIO_CONFIG_PORT(GPIOD_CFGLR, pins, mode)

#define PIN(n) (1U << ((uint8_t)(n)))






/* ===================== AFIO REGISTER DEFINITIONS ===================== */
#define AFIO_BASE        0x40010000
#define AFIO_EXTICR      REG(AFIO_BASE + 0x08)  /* External interrupt config */

/* Map GPIO port (A-D) to EXTI line 0–7 */
#define AFIO_EXTICR_PA   0x0
#define AFIO_EXTICR_PC   0x2
#define AFIO_EXTICR_PD   0x3

/* Macro to connect given pin (0–7) to a specific port */
#define AFIO_CONFIG_EXTI(pin, portcode) do {                     \
    uint32_t tmp = AFIO_EXTICR;                                  \
    tmp &= ~(0xF << ((pin) * 4));                                \
    tmp |= ((uint32_t)(portcode & 0xF) << ((pin) * 4));          \
    AFIO_EXTICR = tmp;                                           \
} while (0)

/* ===================== EXTI REGISTER DEFINITIONS ===================== */
#define EXTI_BASE        0x40010400
#define EXTI_INTENR      REG(EXTI_BASE + 0x00)  /* Interrupt enable */
#define EXTI_EVENR       REG(EXTI_BASE + 0x04)  /* Event enable */
#define EXTI_RTENR       REG(EXTI_BASE + 0x08)  /* Rising trigger enable */
#define EXTI_FTENR       REG(EXTI_BASE + 0x0C)  /* Falling trigger enable */
#define EXTI_SWIEVR      REG(EXTI_BASE + 0x10)  /* Software interrupt trigger */
#define EXTI_INTFR       REG(EXTI_BASE + 0x14)  /* Interrupt pending flag */

/* ============================================================
   EXTI CONFIGURATION MACROS
   ============================================================ */

/* Enable EXTI line interrupt */
#define EXTI_ENABLE(line)        (EXTI_INTENR |= (1U << (line)))
/* Disable EXTI line interrupt */
#define EXTI_DISABLE(line)       (EXTI_INTENR &= ~(1U << (line)))

/* Enable trigger edge */
#define EXTI_TRIGGER_RISING(line)   (EXTI_RTENR |= (1U << (line)))
#define EXTI_TRIGGER_FALLING(line)  (EXTI_FTENR |= (1U << (line)))
#define EXTI_TRIGGER_BOTH(line)     do { EXTI_RTENR |= (1U << (line)); EXTI_FTENR |= (1U << (line)); } while (0)

/* Clear pending flag */
#define EXTI_CLEAR(line)         (EXTI_INTFR = (1U << (line)))

/* Software trigger */
#define EXTI_SW_TRIGGER(line)    (EXTI_SWIEVR |= (1U << (line)))
