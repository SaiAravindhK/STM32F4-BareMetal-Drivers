#include "stm32f401re_i2c_driver.h"
#include "stm32f401re_rcc_driver.h"

/******************************************************************************/
/* Private Helper Function Prototypes                                         */
/******************************************************************************/
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);

/******************************************************************************/
/* I2C Driver API Implementations                                             */
/******************************************************************************/

/**
 * @brief  Returns the status of a specific I2C flag.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  FlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName) {
    if (pI2Cx->SR1 & FlagName) {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/**
 * @brief  Enables or disables peripheral clock for the given I2C peripheral.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_PeriphClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        if      (pI2Cx == I2C1) { I2C1_PCLK_EN(); }
        else if (pI2Cx == I2C2) { I2C2_PCLK_EN(); }
        else if (pI2Cx == I2C3) { I2C3_PCLK_EN(); }
    } else {
        if      (pI2Cx == I2C1) { I2C1_PCLK_DI(); }
        else if (pI2Cx == I2C2) { I2C2_PCLK_DI(); }
        else if (pI2Cx == I2C3) { I2C3_PCLK_DI(); }
    }
}

/**
 * @brief  Enables or disables the I2C peripheral (PE bit).
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        pI2Cx->CR1 |= (1U << I2C_CR1_PE);
    } else {
        pI2Cx->CR1 &= ~(1U << I2C_CR1_PE);
    }
}

/**
 * @brief  Initializes the given I2C peripheral according to the handle parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_Init(I2C_Handle_t *pI2CHandle) {
    uint32_t tempreg = 0;

    tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK);
    pI2CHandle->pI2Cx->CR1 = tempreg;

    tempreg = 0;
    tempreg = RCC_GetPCLK1Value() / 1000000U;
    pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3FU);

    tempreg = 0;
    tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1U;
    tempreg |= (1U << 14);
    pI2CHandle->pI2Cx->OAR1 = tempreg;

    uint16_t ccr_value = 0;
    tempreg = 0;
    if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
        ccr_value = RCC_GetPCLK1Value() / (2U * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        tempreg |= (ccr_value & 0xFFFU);
    } else {
        tempreg |= (1U << I2C_CCR_FS);
        tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);
        
        if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2) {
            ccr_value = RCC_GetPCLK1Value() / (3U * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        } else {
            ccr_value = RCC_GetPCLK1Value() / (25U * pI2CHandle->I2C_Config.I2C_SCLSpeed);
        }
        tempreg |= (ccr_value & 0xFFFU);
    }
    pI2CHandle->pI2Cx->CCR = tempreg;

    if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
        tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1U;
    } else {
        tempreg = ((RCC_GetPCLK1Value() * 300U) / 1000000U) + 1U;
    }
    pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3FU);
}

/**
 * @brief  De-initializes the given I2C peripheral.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @return None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx) {
    if      (pI2Cx == I2C1) { I2C1_REG_RESET(); }
    else if (pI2Cx == I2C2) { I2C2_REG_RESET(); }
    else if (pI2Cx == I2C3) { I2C3_REG_RESET(); }
}

/**
 * @brief  Master sends data over I2C in a blocking manner.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return None
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

    I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

    I2C_ClearADDRFlag(pI2CHandle);

    while (Len > 0) {
        while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
        pI2CHandle->pI2Cx->DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

    if (Sr == I2C_DISABLE_SR) {
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
    }
}

/**
 * @brief  Master receives data over I2C in a blocking manner.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return None
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

    I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

    if (Len == 1) {
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
        I2C_ClearADDRFlag(pI2CHandle);
        while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

        if (Sr == I2C_DISABLE_SR) {
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        }
        *pRxBuffer = pI2CHandle->pI2Cx->DR;
    }

    if (Len > 1) {
        I2C_ClearADDRFlag(pI2CHandle);

        for (uint32_t i = Len; i > 0; i--) {
            while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

            if (i == 2) {
                I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
                if (Sr == I2C_DISABLE_SR) {
                    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                }
            }

            *pRxBuffer = pI2CHandle->pI2Cx->DR;
            pRxBuffer++;
        }
    }

    if (pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE) {
        I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
    }
}

/**
 * @brief  Master sends data over I2C in a non-blocking manner (interrupt mode).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return Current application state.
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
    uint8_t busystate = pI2CHandle->TxRxState;

    if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
        pI2CHandle->pTxBuffer = pTxBuffer;
        pI2CHandle->TxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
        pI2CHandle->DevAddr = SlaveAddr;
        pI2CHandle->Sr = Sr;

        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITBUFEN);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITEVTEN);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITERREN);
    }
    return busystate;
}

/**
 * @brief  Master receives data over I2C in a non-blocking manner (interrupt mode).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return Current application state.
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr) {
    uint8_t busystate = pI2CHandle->TxRxState;

    if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
        pI2CHandle->pRxBuffer = pRxBuffer;
        pI2CHandle->RxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
        pI2CHandle->RxSize = Len;
        pI2CHandle->DevAddr = SlaveAddr;
        pI2CHandle->Sr = Sr;

        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITBUFEN);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITEVTEN);
        pI2CHandle->pI2Cx->CR2 |= (1U << I2C_CR2_ITERREN);
    }
    return busystate;
}

/**
 * @brief  Handles the TXE interrupt.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle) {
    if (pI2CHandle->TxLen > 0) {
        pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);
        pI2CHandle->TxLen--;
        pI2CHandle->pTxBuffer++;
    }
}

/**
 * @brief  Handles the RXNE interrupt.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle) {
    if (pI2CHandle->RxSize == 1) {
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
        pI2CHandle->RxLen--;
    }

    if (pI2CHandle->RxSize > 1) {
        if (pI2CHandle->RxLen == 2) {
            I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
        }
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
        pI2CHandle->pRxBuffer++;
        pI2CHandle->RxLen--;
    }

    if (pI2CHandle->RxLen == 0) {
        if (pI2CHandle->Sr == I2C_DISABLE_SR) {
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        }

        I2C_CloseReceiveData(pI2CHandle);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
    }
}

/**
 * @brief  Closes active I2C reception and resets Rx parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle) {
    pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pRxBuffer = 0;
    pI2CHandle->RxLen = 0;
    pI2CHandle->RxSize = 0;

    if (pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE) {
        I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
    }
}

/**
 * @brief  Closes active I2C transmission and resets Tx parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle) {
    pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->CR2 &= ~(1U << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pTxBuffer = 0;
    pI2CHandle->TxLen = 0;
}

/**
 * @brief  Handles the I2C event interrupt (EV_IRQ).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle) {
    uint32_t temp1, temp2, temp3;

    temp1 = pI2CHandle->pI2Cx->CR2 & (1U << I2C_CR2_ITEVTEN);
    temp2 = pI2CHandle->pI2Cx->CR2 & (1U << I2C_CR2_ITBUFEN);
    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_SB);
    
    if (temp1 && temp3) {
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
            I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
        } else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
            I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
        }
    }

    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_ADDR);
    if (temp1 && temp3) {
        I2C_ClearADDRFlag(pI2CHandle);
    }

    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_BTF);
    if (temp1 && temp3) {
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
            if (pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_TXE)) {
                if (pI2CHandle->TxLen == 0) {
                    if (pI2CHandle->Sr == I2C_DISABLE_SR) {
                        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                    }
                    I2C_CloseSendData(pI2CHandle);
                    I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
                }
            }
        } else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
            ;
        }
    }

    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_STOPF);
    if (temp1 && temp3) {
        pI2CHandle->pI2Cx->CR1 |= 0x0000;
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
    }

    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_TXE);
    if (temp1 && temp2 && temp3) {
        if (pI2CHandle->pI2Cx->SR2 & (1U << I2C_SR2_MSL)) {
            if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX) {
                I2C_MasterHandleTXEInterrupt(pI2CHandle);
            }
        } else {
            if (pI2CHandle->pI2Cx->SR2 & (1U << I2C_SR2_TRA)) {
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
            }
        }
    }

    temp3 = pI2CHandle->pI2Cx->SR1 & (1U << I2C_SR1_RXNE);
    if (temp1 && temp2 && temp3) {
        if (pI2CHandle->pI2Cx->SR2 & (1U << I2C_SR2_MSL)) {
            if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
                I2C_MasterHandleRXNEInterrupt(pI2CHandle);
            }
        } else {
            if (!(pI2CHandle->pI2Cx->SR2 & (1U << I2C_SR2_TRA))) {
                I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
            }
        }
    }
}

/**
 * @brief  Handles the I2C error interrupt (ER_IRQ).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle) {
    uint32_t temp1, temp2;

    temp2 = (pI2CHandle->pI2Cx->CR2) & (1U << I2C_CR2_ITERREN);

    temp1 = (pI2CHandle->pI2Cx->SR1) & (1U << I2C_SR1_BERR);
    if (temp1 && temp2) {
        pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_BERR);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
    }

    temp1 = (pI2CHandle->pI2Cx->SR1) & (1U << I2C_SR1_ARLO);
    if (temp1 && temp2) {
        pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_ARLO);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
    }

    temp1 = (pI2CHandle->pI2Cx->SR1) & (1U << I2C_SR1_AF);
    if (temp1 && temp2) {
        pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_AF);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
    }

    temp1 = (pI2CHandle->pI2Cx->SR1) & (1U << I2C_SR1_OVR);
    if (temp1 && temp2) {
        pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_OVR);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
    }

    temp1 = (pI2CHandle->pI2Cx->SR1) & (1U << I2C_SR1_TIMEOUT);
    if (temp1 && temp2) {
        pI2CHandle->pI2Cx->SR1 &= ~(1U << I2C_SR1_TIMEOUT);
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
    }
}

/**
 * @brief  Slave sends 1 byte of data.
 * @param  pI2C: Base address of the I2C peripheral.
 * @param  data: Byte to be transmitted.
 * @return None
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data) {
    pI2C->DR = data;
}

/**
 * @brief  Slave receives 1 byte of data.
 * @param  pI2C: Base address of the I2C peripheral.
 * @return Received data byte.
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C) {
    return (uint8_t)pI2C->DR;
}

/**
 * @brief  Enables or disables the generation of ACK.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
    if (EnorDi == I2C_ACK_ENABLE) {
        pI2Cx->CR1 |= (1U << I2C_CR1_ACK);
    } else {
        pI2Cx->CR1 &= ~(1U << I2C_CR1_ACK);
    }
}

/**
 * @brief  Generates a START condition on the I2C bus.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @return None
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1U << I2C_CR1_START);
}

/**
 * @brief  Executes the address phase for writing.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  SlaveAddr: Target slave address.
 * @return None
 */
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr) {
    SlaveAddr = SlaveAddr << 1U;
    SlaveAddr &= ~(1U);
    pI2Cx->DR = SlaveAddr;
}

/**
 * @brief  Executes the address phase for reading.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  SlaveAddr: Target slave address.
 * @return None
 */
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr) {
    SlaveAddr = SlaveAddr << 1U;
    SlaveAddr |= 1U;
    pI2Cx->DR = SlaveAddr;
}

/**
 * @brief  Clears the ADDR flag in the status register.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle) {
    uint32_t dummyRead;
    
    if (pI2CHandle->pI2Cx->SR2 & (1U << I2C_SR2_MSL)) {
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX) {
            if (pI2CHandle->RxSize == 1) {
                I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
                dummyRead = pI2CHandle->pI2Cx->SR1;
                dummyRead = pI2CHandle->pI2Cx->SR2;
                (void)dummyRead;
            }
        } else {
            dummyRead = pI2CHandle->pI2Cx->SR1;
            dummyRead = pI2CHandle->pI2Cx->SR2;
            (void)dummyRead;
        }
    } else {
        dummyRead = pI2CHandle->pI2Cx->SR1;
        dummyRead = pI2CHandle->pI2Cx->SR2;
        (void)dummyRead;
    }
}

/**
 * @brief  Generates a STOP condition on the I2C bus.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @return None
 */
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1U << I2C_CR1_STOP);
}
