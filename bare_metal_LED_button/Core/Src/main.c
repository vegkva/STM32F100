#include "main.h"
#include <stdint.h>
#include <stdbool.h>


#define RCC_BASE        0x40021000
#define RCC_APB2ENR     *((volatile uint32_t *)(RCC_BASE + 0x18)) // Page 92 datasheet (6.3.7 APB2 peripheral clock enable register (RCC_APB2ENR))
#define GPIOA_BASE      0x40010800
#define GPIOA_IDR       *((volatile uint32_t *)(GPIOA_BASE + 0x08)) // Page 114 datasheet (7.2.3 Port input data register (GPIOx_IDR) (x=A..G))
#define GPIOC_BASE      0x40011000
#define GPIOC_CRH       *((volatile uint32_t *)(GPIOC_BASE + 0x04)) // Paget 114 datasheet (7.2.2 Port configuration register high (GPIOx_CRH) (x=A..G))
#define GPIOC_BSRR      *((volatile uint32_t *)(GPIOC_BASE + 0x10)) // Page 115  datasheet (7.2.5 Port bit set/reset register (GPIOx_BSRR) (x=A..G))

#define RCC_APB2ENR_IOPAEN  (1 << 2) // Enable clock for GPIOA
#define RCC_APB2ENR_IOPCEN  (1 << 4) // Enable clock for GPIOC
#define GPIO_CRH_MODE8_0    (1 << 0) // Set PC8 as output mode
#define GPIO_CRH_MODE9_0    (1 << 4) // Set PC8 as output mode


//int count = 0;


int main(void) {
	HAL_Init();

	bool flash;
    // Enable clock for GPIOA and GPIOC
    RCC_APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN);

    // Configure PC8 as output (LED) and PA0 as input (Button)
    GPIOC_CRH &= ~((0xF << 0) | (0xF << 4)); // Clear mode and configuration for PC8 and PC9
    GPIOC_CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_0); // Set PC8 and PC9 as output
    // PA0 is input by default, so no need to explicitly set it

    while (1) {
        if (GPIOA_IDR & 0x01) { // Check if the button (PA0) is pressed
        	//count ++;
        	flash = true;
        	GPIOC_BSRR = (1 << 8) | (1 << 25);  // Turn on the blue LED (PC8) and off the green LED (PC9)
        }

		if (flash) {
			GPIOC_BSRR = (1 << 24) | (1 << 9);
			HAL_Delay(50);
			GPIOC_BSRR = (1 << 8) | (1 << 25);// Turn off the blue LED (PC8) and on the green LED (PC9)
			HAL_Delay(50);
			flash = false;
			GPIOC_BSRR = (1 << 24) | (1 << 25);
		}


    }
}


