#ifndef INC_STM32F401RE_USART_DRIVER_H_
#define INC_STM32F401RE_USART_DRIVER_H_

#include "stm32f401re.h"

/******************************************************************************/
/* Configuration and Handle Structures                                        */
/******************************************************************************/

/**
 * @brief Configuration structure for USART peripheral
 */
typedef struct {
    uint8_t  USART_Mode;            /**< Tx/Rx mode. Refer @ref USART_MODE */
    uint32_t USART_Baud;            /**< Baud rate. Refer @ref USART_BAUD */
    uint32_t USART_NoOfStopBits;    /**< Number of stop bits. Refer @ref USART_STOPBITS */
    uint32_t USART_WordLength;      /**< Word length (8 or 9 bits). Refer @ref USART_WORDLEN */
    uint32_t USART_ParityControl;   /**< Parity control. Refer @ref USART_PARITY */
    uint32_t USART_HWFlowControl;   /**< Hardware flow control. Refer @ref USART_HW_FLOW_CTRL */
} USART_Config_t;

/**
 * @brief Handle structure for USART peripheral
 */
typedef struct {
    USART_RegDef_t *pUSARTx;        /**< Pointer to the base address of the USART peripheral */
    USART_Config_t USART_Config;    /**< USART configuration settings */
    uint8_t        *pTxBuffer;      /**< Pointer to Tx buffer */
    uint8_t        *pRxBuffer;      /**< Pointer to Rx buffer */
    uint32_t       TxLen;           /**< Length of Tx data remaining */
    uint32_t       RxLen;           /**< Length of Rx data remaining */
    uint8_t        TxBusyState;     /**< Current Tx state. Refer @ref USART_APP_STATES */
    uint8_t        RxBusyState;     /**< Current Rx state. Refer @ref USART_APP_STATES */
} USART_Handle_t;


/******************************************************************************/
/* USART Configuration Macros                                                 */
/******************************************************************************/

/** * @defgroup USART_MODE USART Mode Options
 * @{
 */
#define USART_MODE_ONLY_TX          (0U)
#define USART_MODE_ONLY_RX          (1U)
#define USART_MODE_TXRX             (2U)
/** @} */

/** * @defgroup USART_BAUD USART Standard Baud Rates
 * @{
 */
#define USART_STD_BAUD_1200         (1200U)
#define USART_STD_BAUD_400         (400U)      
#define USART_STD_BAUD_9600         (9600U)
#define USART_STD_BAUD_19200        (19200U)
#define USART_STD_BAUD_38400        (38400U)
#define USART_STD_BAUD_57600        (57600U)
#define USART_STD_BAUD_115200       (115200U)
#define USART_STD_BAUD_230400       (230400U)
#define USART_STD_BAUD_460800       (460800U)
#define USART_STD_BAUD_921600       (921600U)
#define USART_STD_BAUD_2M           (2000000U)
#define USART_STD_BAUD_3M           (3000000U)
/** @} */

/** * @defgroup USART_PARITY USART Parity Control Options
 * @{
 */
#define USART_PARITY_DISABLE        (0U)
#define USART_PARITY_EN_EVEN        (1U)
#define USART_PARITY_EN_ODD         (2U)
/** @} */

/** * @defgroup USART_WORDLEN USART Word Length Options
 * @{
 */
#define USART_WORDLEN_8BITS         (0U)
#define USART_WORDLEN_9BITS         (1U)
/** @} */

/** * @defgroup USART_STOPBITS USART Number of Stop Bits Options
 * @{
 */
#define USART_STOPBITS_1            (0U)
#define USART_STOPBITS_0_5          (1U)
#define USART_STOPBITS_2            (2U)
#define USART_STOPBITS_1_5          (3U)
/** @} */

/** * @defgroup USART_HW_FLOW_CTRL USART Hardware Flow Control Options
 * @{
 */
