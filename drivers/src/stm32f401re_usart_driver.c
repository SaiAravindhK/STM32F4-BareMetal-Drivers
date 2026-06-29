#include "stm32f401re_usart_driver.h"
#include "stm32f401re_rcc_driver.h"
/******************************************************************************/
/* USART Driver API Implementations                                           */
/******************************************************************************/

/**
 * @brief  Calculates and sets the USART baud rate in the BRR register.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  BaudRate: Desired baud rate.
 * @return None
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate) {
    uint32_t PCLKx;
    uint32_t usartdiv;
    uint32_t M_part, F_part;
    uint32_t tempreg = 0;

    // Get the value of APB bus clock into the variable PCLKx
    if (pUSARTx == USART1 || pUSARTx == USART6) {
        // USART1 and USART6 are hanging on APB2 bus
        PCLKx = RCC_GetPCLK2Value();
    } else {
        PCLKx = RCC_GetPCLK1Value();
    }

    // Check for OVER8 configuration bit
    if (pUSARTx->CR1 & (1U << USART_CR1_OVER8)) {
        // OVER8 = 1, over sampling by 8
        usartdiv = ((25 * PCLKx) / (2 * BaudRate));
    } else {
        // Over sampling by 16
        usartdiv = ((25 * PCLKx) / (4 * BaudRate));
    }

    // Calculate the Mantissa part
    M_part = usartdiv / 100;

    // Place the Mantissa part in appropriate bit position (refer USART_BRR)
    tempreg |= M_part << 4;

    // Extract the fraction part
    F_part = (usartdiv - (M_part * 100));

    // Calculate the final fractional
    if (pUSARTx->CR1 & (1U << USART_CR1_OVER8)) {
        // OVER8 = 1, over sampling by 8
        F_part = (((F_part * 8) + 50) / 100) & ((uint8_t)0x07);
    } else {
        // Over sampling by 16
        F_part = (((F_part * 16) + 50) / 100) & ((uint8_t)0x0F);
    }

    // Place the fractional part in appropriate bit position
    tempreg |= F_part;

    // Copy the value of tempreg into BRR register
    pUSARTx->BRR = tempreg;
}

/**
 * @brief  Initializes the given USART peripheral according to the handle parameters.
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @return None
 */
void USART_Init(USART_Handle_t *pUSARTHandle) {
    uint32_t tempreg = 0;

    /******************************** Configuration of CR1 ********************************/
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX) {
        tempreg |= (1U << USART_CR1_RE);
    } else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX) {
        tempreg |= (1U << USART_CR1_TE);
    } else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX) {
        tempreg |= ((1U << USART_CR1_RE) | (1U << USART_CR1_TE));
    }

    tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M;

    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN) {
        tempreg |= (1U << USART_CR1_PCE);
        // EVEN parity is default when PCE is set
    } else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD) {
        tempreg |= (1U << USART_CR1_PCE);
        tempreg |= (1U << USART_CR1_PS);
    }

    pUSARTHandle->pUSARTx->CR1 = tempreg;

    /******************************** Configuration of CR2 ********************************/
    tempreg = 0;
    tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;
    pUSARTHandle->pUSARTx->CR2 = tempreg;

    /******************************** Configuration of CR3 ********************************/
    tempreg = 0;
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS) {
        tempreg |= (1U << USART_CR3_CTSE);
    } else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS) {
        tempreg |= (1U << USART_CR3_RTSE);
    } else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS) {
        tempreg |= (1U << USART_CR3_CTSE);
        tempreg |= (1U << USART_CR3_RTSE);
    }
    pUSARTHandle->pUSARTx->CR3 = tempreg;

    /******************************** Configuration of BRR ********************************/
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

/**
 * @brief  De-initializes the given USART peripheral, resetting its registers to default values.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @return None
 */
void USART_DeInit(USART_RegDef_t *pUSARTx) {
    if      (pUSARTx == USART1) { USART1_REG_RESET(); }
    else if (pUSARTx == USART2) { USART2_REG_RESET(); }
    else if (pUSARTx == USART6) { USART6_REG_RESET(); }
}


