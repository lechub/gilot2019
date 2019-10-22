/*
 * Hardware.cpp
 *
 *  Created on: 28.06.2017
 *      Author: lechu
 */

#include "Hardware.h"
#include "Gpio.h"
//#include "Fifo.h"
#include "QuickTask.h"
//#include "komunikacja.h"

#ifndef DEBUG
#define WATCHDOG_ENABLE 1
#endif



/*
+=============================================================================+
| local functions
+=============================================================================+
 */



/*------------------------------------------------------------------------*//**
 * \brief Configures Flash latency.
 * \details Configures Flash latency (wait-states) which allows the chip to run
 * at higher speeds. The constants used for calculations are valid when chip's
 * voltage is in 2.7V to 3.6V range.
 *
 * \param [in] frequency defines the target frequency of the core
 *//*-------------------------------------------------------------------------*/

static void flash_latency(uint32_t frequency){

#ifdef STM32F051
  uint32_t latency = (frequency <= 24000000) ? 0x00 : 0x01; // do 24MHz latency = 0, powyzej =1
  FLASH->ACR = latency;				// set wait_states, disable prefetch
  FLASH->ACR = FLASH_ACR_PRFTBE | latency;	  // set latency, enable prefetch
#endif
#ifdef STM32F4xx
  uint32_t wait_states;
  wait_states = frequency / 30000000ul; // calculate wait_states (30M is valid for 2.7V to 3.6V voltage range, use 24M for 2.4V to 2.7V, 18M for 2.1V to 2.4V or 16M for  1.8V to 2.1V)
  wait_states &= 7;           // trim to max allowed value - 7

  FLASH->ACR = wait_states;       // set wait_states, disable all caches and prefetch
  FLASH->ACR = FLASH_ACR_DCRST | FLASH_ACR_ICRST | wait_states; // reset caches
  FLASH->ACR = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | wait_states;  // enable caches and prefetch
#endif
}



static bool pll_start_lb(uint32_t crystal, uint32_t frequency){
#ifdef stm32F4xx
  uint32_t pllm = 8;
  uint32_t plln = 128;
  uint32_t pllp = 4;

  RCC->CR |=  RCC_CR_HSEON;				// enable HSE clock
  while (!RCC->CR & RCC_CR_HSERDY); // wait for stable clock

  flash_latency(frequency);				// configure Flash latency for desired frequency

  RCC->PLLCFGR = (pllm << RCC_PLLCFGR_PLLM_bit) | (plln << RCC_PLLCFGR_PLLN_bit) | ((pllp / 2 - 1) << RCC_PLLCFGR_PLLP_bit) | RCC_PLLCFGR_PLLQ_DIV9 | RCC_PLLCFGR_PLLSRC;	// configure PLL factors, always divide USB clock by 9

  // prescalery - wszystkie x1
  RCC->CFGR = RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_HPRE_DIV1;	// AHB - no prescaler, APB1 - divide by 1, APB2 - divide by 1

  while (!RCC->CR & RCC_CR_HSERDY);// RCC_CR_HSERDY_bb);				// wait for stable clock

  //RCC_CR_PLLON_bb = 1;					// enable PLL
  RCC->CR |=  RCC_CR_PLLON;

  //while (!RCC_CR_PLLRDY_bb);				// wait for PLL lock
  while (!RCC->CR & RCC_CR_PLLRDY);				// wait for PLL lock

  RCC->CFGR |= RCC_CFGR_SW_PLL;			// change SYSCLK to PLL
  while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// wait for switch

#endif
#ifdef STM32F051
  //	uint32_t pllm = 8;
  //	  uint32_t plln = 128;
  //	  uint32_t pllp = 4;

  RCC->CR |= RCC_CR_HSION;       // enable HSI clock
  while (!(RCC->CR & RCC_CR_HSIRDY)); // wait for stable HSI clock

  RCC->CFGR &= (~RCC_CFGR_SW); // HSI as system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // wait for HSI as system clock

//  RCC->CR |=  RCC_CR_HSEON;       // enable HSE clock
//  while (!(RCC->CR & RCC_CR_HSERDY)); // wait for stable HSE clock

  flash_latency(frequency);       // configure Flash latency for desired frequency

  RCC->CFGR = 0
		  // |RCC_CFGR_PLLNODIV // nie dziel PLL /2
     // | RCC_CFGR_MCO_PRE_1        // MCO is divided by 1
      | RCC_CFGR_MCO_NOCLOCK      // Clock output disabled
      | RCC_CFGR_PLLMUL12          // PLL input clock 8/2 x 12
      | RCC_CFGR_PLLXTPRE_PREDIV1 // PREDIV input clock not divided
      | RCC_CFGR_PLLSRC_HSI_DIV2  // HSi/2 selected as PLL input clock
      | RCC_CFGR_HPRE_DIV1  // SYSCLK not divided to HCLK
      | RCC_CFGR_PPRE_DIV1  // HCLK not divided to APB
      | RCC_CFGR_SW_HSI     // ciagle taktowanie z HSI
      ;
  RCC->CFGR2 = RCC_CFGR2_PREDIV1_DIV1;  // PREDIV input clock not divided

  RCC->CR |=  RCC_CR_PLLON;     // turn PLL on
  while (!(RCC->CR & RCC_CR_PLLRDY));       // wait for PLL lock

  RCC->CFGR |= RCC_CFGR_SW_PLL; // change SYSCLK to PLL
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // wait for PLL as system clock

//
//  RCC->CFGR |= RCC_CFGR_SW_PLL;
//  while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // wait for switch

//  RCC->CFGR &= RCC_CR_HSION;    // wylaczenie HSI

#endif

  SystemCoreClockUpdate();	// na wszelki wypadek

  return true;

}


