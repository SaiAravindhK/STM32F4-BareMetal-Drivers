/**
 * @file    main.c
 * @brief   SPI Driver Validation Application
 *
 * @details This application tests the SPI driver using both blocking (polling)
 * and non-blocking (interrupt-driven) full-duplex communication.
 *
 * APIs Used:
 * - Initialization: SPI2_GPIOInits(), SPI2_Inits()
 * - Polling: SPI_SendData(), SPI_ReceiveData()
 * - Interrupts: SPI_SendDataIT(), SPI_ReceiveDataIT()
 * - ISR/Callbacks: SPI_IRQHandling(), SPI_ApplicationEventCallback()
 *
 * Usage Flow:
 * 1. Configure GPIO alternate functions and SPI peripheral settings.
 * 2. Execute Polling Test (CPU blocks until transfer completes).
 * 3. Execute Interrupt Test (Receiver pre-armed, CPU idles while ISR handles transfer).
 */

#include <string.h>
#include <stdint.h>
#include "stm32f401re.h"
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_spi_driver.h"


SPI_Handle_t SPI2Handle;

volatile uint8_t txComplete = 0U;
volatile uint8_t rxComplete = 0U;

void SPI2_GPIOInits(void) {
    GPIO_Handle_t SPIPins;
    memset(&SPIPins, 0, sizeof(SPIPins));

    GPIO_PeriphClockControl(GPIOB, ENABLE);

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5U;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&SPIPins);

    // MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&SPIPins);

    // NSS
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&SPIPins);

    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
}

void SPI2_Inits(void) {
    memset(&SPI2Handle, 0, sizeof(SPI2Handle));

    SPI2Handle.pSPIx = SPI2;
    SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;
    SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256;

    SPI_PeriphClockControl(SPI2, ENABLE);
    SPI_Init(&SPI2Handle);
}

int main(void) {
    uint8_t dummy_payload[] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t rx_buffer[4] = {0};

    uint8_t int_payload[] = {0xCA, 0xFE, 0xBA, 0xBE};
    uint8_t int_rx_buffer[4] = {0};

    SPI2_GPIOInits();
    SPI2_Inits();

    NVIC_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);
    SPI_SSIConfig(SPI2, ENABLE);

    /* --- TEST 1: POLLING MODE --- */
    SPI_PeripheralControl(SPI2, ENABLE);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_RESET);

    for (uint8_t i = 0U; i < sizeof(dummy_payload); i++) {
        SPI_SendData(SPI2, &dummy_payload[i], 1U);
        SPI_ReceiveData(SPI2, &rx_buffer[i], 1U);
    }

    while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) == FLAG_SET);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
    SPI_PeripheralControl(SPI2, DISABLE);


    /* --- TEST 2: INTERRUPT MODE --- */
    txComplete = 0U;
    rxComplete = 0U;

    SPI_PeripheralControl(SPI2, ENABLE);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_RESET);

    SPI_ReceiveDataIT(&SPI2Handle, int_rx_buffer, sizeof(int_payload));
    SPI_SendDataIT(&SPI2Handle, int_payload, sizeof(int_payload));

    while(txComplete == 0U || rxComplete == 0U);

    while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) == FLAG_SET);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
    SPI_PeripheralControl(SPI2, DISABLE);

    while (1);

    return 0;
}

void SPI2_IRQHandler(void) {
    SPI_IRQHandling(&SPI2Handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {
    if (AppEv == SPI_EVENT_TX_CMPLT) {
        txComplete = 1U;
    } else if (AppEv == SPI_EVENT_RX_CMPLT) {
        rxComplete = 1U;
    }
}
