//
// Created by skjsn on 2019/8/27.
//

#include "EL_RCC.h"
#include "stm32f4xx_hal.h"

void STM32EL::Rcc::SystemClockConfig(u32 plln, u32 pllm, u32 pllp, u32 pllq) {
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure;
    RCC_ClkInitTypeDef RCC_ClkInitStructure;

    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟

    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1

    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化

    if(ret!=HAL_OK) while(true);

    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK) while(true);

    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。

    if(ret!=HAL_OK) while(true);
}
void STM32EL::Rcc::GPIOClockEnable(STM32EL::GpioType type) {
  switch (type){
  case GpioType::NO:
    break;
  case GpioType::PA:
    __HAL_RCC_GPIOA_CLK_ENABLE();
    break;
  case GpioType::PB:
    __HAL_RCC_GPIOB_CLK_ENABLE();
    break;
  case GpioType::PC:
    __HAL_RCC_GPIOC_CLK_ENABLE();
    break;
  case GpioType::PD:
    __HAL_RCC_GPIOD_CLK_ENABLE();
    break;
  case GpioType::PE:
    __HAL_RCC_GPIOE_CLK_ENABLE();
    break;
  case GpioType::PF:
    __HAL_RCC_GPIOF_CLK_ENABLE();
    break;
  case GpioType::PG:
    __HAL_RCC_GPIOG_CLK_ENABLE();
    break;
  case GpioType::PH:
    __HAL_RCC_GPIOH_CLK_ENABLE();
    break;
  case GpioType::PI:
    __HAL_RCC_GPIOI_CLK_ENABLE();
    break;
  }
}

u32 STM32EL::Rcc::GetSystemClock() {
    return SystemCoreClock;
}
u32 STM32EL::Rcc::GetAHBClock() {
  u16 divSys = 1;
  switch (RCC->CFGR & RCC_CFGR_HPRE) {
  case RCC_SYSCLK_DIV1:
    divSys = 1;
    break;
  case RCC_SYSCLK_DIV2:
    divSys = 2;
    break;
  case RCC_SYSCLK_DIV4:
    divSys = 4;
    break;
  case RCC_SYSCLK_DIV8:
    divSys = 8;
    break;
  case RCC_SYSCLK_DIV16:
    divSys = 16;
    break;
  case RCC_SYSCLK_DIV64:
    divSys = 64;
    break;
  case RCC_SYSCLK_DIV128:
    divSys = 128;
    break;
  case RCC_SYSCLK_DIV256:
    divSys = 256;
    break;
  case RCC_SYSCLK_DIV512:
    divSys = 512;
    break;
  }
  return GetSystemClock() / divSys;
}
u32 STM32EL::Rcc::GetAPB1Clock() {
  u16 divAPB1 = 1;
  switch ((u32)(RCC->CFGR & RCC_CFGR_PPRE1)) {
  case RCC_HCLK_DIV1:
    divAPB1 = 1;
    break;
  case RCC_HCLK_DIV2:
    divAPB1 = 2;
    break;
  case RCC_HCLK_DIV4:
    divAPB1 = 4;
    break;
  case RCC_HCLK_DIV8:
    divAPB1 = 8;
    break;
  case RCC_HCLK_DIV16:
    divAPB1 = 16;
    break;
  }
  return GetAHBClock() / divAPB1;
}
u32 STM32EL::Rcc::GetAPB2Clock() {
  u16 divAPB2 = 1;
  switch ((RCC->CFGR & RCC_CFGR_PPRE2) >> 3) {
  case RCC_HCLK_DIV1:
    divAPB2 = 1;
    break;
  case RCC_HCLK_DIV2:
    divAPB2 = 2;
    break;
  case RCC_HCLK_DIV4:
    divAPB2 = 4;
    break;
  case RCC_HCLK_DIV8:
    divAPB2 = 8;
    break;
  case RCC_HCLK_DIV16:
    divAPB2 = 16;
    break;
  }
  return GetAHBClock() / divAPB2;
}
u32 STM32EL::Rcc::GetTick() {
  return HAL_GetTick();
}
