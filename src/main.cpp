/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**	Objective 	: Toggle the green and blue leds each time the User button is pressed,
**				 using interruption.
**
*****************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

//to prevent name mangling
extern "C" void EXTI0_IRQHandler(void);
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  int i = 0;

  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used. 
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates 
  *  SCB->VTOR register.  
  *  E.g.  SCB->VTOR = 0x20000000;  
  */

  /* TODO - Add your application code here */

  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

  /* Turn on LEDs */
  STM_EVAL_LEDOn(LED4);

  //configuração do pino A0,do user button
  //Enable the BUTTON Clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitTypeDef button_cfg;
  button_cfg.GPIO_Pin = GPIO_Pin_0;
  button_cfg.GPIO_Mode = GPIO_Mode_IN;
  button_cfg.GPIO_OType = GPIO_OType_PP;
  button_cfg.GPIO_PuPd = GPIO_PuPd_DOWN;
  button_cfg.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&button_cfg);

  EXTI_InitTypeDef config;
  config.EXTI_Line = EXTI_Line0;
  config.EXTI_Mode = EXTI_Mode_Interrupt;
  config.EXTI_Trigger = EXTI_Trigger_Rising;
  config.EXTI_LineCmd = ENABLE;

  EXTI_Init(&config);

  //connects the specified GPIO pin to its EXTI line
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

  NVIC_InitTypeDef NVIC_cfg;
  NVIC_cfg.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_cfg.NVIC_IRQChannelPreemptionPriority = 0x0f;
  NVIC_cfg.NVIC_IRQChannelSubPriority = 0x0f;
  NVIC_cfg.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_cfg);

//  STM_EVAL_PBInit(BUTTON_USER,BUTTON_MODE_EXTI);

  /* Infinite loop */
  while (1)
  {
	i++;
  }
}

//overrides the EXTI0_IRQHandler() of startup_stm32f40xx.s
void EXTI0_IRQHandler(void){
	STM_EVAL_LEDToggle(LED4);
	STM_EVAL_LEDToggle(LED6);

	//do I need indicate that the interruption was handled? Yes, I do
	EXTI_ClearITPendingBit(EXTI_Line0);
	return;
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
extern "C" void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
extern "C" uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