#define USART_HW_FLOW_CTRL_NONE     (0U)
#define USART_HW_FLOW_CTRL_CTS      (1U)
#define USART_HW_FLOW_CTRL_RTS      (2U)
#define USART_HW_FLOW_CTRL_CTS_RTS  (3U)
/** @} */


/******************************************************************************/
/* USART Status Flags & Application States Definitions                        */
/******************************************************************************/

/** * @defgroup USART_FLAGS USART Status Flags
 * @{
 */
#define USART_FLAG_TXE              (1U << USART_SR_TXE)
#define USART_FLAG_RXNE             (1U << USART_SR_RXNE)
#define USART_FLAG_TC               (1U << USART_SR_TC)
/** @} */

/** * @defgroup USART_APP_STATES USART Application States
 * @{
 */
#define USART_READY                 (0U)
#define USART_BUSY_IN_RX            (1U)
#define USART_BUSY_IN_TX            (2U)
/** @} */

/** * @defgroup USART_APP_EVENTS USART Application Events
 * @{
 */
#define USART_EVENT_TX_CMPLT        (0U)
#define USART_EVENT_RX_CMPLT        (1U)
#define USART_EVENT_IDLE            (2U)
#define USART_EVENT_CTS             (3U)
#define USART_EVENT_PE              (4U)
#define USART_ERR_FE                (5U)
#define USART_ERR_NE                (6U)
#define USART_ERR_ORE               (7U)
/** @} */


/******************************************************************************/
/* USART Driver APIs (Function Prototypes)                                    */
/******************************************************************************/

/* Peripheral Clock Setup */

/**
 * @brief  Enables or disables peripheral clock for the given USART peripheral.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/* Init and De-init */

/**
 * @brief  Initializes the given USART peripheral according to the handle parameters.
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @return None
 */
void USART_Init(USART_Handle_t *pUSARTHandle);

/**
 * @brief  De-initializes the given USART peripheral.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @return None
 */
void USART_DeInit(USART_RegDef_t *pUSARTx);

/* Data Send and Receive */

/**
 * @brief  Sends data over USART in a blocking manner (polling mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return None
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Receives data over USART in a blocking manner (polling mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return None
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/**
 * @brief  Sends data over USART in a non-blocking manner (interrupt mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pTxBuffer: Pointer to the data buffer to be transmitted.
 * @param  Len: Length of the data to be transmitted.
 * @return Current Tx state.
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Receives data over USART in a non-blocking manner (interrupt mode).
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  pRxBuffer: Pointer to the data buffer to store received data.
 * @param  Len: Length of the data to be received.
 * @return Current Rx state.
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/* IRQ Configuration and ISR handling */

/**
 * @brief  Handles the interrupt processing for the USART peripheral.
 * @param  pHandle: Pointer to the USART handle structure.
 * @return None
 */
void USART_IRQHandling(USART_Handle_t *pHandle);

/* Other Peripheral Control APIs */

/**
 * @brief  Enables or disables the USART peripheral (UE bit).
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  EnOrDi: ENABLE or DISABLE macro.
 * @return None
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/**
 * @brief  Returns the status of a specific USART flag.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  FlagName: Name of the flag to check.
 * @return Flag status (SET or RESET).
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);

/**
 * @brief  Clears a specific USART status flag.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  StatusFlagName: Name of the flag to clear.
 * @return None
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/**
 * @brief  Calculates and sets the USART baud rate in the BRR register.
 * @param  pUSARTx: Base address of the USART peripheral.
 * @param  BaudRate: Desired baud rate.
 * @return None
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

/* Application Callback */

/**
 * @brief  Application callback function triggered upon USART events.
 * @note   This function should be implemented by the user application.
 * @param  pUSARTHandle: Pointer to the USART handle structure.
 * @param  AppEv: Application event triggered.
 * @return None
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t AppEv);

#endif /* INC_STM32F401RE_USART_DRIVER_H_ */
