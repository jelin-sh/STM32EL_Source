//
// Created by skjsn on 2019/9/16.
//

#include <EL_Thread.h>
#include "stm32f4xx_hal.h"
#include "EL_WatchDog.h"


WWDG_HandleTypeDef __STM32EL_WWDG_Handler = {0}; //窗口看门狗句柄
bool __STM32EL_WWDG_Reset = false;
void STM32EL::WatchDog::Init() {
    static int lock = 0;
    if(lock){
       return;
    }

    __STM32EL_WWDG_Handler.Instance=WWDG; //窗口看门狗
    __STM32EL_WWDG_Handler.Init.Prescaler = WWDG_PRESCALER_8;//设置分频系数为 8
    __STM32EL_WWDG_Handler.Init.Window=0X7F; //设置窗口值 0X5F
    __STM32EL_WWDG_Handler.Init.Counter=0x7F; //设置计数器值 0x7F
    __STM32EL_WWDG_Handler.Init.EWIMode = WWDG_EWI_ENABLE;
    HAL_WWDG_Init(&__STM32EL_WWDG_Handler); //初始化 WWDG
    lock = 1;


}
void STM32EL::WatchDog::Feed() {
    HAL_WWDG_Refresh(&__STM32EL_WWDG_Handler);
}

void STM32EL::WatchDog::ResetMCU() {
    __STM32EL_WWDG_Reset = true;
}
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg){
  __HAL_RCC_WWDG_CLK_ENABLE();
  HAL_NVIC_SetPriority(WWDG_IRQn,0,0);
  HAL_NVIC_EnableIRQ(WWDG_IRQn);
}
extern "C" void WWDG_IRQHandler()
{
  HAL_WWDG_IRQHandler(&__STM32EL_WWDG_Handler);
}
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
  if(!__STM32EL_WWDG_Reset){
    STM32EL::WatchDog::Feed();
  }
  NOP();
}