/**
 * @brief  Enables or disables the USART peripheral (UE bit).
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  Cmd: ENABLE or DISABLE macro.
 * @return None
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t Cmd) {
    if (Cmd == ENABLE) {
        pUSARTx->CR1 |= (1U << USART_CR1_UE); // Replaced hardcoded 13 with macro
    } else {
        pUSARTx->CR1 &= ~(1U << USART_CR1_UE);
    }
}

/**
 * @brief  Enables or disables peripheral clock for the given USART peripheral.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
			if(pUSARTx == USART1){
				USART1_PCLK_EN();
			}
			else if(pUSARTx == USART2){
				USART2_PCLK_EN();
			}
			else if(pUSARTx == USART6){
				USART6_PCLK_EN();   
			}
		}
		else{
			if(pUSARTx == USART1){
				USART1_PCLK_DI();
			}
			else if(pUSARTx == USART2){
				USART2_PCLK_DI();
			}
			else if(pUSARTx == USART6){
				USART6_PCLK_DI();
			}
		}
}

/**
 * @brief  Returns the status of a specific USART flag.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  StatusFlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t StatusFlagName) {
    if (pUSARTx->SR & StatusFlagName) {
        return SET;
    }
    return RESET;
}

/**
 * @brief  Clears a specific USART status flag.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  StatusFlagName: Name of the flag to clear.
 * @return None
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName) {
    pUSARTx->SR &= ~(StatusFlagName);
}

/**
 * @brief  Sends data over USART in a blocking manner (polling mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return None
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len) {
    uint16_t *pdata;

    for (uint32_t i = 0; i < Len; i++) {
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
            pdata = (uint16_t*)pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                pTxBuffer++;
                pTxBuffer++;
            } else {
                pTxBuffer++;
            }
        } else {
            pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
            pTxBuffer++;
        }
    }
    while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}

/**
 * @brief  Receives data over USART in a blocking manner (polling mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return None
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len) {
    for (uint32_t i = 0; i < Len; i++) {
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

        if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                *((uint16_t*)pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                pRxBuffer++;
                pRxBuffer++;
            } else {
                *pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                pRxBuffer++;
            }
        } else {
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
            } else {
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
            }
            pRxBuffer++;
        }
    }
}

/**
 * @brief  Sends data over USART in a non-blocking manner (interrupt mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return Current Tx state.
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len) {
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if (txstate != USART_BUSY_IN_TX) {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TXEIE);
        pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TCIE);
    }
    return txstate;
}

/**
 * @brief  Receives data over USART in a non-blocking manner (interrupt mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return Current Rx state.
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len) {
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if (rxstate != USART_BUSY_IN_RX) {
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        (void)pUSARTHandle->pUSARTx->DR;

        pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_RXNEIE);
    }
    return rxstate;
}

/**
 * @brief  Handles the interrupt processing for the USART peripheral.
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @return None
 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle) {
    uint32_t temp1, temp2, temp3;
    uint16_t *pdata;

    /************************* Check for TC flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_TC);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_TCIE);

    if (temp1 && temp2) {
        if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX) {
            if (!pUSARTHandle->TxLen) {
                pUSARTHandle->pUSARTx->SR &= ~(1U << USART_SR_TC);
                pUSARTHandle->TxBusyState = USART_READY;
                pUSARTHandle->pTxBuffer = 0;
                pUSARTHandle->TxLen = 0;
                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_TX_CMPLT);
            }
        }
    }

    /************************* Check for TXE flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_TXE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_TXEIE);

    if (temp1 && temp2) {
        if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX) {
            if (pUSARTHandle->TxLen > 0) {
                if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
                    pdata = (uint16_t*)pUSARTHandle->pTxBuffer;
                    pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                        pUSARTHandle->pTxBuffer++;
                        pUSARTHandle->pTxBuffer++;
                        pUSARTHandle->TxLen --;
                    } else {
                        pUSARTHandle->pTxBuffer++;
                        pUSARTHandle->TxLen --;
                    }
                } else {
                    pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
                    pUSARTHandle->pTxBuffer++;
                    pUSARTHandle->TxLen -= 1;
                }
            }
            if (pUSARTHandle->TxLen == 0) {
                pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_TXEIE);
            }
        }
    }

    /************************* Check for RXNE flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_RXNE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_RXNEIE);

    if (temp1 && temp2) {
        if (pUSARTHandle->RxBusyState == USART_BUSY_IN_RX) {
            if (pUSARTHandle->RxLen > 0) {
                if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS) {
                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                        *((uint16_t*)pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                        pUSARTHandle->pRxBuffer++;
                        pUSARTHandle->pRxBuffer++;
                        pUSARTHandle->RxLen --;
                    } else {
                        *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                        pUSARTHandle->pRxBuffer++;
                        pUSARTHandle->RxLen --;
                    }
                } else {
                    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE) {
                        *pUSARTHandle->pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                    } else {
                        *pUSARTHandle->pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
                    }
                    pUSARTHandle->pRxBuffer++;
                    pUSARTHandle->RxLen -= 1;
                }
            }
            if (!pUSARTHandle->RxLen) {
                pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_RXNEIE);
                pUSARTHandle->RxBusyState = USART_READY;
                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_RX_CMPLT);
            }
        }
    }

    /************************* Check for CTS flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_CTS);
    temp2 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_CTSE);
    temp3 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_CTSIE);

    if (temp1 && temp2) {
        pUSARTHandle->pUSARTx->SR &= ~(1U << USART_SR_CTS);
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_CTS);
    }

    /************************* Check for IDLE detection flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_IDLE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_IDLEIE);

    if (temp1 && temp2) {
    	uint32_t dummy_read = pUSARTHandle->pUSARTx->SR;
    	dummy_read = pUSARTHandle->pUSARTx->DR;
    	(void)dummy_read;
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
    }

    /************************* Check for Overrun detection flag *************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1U << USART_SR_ORE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1U << USART_CR1_RXNEIE);

    if (temp1 && temp2) {
        USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
    }

    /************************* Check for Error Flag *************************/
    temp2 = pUSARTHandle->pUSARTx->CR3 & (1U << USART_CR3_EIE);

    if (temp2) {
        temp1 = pUSARTHandle->pUSARTx->SR;
        if (temp1 & (1U << USART_SR_FE)) {
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_FE);
        }
        if (temp1 & (1U << USART_SR_NF)) { // Changed from NE to NF to match header macro
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_NE);
        }
        if (temp1 & (1U << USART_SR_ORE)) {
            USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
        }
    }
}

/**
 * @brief  Application callback function triggered upon USART events.
 * @note   This function should be implemented by the user application.
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  event: Application event triggered.
 * @return None
 */
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t event) {
    // Weak implementation; user overrides this in application code
}
