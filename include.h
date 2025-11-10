#include <stdint.h>

#define REG(x) (*((volatile uint32_t *)(x)))

#define RCC_BASE       0x40021000
#define RCC_APB2PCENR  REG(RCC_BASE + 0x18)

#define RCC_IOPAEN     (1 << 2)
#define RCC_IOPCEN     (1 << 4)
#define RCC_IOPDEN     (1 << 5)
#define RCC_AFIOEN     (1 << 0)

#define RCC_ENABLE_GPIOA() (RCC_APB2PCENR |= RCC_IOPAEN)
#define RCC_ENABLE_GPIOC() (RCC_APB2PCENR |= RCC_IOPCEN)
#define RCC_ENABLE_GPIOD() (RCC_APB2PCENR |= RCC_IOPDEN)
#define RCC_ENABLE_ALL_GPIO() (RCC_APB2PCENR |= (RCC_IOPAEN | RCC_IOPCEN | RCC_IOPDEN))

#define GPIOA_BASE 0x40010800
#define GPIOC_BASE 0x40011000
#define GPIOD_BASE 0x40011400

#define GPIO_CFGLR_OFFSET 0x00
#define GPIO_OUTDR_OFFSET 0x0C
#define GPIO_BSHR_OFFSET  0x10

#define GPIOA_CFGLR REG(GPIOA_BASE + GPIO_CFGLR_OFFSET)
#define GPIOC_CFGLR REG(GPIOC_BASE + GPIO_CFGLR_OFFSET)
#define GPIOD_CFGLR REG(GPIOD_BASE + GPIO_CFGLR_OFFSET)

#define GPIOA_OUTDR REG(GPIOA_BASE + GPIO_OUTDR_OFFSET)
#define GPIOC_OUTDR REG(GPIOC_BASE + GPIO_OUTDR_OFFSET)
#define GPIOD_OUTDR REG(GPIOD_BASE + GPIO_OUTDR_OFFSET)

#define GPIOA_BSHR  REG(GPIOA_BASE + GPIO_BSHR_OFFSET)
#define GPIOC_BSHR  REG(GPIOC_BASE + GPIO_BSHR_OFFSET)
#define GPIOD_BSHR  REG(GPIOD_BASE + GPIO_BSHR_OFFSET)

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

#define PIN(n) (1U << ((uint8_t)(n)))

#define AFIO_BASE        0x40010000UL
#define AFIO_EXTICR_REG  REG(AFIO_BASE + 0x08)   

#define AFIO_EXTICR_PA  0x0
#define AFIO_EXTICR_PC  0x2
#define AFIO_EXTICR_PD  0x3

#define EXTI_BASE        0x40010400UL
#define EXTI_INTENR      REG(EXTI_BASE + 0x00)  
#define EXTI_RTENR       REG(EXTI_BASE + 0x08)  
#define EXTI_FTENR       REG(EXTI_BASE + 0x0C)  
#define EXTI_SWIEVR      REG(EXTI_BASE + 0x10)  
#define EXTI_INTFR       REG(EXTI_BASE + 0x14)  

#define PFIC_BASE        0xE000E000UL
#define PFIC_IENR0       REG(PFIC_BASE + 0x100)  
#define PFIC_IRER0       REG(PFIC_BASE + 0x180)  

#define EXTI7_0_IRQn     20

#define AFIO_CONFIG_EXTI(pin, portcode) do {               \
    uint32_t tmp = AFIO_EXTICR_REG;                          \
                        \
    tmp &= ~(0x3U << ((pin) * 2));                           \
                              \
    tmp |= ((uint32_t)((portcode) & 0x3U) << ((pin) * 2));   \
    AFIO_EXTICR_REG = tmp;                                   \
} while (0)

#define EXTI_ENABLE(line)        (EXTI_INTENR |= (1U << (line)))

#define EXTI_DISABLE(line)       (EXTI_INTENR &= ~(1U << (line)))

#define EXTI_TRIGGER_RISING(line)   (EXTI_RTENR |= (1U << (line)))
#define EXTI_TRIGGER_FALLING(line)  (EXTI_FTENR |= (1U << (line)))
#define EXTI_TRIGGER_BOTH(line)     do {                         \
    EXTI_RTENR |= (1U << (line));                                \
    EXTI_FTENR |= (1U << (line));                                \
} while (0)

#define EXTI_CLEAR(line)         (EXTI_INTFR = (1U << (line)))

#define EXTI_SW_TRIGGER(line)    (EXTI_SWIEVR |= (1U << (line)))

#define PFIC_ENABLE_IRQ(n)       (PFIC_IENR0 |= (1U << (n)))

#define PFIC_DISABLE_IRQ(n)      (PFIC_IRER0 |= (1U << (n)))

#define ENABLE_GLOBAL_IRQS() do {                 \
    __asm__ volatile ("li t0, 0x188");  \
    __asm__ volatile ("csrw mstatus, t0");        \
    __asm__ volatile ("li t0, 0x3");    \
    __asm__ volatile ("csrw mtvec, t0");          \
} while (0)

#define SETUP_EXTI_PIN(pin, portcode, rise, fall, irqnum) do { \
    AFIO_CONFIG_EXTI((pin), (portcode));                         \
    if (rise) EXTI_RTENR |= (1U << (pin));                          \
    if (fall) EXTI_FTENR |= (1U << (pin));                          \
    EXTI_INTENR |= (1U << (pin));                                   \
     \
    PFIC_IENR0 = (1U << (irqnum));                                  \
} while (0)
