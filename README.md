# CH32V003 TRUE MINIMAL Library

A **header-only**, **bare-metal** register library for the CH32V003 microcontroller.  
It provides direct access to core peripherals (RCC, GPIO, AFIO, EXTI, PFIC) with no dependencies, runtime overhead, or unnecessary abstraction.

Designed for full control and minimalism — perfect for low-level development, education, or custom embedded projects.

---

## Features

- Pure C macros for direct hardware access  
- No startup code or runtime dependency  
- Works out of the box with `riscv64-unknown-elf-gcc`  
- Compatible with WCH CH32V003 and similar RISC-V MCUs  
- Fully permissive license (do whatever you want)

---

## Structure

```
CH32V003_TRUE_MINIMAL/
│
├── include/
│   ├── reg_access.h
│   ├── rcc.h
│   ├── gpio.h
│   ├── afio.h
│   ├── exti.h
│   ├── pfic.h
│   └── irq.h
│
└── ch32v003.h     # Include this to access everything
```

---

## Usage

Simply include the main header:

```c
#include "ch32v003.h"
```

This gives you access to register macros for clock control, GPIO, EXTI, AFIO, and PFIC.

You can then write bare-metal firmware directly in C without any HAL or SDK.

---

## Example

Below is a simplified example based on the [MTT16 project](https://github.com/Enderjelen11/MTT16).

```c
#include "ch32v003.h"
#include <stdint.h>

volatile uint8_t segmentCtr;

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("machine")));
void EXTI7_0_IRQHandler(void)
{
    // Example interrupt logic
    EXTI_CLEAR(5);
    segmentCtr++;
}

int main(void)
{
    /* Enable GPIO and AFIO clocks */
    RCC_ENABLE_ALL_GPIO();
    RCC_APB2PCENR |= RCC_AFIOEN;

    /* Configure GPIO pins */
    GPIOA_CONFIG(0x07, GPIO_MODE_OUT_PP_50MHZ);
    GPIOC_CONFIG(PIN(5), GPIO_MODE_IN_PUPD);
    GPIOC_BSHR = PIN(5); // Pull-up

    /* Setup EXTI */
    SETUP_EXTI_PIN(5, AFIO_EXTICR_PC, 1, 1, EXTI7_0_IRQn);
    ENABLE_GLOBAL_IRQS();

    while (1) {
        // Your main loop here
    }
}
```

---

## Building

simply include the **make.mk** file in your Makefile like so:

```Makefile
TARGET=display_block

SRC = main.c

include ../CH32V003_TRUE_MINIMAL/make.mk
```

then run:
```bash
make
```

All the build files should be in the build dir. Make automatically runs [wlink](https://github.com/ch32-rs/wlink) and tries to upload your code. 

---

## License

This library is released under a **do-anything** license.  
You can copy, modify, redistribute, and use it for any purpose without restriction or attribution.

---

## Notes

- Intended for developers who prefer **complete control** and **no abstraction layers**.  
- Reference the CH32V003 datasheet and reference manual for detailed register descriptions.  
- Verified with the MTT16 project, but adaptable to any CH32V003 application.
