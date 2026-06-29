/**
 * @file    main.c
 * @brief   I2C Driver Validation Application
 *
 * @details This application tests the I2C driver acting as a Master, using
 * both blocking (polling) and non-blocking (interrupt-driven) communication.
 *
 * APIs Used:
 * - Initialization: I2C1_GPIOInits(), I2C1_Inits(), GPIO_ButtonInit()
 * - Polling: I2C_MasterSendData(), I2C_MasterReceiveData()
 * - Interrupts: I2C_MasterSendDataIT(), I2C_MasterReceiveDataIT()
 * - ISR/Callbacks: I2C1_EV_IRQHandler(), I2C1_ER_IRQHandler(), I2C_ApplicationEventCallback()
 *
 * Usage Flow:
 * 1. Configure GPIO alternate functions for I2C1 (SCL = PB6, SDA = PB7).
 * 2. Configure I2C peripheral parameters (Speed, Duty Cycle, Address).
 * 3. Execute Polling TX/RX tests.
 * 4. Execute Interrupt TX/RX tests, polling the TxRxState to ensure sequence order.
 */

#include <string.h>
#include <stdint.h>
#include "stm32f401re.h"
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_i2c_driver.h"

#define MY_ADDR     0x61U
#define SLAVE_ADDR  0x68U

extern void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

I2C_Handle_t I2C1Handle;

uint8_t poll_tx_data[2] = {0xAA, 0x55};
uint8_t poll_rx_buf[2]  = {0};

uint8_t it_tx_data[2]   = {0xCC, 0x33};
uint8_t it_rx_buf[2]    = {0};

void delay(void) {
    for(uint32_t i = 0U; i < 100000U; i++);
}

void I2C1_GPIOInits(void) {
    GPIO_Handle_t I2CPins;
    memset(&I2CPins, 0, sizeof(I2CPins));

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4U;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GPIO_PeriphClockControl(GPIOB, ENABLE);

    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(void) {
    memset(&I2C1Handle, 0, sizeof(I2C1Handle));

    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
    I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_PeriphClockControl(I2C1, ENABLE);
    I2C_Init(&I2C1Handle);

    NVIC_IRQInterruptConfig(31U, ENABLE);
    NVIC_IRQInterruptConfig(32U, ENABLE);
}

void GPIO_ButtonInit(void) {
    GPIO_Handle_t GpioBtn;
    memset(&GpioBtn, 0, sizeof(GpioBtn));

    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriphClockControl(GPIOC, ENABLE);
    GPIO_Init(&GpioBtn);
}

int main(void) {
    GPIO_ButtonInit();
    I2C1_GPIOInits();
    I2C1_Inits();

    I2C_PeripheralControl(I2C1, ENABLE);
    I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

    while(1) {
        I2C_MasterSendData(&I2C1Handle, poll_tx_data, 2U, SLAVE_ADDR, I2C_DISABLE_SR);
        delay();

        I2C_MasterReceiveData(&I2C1Handle, poll_rx_buf, 2U, SLAVE_ADDR, I2C_DISABLE_SR);
        delay();

        while(I2C1Handle.TxRxState != I2C_READY);
        I2C_MasterSendDataIT(&I2C1Handle, it_tx_data, 2U, SLAVE_ADDR, I2C_DISABLE_SR);
        while(I2C1Handle.TxRxState != I2C_READY);
        delay();

        while(I2C1Handle.TxRxState != I2C_READY);
        I2C_MasterReceiveDataIT(&I2C1Handle, it_rx_buf, 2U, SLAVE_ADDR, I2C_DISABLE_SR);
        while(I2C1Handle.TxRxState != I2C_READY);
        delay();
    }

    return 0;
}

void I2C1_EV_IRQHandler(void) {
    I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void) {
    I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv) {
    if(AppEv == I2C_ERROR_AF) {
        I2C_CloseSendData(pI2CHandle);
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        while(1);
    }
}
