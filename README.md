# CH32V003 Truly Minimal Register Library


A **zero-overhead**, **bare-metal** register-level library for the **WCH CH32V003** microcontroller.  
This header provides direct register definitions and macros for **GPIO**, **RCC**, **AFIO**, and **EXTI**,  
allowing you to write low-level firmware without any SDK or function call overhead.

CH32V003 has little flash, unfortunately the popular ch32v003_fun lib uses a lot of it
for stuff that user may not need. I've menaged to strip 2kb projects made with the upper mentioned
lib to something like 600 bytes just by migrating to this lib.

Another problem are the licences which enforce the user to make his code open source. This library
has no strings attached, so do whatever you want with it. 

---

##  Features

-  **Pure register access** — no functions, no flash usage  
-  **Full GPIO control** (configuration, output, BSHR access)  
-  **RCC peripheral clock enable macros**  
-  **AFIO EXTI mapping support**  
-  **EXTI interrupt configuration (rising/falling edge, clear flags, software trigger)**  
-  Fully self-contained — just include one header file  

---

##  File structure

```
include.h   # Main header file (include this in your project)
```

---

##  Quick Start

### 1. Include the header
```c
#include "include.h"
```

### 2. Enable peripheral clocks
```c
RCC_ENABLE_ALL_GPIO();      // Enables GPIOA, GPIOC, GPIOD
RCC_APB2PCENR |= RCC_AFIOEN; // Enables AFIO (for EXTI mapping)
```

### 3. Configure GPIO pins
```c
// Configure PC0 as output (push-pull 50MHz)
GPIOC_CONFIG(PIN(0), GPIO_MODE_OUT_PP_50MHZ);

// Configure PC4 as input pull-up
GPIOC_CONFIG(PIN(4), GPIO_MODE_IN_PUPD);
GPIOC_BSHR = PIN(4); // enable pull-up (set OUTDR bit)
```

### 4. Setup EXTI interrupt (example on PC4)
```c
// Map EXTI4 line to port C
AFIO_CONFIG_EXTI(4, AFIO_EXTICR_PC);

// Enable falling edge trigger and interrupt
EXTI_TRIGGER_FALLING(4);
EXTI_ENABLE(4);
```

### 5. Example interrupt handler
```c
void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI7_0_IRQHandler(void)
{
    if (EXTI_INTFR & PIN(4)) {       // Check if PC4 triggered
        GPIOC_BSHR = (PIN(0) << 16); // Reset PC0
        EXTI_CLEAR(4);               // Clear interrupt flag
    }
}
```

---

##  Available Macros

### RCC (Clock Control)
| Macro | Description |
|-------|--------------|
| `RCC_ENABLE_GPIOA()` | Enable GPIOA clock |
| `RCC_ENABLE_GPIOC()` | Enable GPIOC clock |
| `RCC_ENABLE_GPIOD()` | Enable GPIOD clock |
| `RCC_ENABLE_ALL_GPIO()` | Enable all GPIO ports |
| `RCC_AFIOEN` | Enable AFIO peripheral clock |

---

### GPIO Configuration
| Macro | Description |
|--------|-------------|
| `GPIOA_CONFIG(pins, mode)` | Configure GPIOA pins |
| `GPIOC_CONFIG(pins, mode)` | Configure GPIOC pins |
| `GPIOD_CONFIG(pins, mode)` | Configure GPIOD pins |
| `PIN(n)` | Bitmask for pin number `n` |

**Modes (examples):**
```c
GPIO_MODE_OUT_PP_50MHZ    // Output push-pull, 50MHz
GPIO_MODE_IN_FLOATING     // Floating input
GPIO_MODE_IN_PUPD         // Input with pull-up/pull-down
GPIO_MODE_AF_PP_10MHZ     // Alternate function, push-pull
```

---

### EXTI (External Interrupts)
| Macro | Description |
|--------|-------------|
| `AFIO_CONFIG_EXTI(pin, portcode)` | Map EXTI line to a GPIO port |
| `EXTI_ENABLE(line)` | Enable interrupt on EXTI line |
| `EXTI_DISABLE(line)` | Disable interrupt on EXTI line |
| `EXTI_TRIGGER_RISING(line)` | Enable rising-edge trigger |
| `EXTI_TRIGGER_FALLING(line)` | Enable falling-edge trigger |
| `EXTI_TRIGGER_BOTH(line)` | Enable both edge triggers |
| `EXTI_CLEAR(line)` | Clear interrupt pending flag |
| `EXTI_SW_TRIGGER(line)` | Trigger interrupt in software |

**Port codes for AFIO:**
```c
AFIO_EXTICR_PA
AFIO_EXTICR_PC
AFIO_EXTICR_PD
```

---

##  Example: LED toggle on button press

```c
#include "include.h"

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI7_0_IRQHandler(void)
{
    if (EXTI_INTFR & PIN(4)) {
        GPIOC_BSHR = PIN(0) << 16; // Turn LED off
        EXTI_CLEAR(4);
    }
}

int main(void)
{
    RCC_ENABLE_ALL_GPIO();
    RCC_APB2PCENR |= RCC_AFIOEN;

    GPIOC_CONFIG(PIN(0), GPIO_MODE_OUT_PP_50MHZ); // LED
    GPIOC_CONFIG(PIN(4), GPIO_MODE_IN_PUPD);      // Button input
    GPIOC_BSHR = PIN(4); // Pull-up

    AFIO_CONFIG_EXTI(4, AFIO_EXTICR_PC);
    EXTI_TRIGGER_FALLING(4);
    EXTI_ENABLE(4);

    while (1) {
        GPIOC_BSHR = PIN(0);      // LED on
        for (volatile int i = 0; i < 50000; i++);
        GPIOC_BSHR = PIN(0) << 16; // LED off
        for (volatile int i = 0; i < 50000; i++);
    }
}
```

---

##  Philosophy

This library aims to:
- Provide **the absolute minimum layer** on top of registers  
- Avoid all runtime overhead (no C functions, structs, or variables)  
- Remain fully **transparent** — everything compiles to direct register operations  
- Be **human-readable**, matching the CH32V003 reference manual

---

##  Compatibility

- **MCU:** WCH CH32V003 (RISC-V)
- **Toolchain:** `riscv-none-elf-gcc` or `riscv64-unknown-elf-gcc` and [wlink](https://github.com/ch32-rs/wlink) 
- **C standard:** C99+
- **No dependencies**

---

##  License

Public domain / CC0 — use freely for any purpose.  
No attribution required, but (very much) appreciated .

Use it however you want, I don't care.

The software is given "as is" with no guarantee.
something breaks it's your fault, not mine.

---

**Author:** Kacper Jelonek  
**Version:** 0.9 — Truly minimal bare-metal CH32V003 register library
