/**
 * @file    main.c
 * @brief   GPIO Interrupt Validation Application
 *
 * @details This application tests the GPIO driver using external interrupts (EXTI).
 * It configures a button to trigger a falling-edge interrupt, which then toggles an LED.
 *
 * APIs Used:
 * - Initialization: GPIO_PeriphClockControl(), GPIO_Init()
 * - NVIC/Core: NVIC_IRQPriorityConfig(), NVIC_IRQInterruptConfig()
 * - Interrupt Handling: GPIO_IRQHandling(), GPIO_ToggleOutputPin()
 *
 * Usage Flow:
 * 1. Hardware Setup: LED connected to PA5, Button connected to PC13.
 * 2. Configure GPIO structures for Output (LED) and Interrupt Falling Edge (Button).
 * 3. Enable the specific EXTI IRQ line in the ARM Cortex-M4 NVIC.
 * 4. The CPU idles in the main loop while the ISR handles the hardware triggers.
 */

#include <string.h>
#include "stm32f401re.h"
#include "core_cm4.h"
#include "stm32f401re_gpio_driver.h"

#define BTN_PRESSED 0

void delay(void) {
    for (uint32_t i = 0U; i < 250000U; i++);
}

int main(void) {
    GPIO_Handle_t GpioLed, GpioBtn;

    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GpioBtn, 0, sizeof(GpioBtn));

    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriphClockControl(GPIOA, ENABLE);
    GPIO_Init(&GpioLed);

    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_PeriphClockControl(GPIOC, ENABLE);
    GPIO_Init(&GpioBtn);

    NVIC_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15U);
    NVIC_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

    while(1) {
        // Idle
    }

    return 0;
}

void EXTI15_10_IRQHandler(void) {
    delay();
    GPIO_IRQHandling(GPIO_PIN_NO_13);
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