/*------------------------------------------------------------------------*//**
 * \brief Enables FPU
 * \details Enables FPU in Cortex-M4 for both privileged and user mode. This is
 * done by enabling CP10 and CP11 coprocessors in CPACR register (possible only
 * when in privileged mode).
 *//*-------------------------------------------------------------------------*/

//static void fpu_enable(void)
//{
//#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
//	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));	// set CP10 and CP11 Full Access
//#endif
//}


static void rccInit(void){
  // reset gpio's i obu DMA
  //	RCC->AHB1RSTR = RCC_AHB1RSTR_GPIOARST | RCC_AHB1RSTR_GPIOBRST
  //			|RCC_AHB1RSTR_GPIOCRST | RCC_AHB1RSTR_GPIODRST
  //			|RCC_AHB1RSTR_GPIOERST | RCC_AHB1RSTR_GPIOHRST
  //			|RCC_AHB1RSTR_DMA1RST | RCC_AHB1RSTR_DMA2RST
  //			;

  __IO uint32_t tmpreg = 0x00U;

  do {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN
//        | RCC_AHBENR_GPIOBEN
//        |RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN
//        |RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOFEN
        //	|RCC_AHB1ENR_DMA1EN
        //  | RCC_AHB1ENR_DMA2EN
        ;
    tmpreg = RCC->AHBENR;
    (void)(tmpreg);
  } while(0U);


//  do {
//    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
//    /* Delay after an RCC peripheral clock enabling */
//    tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);
//    (void)(tmpreg);
//  } while(0U);


//  do {
//    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN
//        ;
//    /* Delay after an RCC peripheral clock enabling */
//    tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
//    (void)(tmpreg);
//  } while(0U);

}


void Hardware::NVIC_init(){

}

void Hardware::init(){

  // Procesor i zegar zainicjowany

  //flash_latency(32000000);
  pll_start_lb(8000000, 32000000);
  //fpu_enable();

  rccInit();

  if (SysTick_Config( SystemCoreClock / SYSTICK_FREQUENCY_HZ)) {
    errorDispatch(Hardware::ErrorCode::Failure);
  }

  NVIC_init();	// przerwania

//
//
//  void usart_init();

  WDOG_Init();

}

/*------------------------------------------------------------------------*//**
 * \brief GPIO initialization.
 * \details Enables all GPIO ports.
 *//*-------------------------------------------------------------------------*/


#define IWDG_KEY_RELOAD                 0x0000AAAAU  /*!< IWDG Reload Counter Enable   */
#define IWDG_KEY_ENABLE                 0x0000CCCCU  /*!< IWDG Peripheral Enable       */
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x00005555U  /*!< IWDG KR Write Access Enable  */
#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x00000000U  /*!< IWDG KR Write Access Disable */

#define IWDG_PRESCALER_4                0x00000000U                   /*!< IWDG prescaler set to 4   */
#define IWDG_PRESCALER_8                IWDG_PR_PR_0                  /*!< IWDG prescaler set to 8   */
#define IWDG_PRESCALER_16               IWDG_PR_PR_1                  /*!< IWDG prescaler set to 16  */
#define IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0) /*!< IWDG prescaler set to 32  */
#define IWDG_PRESCALER_64               IWDG_PR_PR_2                  /*!< IWDG prescaler set to 64  */
#define IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0) /*!< IWDG prescaler set to 128 */
#define IWDG_PRESCALER_256              (IWDG_PR_PR_2 | IWDG_PR_PR_1) /*!< IWDG prescaler set to 256 */

void Hardware::WDOG_Init(){
#ifdef WATCHDOG_ENABLE
  // Enable write access to IWDG_PR and IWDG_RLR registers by writing 0x5555 in KR
  IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;

  // Write to IWDG registers the Prescaler & Reload values to work with
  IWDG->PR = IWDG_PRESCALER_256;		// 32kHz/256 = 125Hz == 8ms
  IWDG->RLR = 125 * 8 * 2;	// 2 sekundy

  // Enable IWDG. LSI is turned on automaticaly
  IWDG->KR =  IWDG_KEY_ENABLE;	// aktywacja IWDG
#endif	//WATCHDOG_ENABLE
}

void Hardware::WDOG_Reload(){
#ifdef WATCHDOG_ENABLE
  IWDG->KR =  IWDG_KEY_RELOAD;	// przeladowanie IWDG
#endif	//WATCHDOG_ENABLE
}






/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void){
  while(true);
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void){
  while(true);
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void){
  while(true);
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void){
  while(true);
}

///**
//* @brief This function handles System tick timer.
//*/
//void SysTick_Handler(void){
//  QuickTask::tick(1);
//}




