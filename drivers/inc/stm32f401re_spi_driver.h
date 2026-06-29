#ifndef INC_STM32F401RE_SPI_DRIVER_H_
#define INC_STM32F401RE_SPI_DRIVER_H_

#include "stm32f401re.h"

/******************************************************************************/
/* Configuration and Handle Structures                                        */
/******************************************************************************/

/**
 * @brief Configuration structure for SPIx peripheral
 */
typedef struct {
    uint8_t SPI_DeviceMode; /**< Master or Slave mode. Refer @ref SPI_DEVICE_MODES */
    uint8_t SPI_BusConfig;  /**< Full-duplex, Half-duplex, etc. Refer @ref SPI_BUS_CONFIGS */
    uint8_t SPI_SclkSpeed;  /**< Clock prescaler. Refer @ref SPI_SCLK_SPEEDS */
    uint8_t SPI_DFF;        /**< Data frame format (8 or 16 bits). Refer @ref SPI_DFF_MODES */
    uint8_t SPI_CPOL;       /**< Clock polarity. Refer @ref SPI_CPOL_MODES */
    uint8_t SPI_CPHA;       /**< Clock phase. Refer @ref SPI_CPHA_MODES */
    uint8_t SPI_SSM;        /**< Software slave management. Refer @ref SPI_SSM_MODES */
} SPI_Config_t;

/**
 * @brief Handle structure for SPIx peripheral
 */
typedef struct {
    SPI_RegDef_t *pSPIx;    /**< Pointer to the base address of the SPI peripheral */
    SPI_Config_t SPIConfig; /**< SPI configuration settings */
    uint8_t      *pTxBuffer;/**< Pointer to Tx buffer for interrupt-based transmission */
    uint8_t      *pRxBuffer;/**< Pointer to Rx buffer for interrupt-based reception */
    uint32_t     TxLen;     /**< Length of Tx data remaining */
    uint32_t     RxLen;     /**< Length of Rx data remaining */
    uint8_t      TxState;   /**< Current Tx state. Refer @ref SPI_APP_STATES */
    uint8_t      RxState;   /**< Current Rx state. Refer @ref SPI_APP_STATES */
} SPI_Handle_t;


/******************************************************************************/
/* SPI Configuration Macros                                                   */
/******************************************************************************/

/** * @defgroup SPI_APP_STATES SPI Application States
 * @{
 */
#define SPI_READY               (0U)
#define SPI_BUSY_IN_RX          (1U)
#define SPI_BUSY_IN_TX          (2U)
/** @} */

/** * @defgroup SPI_APP_EVENTS SPI Application Events
 * @{
 */
#define SPI_EVENT_TX_CMPLT      (1U)
#define SPI_EVENT_RX_CMPLT      (2U)
#define SPI_EVENT_OVR_ERR       (3U)
#define SPI_EVENT_CRC_ERR       (4U)
/** @} */

/** * @defgroup SPI_DEVICE_MODES SPI Device Mode
 * @{
 */
#define SPI_DEVICE_MODE_SLAVE   (0U)
#define SPI_DEVICE_MODE_MASTER  (1U)
/** @} */

/** * @defgroup SPI_BUS_CONFIGS SPI Bus Configuration
 * @{
 */
#define SPI_BUS_CONFIG_FD               (1U)    /**< Full Duplex */
#define SPI_BUS_CONFIG_HD               (2U)    /**< Half Duplex */
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY   (3U)    /**< Simplex Receive Only */
/** @} */

/** * @defgroup SPI_SCLK_SPEEDS SPI Clock Speed (Prescaler)
 * @{
 */
#define SPI_SCLK_SPEED_DIV2     (0U)
#define SPI_SCLK_SPEED_DIV4     (1U)
#define SPI_SCLK_SPEED_DIV8     (2U)
#define SPI_SCLK_SPEED_DIV16    (3U)
#define SPI_SCLK_SPEED_DIV32    (4U)
#define SPI_SCLK_SPEED_DIV64    (5U)
#define SPI_SCLK_SPEED_DIV128   (6U)
#define SPI_SCLK_SPEED_DIV256   (7U)
/** @} */

/** * @defgroup SPI_DFF_MODES SPI Data Frame Format
 * @{
 */
#define SPI_DFF_8BITS           (0U)
#define SPI_DFF_16BITS          (1U)
/** @} */

/** * @defgroup SPI_CPOL_MODES SPI Clock Polarity
 * @{
 */
#define SPI_CPOL_LOW            (0U)
#define SPI_CPOL_HIGH           (1U)
/** @} */

/** * @defgroup SPI_CPHA_MODES SPI Clock Phase
 * @{
 */
#define SPI_CPHA_LOW            (0U)
#define SPI_CPHA_HIGH           (1U)
/** @} */

/** * @defgroup SPI_SSM_MODES SPI Software Slave Management
 * @{
 */
#define SPI_SSM_DI              (0U)
#define SPI_SSM_EN              (1U)
/** @} */


/******************************************************************************/
/* SPI Status Flags Definitions                                               */
/******************************************************************************/

/** * @defgroup SPI_FLAGS SPI Status Flags
 * @{
 */
#define SPI_TXE_FLAG            (1U << SPI_SR_TXE)
#define SPI_RXNE_FLAG           (1U << SPI_SR_RXNE)
#define SPI_BUSY_FLAG           (1U << SPI_SR_BSY)
/** @} */


/******************************************************************************/
/* SPI Driver APIs (Function Prototypes)                                      */
/******************************************************************************/

/**
 * @brief  Returns the status of a specific SPI flag.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  FlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

/**
 * @brief  Enables or disables peripheral clock for the given SPI peripheral.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_PeriphClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Enables or disables the SPI peripheral (SPE bit).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Configures the Internal Slave Select (SSI) bit.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Configures the SS Output Enable (SSOE) bit.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  EnOrDi: ENABLE or DISABLE macro.
 * @return None
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/**
 * @brief  Initializes the given SPI peripheral according to the specified parameters in the handle.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);

/**
 * @brief  De-initializes the given SPI peripheral, resetting its registers to default values.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @return None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/**
 * @brief  Sends data over SPI in a blocking manner (polling mode).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return None
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Receives data over SPI in a blocking manner (polling mode).
 * @param  pSPIx: Base address of the SPI peripheral.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return None
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/**
 * @brief  Sends data over SPI in a non-blocking manner (interrupt mode).
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return Current Tx state (SPI_READY or SPI_BUSY_IN_TX).
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Receives data over SPI in a non-blocking manner (interrupt mode).
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return Current Rx state (SPI_READY or SPI_BUSY_IN_RX).
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/**
 * @brief  Handles the interrupt processing for the SPI peripheral.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Clears the Overrun (OVR) flag in the status register.
 * @param  pSPIx: Base address of the SPI peripheral.
 * @return None
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);

/**
 * @brief  Closes active SPI transmission and resets Tx parameters.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Closes active SPI reception and resets Rx parameters.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @return None`
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Application callback function triggered upon SPI events.
 * @note   This function should be implemented by the user application.
 * @param  pSPIHandle: Pointer to the SPI handle structure.
 * @param  AppEv: Application event triggered.
 * @return None
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* INC_STM32F401RE_SPI_DRIVER_H_ */
