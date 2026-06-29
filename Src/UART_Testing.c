/**
 * @file    main.c
 * @brief   USART Driver Validation Application
 *
 * @details This application tests the USART driver using both blocking (polling)
 * and non-blocking (interrupt-driven) full-duplex communication.
 *
 * APIs Used:
 * - Initialization: USART1_GPIOInit(), USART1_Init()
 * - Polling: USART_SendData(), USART_ReceiveData()
 * - Interrupts: USART_SendDataIT(), USART_ReceiveDataIT()
 * - ISR: USART1_IRQHandler()
 *
 * Usage Flow:
 * 1. Hardware Setup: Connect STM32 PA9 (TX) to target RX, and STM32 PA10 (RX) to target TX.
 * 2. Configure GPIO alternate functions and USART peripheral settings.
 * 3. Execute Polling Test (CPU blocks until transfer completes).
 * 4. Execute Interrupt Test (Receiver pre-armed, CPU idles while ISR handles transfer).
 */

#include <string.h>
#include <stdint.h>
#include "stm32f401re.h"
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_usart_driver.h"

uint8_t msg[] = {0xAB, 0xCD, 0xEF};
uint32_t length = sizeof(msg) / sizeof(msg[0]);
uint8_t rcv_buff[3] = {0};
uint8_t it_rcv_buff[3] = {0};

USART_Handle_t usart1_handle;

void USART1_Init(void) {
    usart1_handle.pUSARTx = USART1;
    usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    usart1_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart1_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart1_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(&usart1_handle);
}

void USART1_GPIOInit(void) {
    GPIO_Handle_t usart_gpios;
    memset(&usart_gpios, 0, sizeof(usart_gpios));

    GPIO_PeriphClockControl(GPIOA, ENABLE);

    usart_gpios.pGPIOx = GPIOA;
    usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7U;

    // USART1 TX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&usart_gpios);

    // USART1 RX
    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    GPIO_Init(&usart_gpios);
}

void delay(void) {
    for(uint32_t i = 0U; i < 100000U; i++);
}

int main(void) {
    USART1_GPIOInit();
    USART1_Init();

    USART_PeripheralControl(USART1, ENABLE);
    NVIC_IRQInterruptConfig(IRQ_NO_USART1, ENABLE);

    /* --- TEST 1: POLLING MODE --- */
    USART_SendData(&usart1_handle, msg, length);
    USART_ReceiveData(&usart1_handle, rcv_buff, length);

    delay();
    delay();

    /* --- TEST 2: INTERRUPT MODE --- */
    USART_ReceiveDataIT(&usart1_handle, it_rcv_buff, length);
    USART_SendDataIT(&usart1_handle, msg, length);

    while(usart1_handle.TxBusyState == USART_BUSY_IN_TX || usart1_handle.RxBusyState == USART_BUSY_IN_RX);

    while(1) {
        // Idle
    }

    return 0;
}

void USART1_IRQHandler(void) {
    USART_IRQHandling(&usart1_handle);
}
