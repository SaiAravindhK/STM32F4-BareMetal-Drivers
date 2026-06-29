#ifndef INC_STM32F401RE_I2C_DRIVER_H_
#define INC_STM32F401RE_I2C_DRIVER_H_

#include "stm32f401re.h"

/******************************************************************************/
/* Configuration and Handle Structures                                        */
/******************************************************************************/

/**
 * @brief Configuration structure for I2Cx peripheral
 */
typedef struct {
    uint32_t I2C_SCLSpeed;      /**< SCL clock speed. Refer @ref I2C_SCL_SPEEDS */
    uint32_t I2C_DeviceAddress; /**< Slave device address (7-bit or 10-bit) */
    uint32_t I2C_ACKControl;    /**< ACK enable/disable. Refer @ref I2C_ACK_CONTROL */
    uint32_t I2C_FMDutyCycle;   /**< Fast mode duty cycle. Refer @ref I2C_FM_DUTY_CYCLE */
} I2C_Config_t;

/**
 * @brief Handle structure for I2Cx peripheral
 */
typedef struct {
    I2C_RegDef_t *pI2Cx;        /**< Pointer to the base address of the I2C peripheral */
    I2C_Config_t I2C_Config;    /**< I2C configuration settings */
    uint8_t      *pTxBuffer;    /**< Pointer to Tx buffer */
    uint8_t      *pRxBuffer;    /**< Pointer to Rx buffer */
    uint32_t     TxLen;         /**< Length of Tx data remaining */
    uint32_t     RxLen;         /**< Length of Rx data remaining */
    uint8_t      TxRxState;     /**< Current communication state. Refer @ref I2C_APP_STATES */
    uint8_t      DevAddr;       /**< Slave device address for communication */
    uint32_t     RxSize;        /**< Total size of Rx data to be received */
    uint8_t      Sr;            /**< Repeated Start value */
} I2C_Handle_t;


/******************************************************************************/
/* I2C Configuration Macros                                                   */
/******************************************************************************/

/** * @defgroup I2C_APP_STATES I2C Application States
 * @{
 */
#define I2C_READY               (0U)
#define I2C_BUSY_IN_RX          (1U)
#define I2C_BUSY_IN_TX          (2U)
/** @} */

/** * @defgroup I2C_APP_EVENTS I2C Application Events
 * @{
 */
#define I2C_EV_TX_CMPLT         (0U)
#define I2C_EV_RX_CMPLT         (1U)
#define I2C_EV_STOP             (2U)
#define I2C_ERROR_BERR          (3U)
#define I2C_ERROR_ARLO          (4U)
#define I2C_ERROR_AF            (5U)
#define I2C_ERROR_OVR           (6U)
#define I2C_ERROR_TIMEOUT       (7U)
#define I2C_EV_DATA_REQ         (8U)
#define I2C_EV_DATA_RCV         (9U)
/** @} */

/** * @defgroup I2C_SCL_SPEEDS I2C SCL Speeds
 * @{
 */
#define I2C_SCL_SPEED_SM        (100000U)   /**< Standard Mode: 100 kHz */
#define I2C_SCL_SPEED_FM_200K   (200000U)   /**< Fast Mode: 200 kHz */
#define I2C_SCL_SPEED_FM_400K   (400000U)   /**< Fast Mode: 400 kHz */
/** @} */

/** * @defgroup I2C_ACK_CONTROL I2C ACK Control
 * @{
 */
#define I2C_ACK_DISABLE         (0U)
#define I2C_ACK_ENABLE          (1U)
/** @} */

/** * @defgroup I2C_FM_DUTY_CYCLE I2C Fast Mode Duty Cycle
 * @{
 */
#define I2C_FM_DUTY_2           (0U)
#define I2C_FM_DUTY_16_9        (1U)
/** @} */

/** * @defgroup I2C_REPEATED_START I2C Repeated Start Control
 * @{
 */
#define I2C_DISABLE_SR          RESET
#define I2C_ENABLE_SR           SET
/** @} */


/******************************************************************************/
/* I2C Status Flags Definitions                                               */
/******************************************************************************/

/** * @defgroup I2C_FLAGS I2C Status Flags
 * @{
 */
#define I2C_FLAG_TXE            (1U << I2C_SR1_TXE)
#define I2C_FLAG_RXNE           (1U << I2C_SR1_RXNE)
#define I2C_FLAG_SB             (1U << I2C_SR1_SB)
#define I2C_FLAG_ADDR           (1U << I2C_SR1_ADDR)
#define I2C_FLAG_BTF            (1U << I2C_SR1_BTF)
/** @} */


/******************************************************************************/
/* I2C Driver APIs (Function Prototypes)                                      */
/******************************************************************************/

/* Peripheral Clock and Control */

/**
 * @brief  Enables or disables peripheral clock for the given I2C peripheral.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_PeriphClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/**
 * @brief  Enables or disables the I2C peripheral (PE bit).
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/**
 * @brief  Initializes the given I2C peripheral according to the handle parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);

/**
 * @brief  De-initializes the given I2C peripheral, resetting its registers to default values.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @return None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/**
 * @brief  Returns the status of a specific I2C flag.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  FlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/**
 * @brief  Enables or disables the generation of ACK.
 * @param  pI2Cx: Base address of the I2C peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);


/* Data Send and Receive (Blocking / Polling) */

/**
 * @brief  Master sends data over I2C in a blocking manner.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return None
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief  Master receives data over I2C in a blocking manner.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return None
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief  Slave sends 1 byte of data.
 * @param  pI2C: Base address of the I2C peripheral.
 * @param  data: Byte to be transmitted.
 * @return None
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2C, uint8_t data);

/**
 * @brief  Slave receives 1 byte of data.
 * @param  pI2C: Base address of the I2C peripheral.
 * @return Received data byte.
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);


/* Data Send and Receive (Non-Blocking / Interrupt) */

/**
 * @brief  Master sends data over I2C in a non-blocking manner (interrupt mode).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return Current application state.
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief  Master receives data over I2C in a non-blocking manner (interrupt mode).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @param  SlaveAddr: Address of the target slave device.
 * @param  Sr: Repeated start enable/disable.
 * @return Current application state.
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);


/* Interrupt Closures */

/**
 * @brief  Closes active I2C transmission and resets Tx parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Closes active I2C reception and resets Rx parameters.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);


/* IRQ Configuration and ISR Handling */

/**
 * @brief  Handles the I2C event interrupt (EV_IRQ).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Handles the I2C error interrupt (ER_IRQ).
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @return None
 */
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Application callback function triggered upon I2C events.
 * @note   This function should be implemented by the user application.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @param  AppEv: Application event triggered.
 * @return None
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

#endif /* INC_STM32F401RE_I2C_DRIVER_H_ */