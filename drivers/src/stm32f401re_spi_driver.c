#include "stm32f401re_spi_driver.h"

/******************************************************************************/
/* Private Helper Function Prototypes                                         */
/******************************************************************************/
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/******************************************************************************/
/* SPI Driver API Implementations                                             */
/******************************************************************************/

/**
 * @brief  Returns the status of a specific SPI flag.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  FlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName) {
    if (pSPIx->SR & FlagName) {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/**
 * @brief  Enables or disables peripheral clock for the given SPI peripheral.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_PeriphClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        if      (pSPIx == SPI1) { SPI1_PCLK_EN(); }
        else if (pSPIx == SPI2) { SPI2_PCLK_EN(); }
        else if (pSPIx == SPI3) { SPI3_PCLK_EN(); }
        else if (pSPIx == SPI4) { SPI4_PCLK_EN(); }
    } else {
        if      (pSPIx == SPI1) { SPI1_PCLK_DI(); }
        else if (pSPIx == SPI2) { SPI2_PCLK_DI(); }
        else if (pSPIx == SPI3) { SPI3_PCLK_DI(); }
        else if (pSPIx == SPI4) { SPI4_PCLK_DI(); }
    }
}

/**
 * @brief  Enables or disables the SPI peripheral (SPE bit).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        pSPIx->CR1 |= (1U << SPI_CR1_SPE);
    } else {
        pSPIx->CR1 &= ~(1U << SPI_CR1_SPE);
    }
}

/**
 * @brief  Configures the Internal Slave Select (SSI) bit.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        pSPIx->CR1 |= (1U << SPI_CR1_SSI);
    } else {
        pSPIx->CR1 &= ~(1U << SPI_CR1_SSI);
    }
}

/**
 * @brief  Configures the SS Output Enable (SSOE) bit.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnOrDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE) {
        pSPIx->CR2 |= (1U << SPI_CR2_SSOE);
    } else {
        pSPIx->CR2 &= ~(1U << SPI_CR2_SSOE);
    }
}

/**
 * @brief  Initializes the given SPI peripheral according to the specified parameters in the handle.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_Init(SPI_Handle_t *pSPIHandle) {
    // Configuring the SPI_CR1 register
    uint32_t tempreg = 0;

    // Device mode
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

    // Bus configuration
    if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
        // Bi-directional mode should be cleared
        tempreg &= ~(1U << SPI_CR1_BIDIMODE);
    } 
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
        // Bi-directional mode should be set
        tempreg |= (1U << SPI_CR1_BIDIMODE);
    } 
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
        // Bi-directional mode should be cleared
        tempreg &= ~(1U << SPI_CR1_BIDIMODE);
        // RXONLY bit must be set
        tempreg |= (1U << SPI_CR1_RXONLY);
    }

    // SPI serial clock speed (baud rate)
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

    // Configuring the DFF
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

    // Configuring the CPOL
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

    // Configuring the CPHA
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

    // Configuring the SSM
    tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

    pSPIHandle->pSPIx->CR1 = tempreg;
}

/**
 * @brief  De-initializes the given SPI peripheral, resetting its registers to default values.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @return None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx) {
    if      (pSPIx == SPI1) { SPI1_REG_RESET(); }
    else if (pSPIx == SPI2) { SPI2_REG_RESET(); }
    else if (pSPIx == SPI3) { SPI3_REG_RESET(); }
    else if (pSPIx == SPI4) { SPI4_REG_RESET(); }
}

/**
 * @brief  Sends data over SPI in a blocking manner (polling mode).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return None
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len) {
    while (Len > 0) {
        // 1. Wait until TXE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

        // 2. Check the DFF bit in CR1
        if (pSPIx->CR1 & (1U << SPI_CR1_DFF)) {
            // 16 bit DFF
            // Load the data into the DR
            pSPIx->DR = *((uint16_t*)pTxBuffer);
            Len --;
            Len --;
            pTxBuffer += 2;
        } else {
            // 8 bit DFF
            pSPIx->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
}

/**
 * @brief  Receives data over SPI in a blocking manner (polling mode).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return None
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len) {
    while (Len > 0) {
        // 1. Wait until RXNE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == (uint8_t)FLAG_RESET);

        // 2. Check the DFF bit in CR1
        if (pSPIx->CR1 & (1U << SPI_CR1_DFF)) {
            // 16 bit DFF
            // Load the data from DR to Rxbuffer address
            *((uint16_t*)pRxBuffer) = pSPIx->DR;
            Len--;
            Len--;
            pRxBuffer += 2;
        } else {
            // 8 bit DFF
            *(pRxBuffer) = pSPIx->DR;
            Len--;
            pRxBuffer++;
        }
    }
}

/**
 * @brief  Sends data over SPI in a non-blocking manner (interrupt mode).
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return Current Tx state (SPI_READY or SPI_BUSY_IN_TX).
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len) {
    uint8_t state = pSPIHandle->TxState;

    if (state != SPI_BUSY_IN_TX) {
        // Save the TX buffer address and Length information in global variables
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = Len;

        // Mark the SPI state as busy in TX so that no other code can take over same SPI peripheral until TX is over
        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        // Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= (1U << SPI_CR2_TXEIE);
    }
    return state;
}

/**
 * @brief  Receives data over SPI in a non-blocking manner (interrupt mode).
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return Current Rx state (SPI_READY or SPI_BUSY_IN_RX).
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len) {
    uint8_t state = pSPIHandle->RxState;

    if (state != SPI_BUSY_IN_RX) {
        // Save the RX buffer address and Length information in global variables
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = Len;

        // Mark the SPI state as busy in RX so that no other code can take over same SPI peripheral until RX is over
        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        // Enable the RXNEIE control bit to get interrupt whenever RXNE flag is set in SR
        pSPIHandle->pSPIx->CR2 |= (1U << SPI_CR2_RXNEIE);
    }
    return state;
}

/**
 * @brief  Handles the interrupt processing for the SPI peripheral.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle) {
    uint8_t temp1, temp2;

    // TXE handling
    temp1 = pSPIHandle->pSPIx->SR & (1U << SPI_SR_TXE);
    temp2 = pSPIHandle->pSPIx->CR2 & (1U << SPI_CR2_TXEIE);
    if (temp1 && temp2) {
        spi_txe_interrupt_handle(pSPIHandle);
    }

    // RXNE handling
    temp1 = pSPIHandle->pSPIx->SR & (1U << SPI_SR_RXNE);
    temp2 = pSPIHandle->pSPIx->CR2 & (1U << SPI_CR2_RXNEIE);
    if (temp1 && temp2) {
        spi_rxne_interrupt_handle(pSPIHandle);
    }

    // OVR handling
    temp1 = pSPIHandle->pSPIx->SR & (1U << SPI_SR_OVR);
    temp2 = pSPIHandle->pSPIx->CR2 & (1U << SPI_CR2_ERRIE);
    if (temp1 && temp2) {
        spi_ovr_err_interrupt_handle(pSPIHandle);
    }
}

/******************************************************************************/
/* Private Helper Functions Implementation                                    */
/******************************************************************************/

