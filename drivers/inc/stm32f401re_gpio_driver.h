#ifndef INC_STM32F401RE_GPIO_DRIVER_H_
#define INC_STM32F401RE_GPIO_DRIVER_H_

#include "stm32f401re.h"
#include "core_cm4.h"

/******************************************************************************/
/* Configuration and Handle Structures                                        */
/******************************************************************************/

/**
 * @brief Configuration structure for a GPIO pin
 */
typedef struct {
    uint8_t GPIO_PinNumber;         /**< Pin number. Refer @ref GPIO_PIN_NUMBERS */
    uint8_t GPIO_PinMode;           /**< Pin mode. Refer @ref GPIO_PIN_MODES */
    uint8_t GPIO_PinSpeed;          /**< Output speed. Refer @ref GPIO_PIN_SPEEDS */
    uint8_t GPIO_PinPuPdControl;    /**< Pull-up/pull-down control. Refer @ref GPIO_PIN_PUPD */
    uint8_t GPIO_PinOPType;         /**< Output type. Refer @ref GPIO_PIN_OP_TYPES */
    uint8_t GPIO_PinAltFunMode;     /**< Alternate function mode value */
} GPIO_PinConfig_t;

/**
 * @brief Handle structure for a GPIO pin
 */
typedef struct {
    GPIO_RegDef_t *pGPIOx;          /**< Pointer to the base address of the GPIO port */
    GPIO_PinConfig_t GPIO_PinConfig; /**< GPIO pin configuration settings */
} GPIO_Handle_t;


/******************************************************************************/
/* GPIO Configuration Macros                                                  */
/******************************************************************************/

/** * @defgroup GPIO_PIN_NUMBERS GPIO Pin Numbers
 * @{
 */
#define GPIO_PIN_NO_0           (0U)
#define GPIO_PIN_NO_1           (1U)
#define GPIO_PIN_NO_2           (2U)
#define GPIO_PIN_NO_3           (3U)
#define GPIO_PIN_NO_4           (4U)
#define GPIO_PIN_NO_5           (5U)
#define GPIO_PIN_NO_6           (6U)
#define GPIO_PIN_NO_7           (7U)
#define GPIO_PIN_NO_8           (8U)
#define GPIO_PIN_NO_9           (9U)
#define GPIO_PIN_NO_10          (10U)
#define GPIO_PIN_NO_11          (11U)
#define GPIO_PIN_NO_12          (12U)
#define GPIO_PIN_NO_13          (13U)
#define GPIO_PIN_NO_14          (14U)
#define GPIO_PIN_NO_15          (15U)
/** @} */

/** * @defgroup GPIO_PIN_MODES GPIO Pin Possible Modes
 * @{
 */
#define GPIO_MODE_IN            (0U)    /**< Input mode */
#define GPIO_MODE_OUT           (1U)    /**< Output mode */
#define GPIO_MODE_ALTFN         (2U)    /**< Alternate function mode */
#define GPIO_MODE_ANALOG        (3U)    /**< Analog mode */
#define GPIO_MODE_IT_FT         (4U)    /**< Interrupt: Falling edge trigger */
#define GPIO_MODE_IT_RT         (5U)    /**< Interrupt: Rising edge trigger */
#define GPIO_MODE_IT_RFT        (6U)    /**< Interrupt: Rising and falling edge trigger */
/** @} */

/** * @defgroup GPIO_PIN_OP_TYPES GPIO Pin Possible Output Types
 * @{
 */
#define GPIO_OP_TYPE_PP         (0U)    /**< Push-pull */
#define GPIO_OP_TYPE_OD         (1U)    /**< Open-drain */
/** @} */

/** * @defgroup GPIO_PIN_SPEEDS GPIO Pin Possible Output Speeds
 * @{
 */
#define GPIO_SPEED_LOW          (0U)
#define GPIO_SPEED_MEDIUM       (1U)
#define GPIO_SPEED_FAST         (2U)
#define GPIO_SPEED_HIGH         (3U)
/** @} */

/** * @defgroup GPIO_PIN_PUPD GPIO Pin Pull-up/Pull-down Configuration
 * @{
 */
#define GPIO_NO_PUPD            (0U)    /**< No pull-up, no pull-down */
#define GPIO_PIN_PU             (1U)    /**< Pull-up enabled */
#define GPIO_PIN_PD             (2U)    /**< Pull-down enabled */
/** @} */


/******************************************************************************/
/* GPIO Driver APIs (Function Prototypes)                                     */
/******************************************************************************/

/**
 * @brief  Enables or disables peripheral clock for the given GPIO port.
 * @param  pGPIOx: Base address of the GPIO port.
 * @param  EnorDi: ENABLE or DISABLE macro.
 * @return None
 */
void GPIO_PeriphClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/**
 * @brief  Initializes the given GPIO pin according to the specified parameters in the handle.
 * @param  pGPIOHandle: Pointer to the GPIO handle structure.
 * @return None
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

/**
 * @brief  De-initializes the given GPIO port, resetting its registers to default values.
 * @param  pGPIOx: Base address of the GPIO port.
 * @return None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/**
 * @brief  Reads the value from a specific input pin.
 * @param  pGPIOx: Base address of the GPIO port.
 * @param  PinNumber: Pin number to read from.
 * @return 0 or 1 depending on the pin state.
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/**
 * @brief  Reads the entire 16-bit value from a specific input port.
 * @param  pGPIOx: Base address of the GPIO port.
 * @return 16-bit value representing the port state.
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

/**
 * @brief  Writes a binary value to a specific output pin.
 * @param  pGPIOx: Base address of the GPIO port.
 * @param  PinNumber: Pin number to write to.
 * @param  Value: GPIO_PIN_SET or GPIO_PIN_RESET.
 * @return None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);

/**
 * @brief  Writes a 16-bit value to the entire output port.
 * @param  pGPIOx: Base address of the GPIO port.
 * @param  Value: 16-bit value to write to the port.
 * @return None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

/**
 * @brief  Toggles the state of a specific output pin.
 * @param  pGPIOx: Base address of the GPIO port.
 * @param  PinNumber: Pin number to toggle.
 * @return None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/**
 * @brief  Handles the interrupt processing for a specific GPIO pin.
 * @param  PinNumber: Pin number that triggered the interrupt.
 * @return None
 */
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F401RE_GPIO_DRIVER_H_ */