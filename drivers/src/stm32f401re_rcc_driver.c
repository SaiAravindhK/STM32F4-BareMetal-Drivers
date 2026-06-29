#include "stm32f401re_rcc_driver.h"

static const uint16_t AHB_PreScaler[8] = {2U, 4U, 8U, 16U, 64U, 128U, 256U, 512U};
static const uint16_t APB_PreScaler[4] = {2U, 4U, 8U, 16U};

/******************************************************************************/
/* RCC Driver API Implementations                                             */
/******************************************************************************/

/**
 * @brief  Calculates and returns the APB1 peripheral clock (PCLK1) frequency.
 * @return PCLK1 value in Hz.
 */
uint32_t RCC_GetPCLK1Value(void) {
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahbp, apb1p;

    clksrc = (RCC->CFGR >> 2U) & 0x3U;
    
    if (clksrc == 0U) {
        SystemClk = 16000000U; // HSI
    } else if (clksrc == 1U) {
        SystemClk = 8000000U;  // HSE
    } else {
        SystemClk = 16000000U; // Default fallback if PLL is accidentally selected
    }

    temp = (RCC->CFGR >> 4U) & 0xFU;
    if (temp < 8U) {
        ahbp = 1U;
    } else {
        ahbp = AHB_PreScaler[temp - 8U];
    }

    temp = (RCC->CFGR >> 10U) & 0x7U;
    if (temp < 4U) {
        apb1p = 1U;
    } else {
        apb1p = APB_PreScaler[temp - 4U];
    }
    
    pclk1 = ((SystemClk / ahbp) / apb1p);
    return pclk1;
}

/**
 * @brief  Calculates and returns the APB2 peripheral clock (PCLK2) frequency.
 * @return PCLK2 value in Hz.
 */
uint32_t RCC_GetPCLK2Value(void) {
    uint32_t pclk2, SystemClk;
    uint8_t clksrc, temp, ahbp, apb2p;

    clksrc = (RCC->CFGR >> 2U) & 0x3U;
    
    if (clksrc == 0U) {
        SystemClk = 16000000U; // HSI
    } else if (clksrc == 1U) {
        SystemClk = 8000000U;  // HSE
    } else {
        SystemClk = 16000000U; // Default fallback
    }

    temp = (RCC->CFGR >> 4U) & 0xFU;
    if (temp < 8U) {
        ahbp = 1U;
    } else {
        ahbp = AHB_PreScaler[temp - 8U];
    }

    temp = (RCC->CFGR >> 13U) & 0x7U;
    if (temp < 4U) {
        apb2p = 1U;
    } else {
        apb2p = APB_PreScaler[temp - 4U];
    }

    pclk2 = ((SystemClk / ahbp) / apb2p);
    return pclk2;
}