/**
 * @brief  Handles the TXE interrupt.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle) {
    if (pSPIHandle->pSPIx->CR1 & (1U << SPI_CR1_DFF)) {
        // 16 bit DFF
        // Load the data into DR
        pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
        pSPIHandle->TxLen --;
        pSPIHandle->TxLen --;
        pSPIHandle->pTxBuffer += 2;
    } else {
        pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
        pSPIHandle->TxLen--;
        pSPIHandle->pTxBuffer++;
    }
    
    if (!pSPIHandle->TxLen) {
        // Tx is over, so clear the TXEIE bit
        // This stops the interrupts from setting the TXE flag
        SPI_CloseTransmission(pSPIHandle);
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
    }
}

/**
 * @brief  Handles the RXNE interrupt.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle) {
    if (pSPIHandle->pSPIx->CR1 & (1U << SPI_CR1_DFF)) {
        // 16 bit DFF
        *((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
        pSPIHandle->RxLen --;
        pSPIHandle->RxLen --;
        pSPIHandle->pRxBuffer += 2;
    } else {
        *(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
        pSPIHandle->RxLen--;
        pSPIHandle->pRxBuffer++;
    }
    
    if (!pSPIHandle->RxLen) {
        SPI_CloseReception(pSPIHandle);
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
    }
}

/**
 * @brief  Handles the Overrun (OVR) error interrupt.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle) {
    uint8_t temp;
    // Clear the OVR flag
    if (pSPIHandle->TxState != SPI_BUSY_IN_TX) {
        temp = pSPIHandle->pSPIx->DR;
        temp = pSPIHandle->pSPIx->SR;
        (void)temp; // Prevent unused variable warning
    }
    // Inform the application
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

/**
 * @brief  Closes active SPI transmission and resets Tx parameters.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle) {
    pSPIHandle->pSPIx->CR2 &= ~(1U << SPI_CR2_TXEIE);
    pSPIHandle->pTxBuffer = 0; // Using 0 instead of NULL to avoid stddef.h dependency if not included
    pSPIHandle->TxLen = 0;
    pSPIHandle->TxState = SPI_READY;
}

/**
 * @brief  Closes active SPI reception and resets Rx parameters.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle) {
    pSPIHandle->pSPIx->CR2 &= ~(1U << SPI_CR2_RXNEIE);
    pSPIHandle->pRxBuffer = 0; // Using 0 instead of NULL to avoid stddef.h dependency if not included
    pSPIHandle->RxLen = 0;
    pSPIHandle->RxState = SPI_READY;
}

/**
 * @brief  Clears the Overrun (OVR) flag in the status register.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @return None
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx) {
    uint8_t temp;
    temp = pSPIx->DR;
    temp = pSPIx->SR;
    (void)temp; // Prevent unused variable warning
}

/**
 * @brief  Application callback function triggered upon SPI events.
 * @note   This function should be implemented by the user application.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  AppEv: Application event triggered.
 * @return None
 */
__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {
    // Weak implementation; user overrides this in application code
}
