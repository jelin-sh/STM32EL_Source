//
// Created by skjsn on 2019/8/27.
//

#ifndef STM32EL_STM32F427IIH6_EL_RCC_H
#define STM32EL_STM32F427IIH6_EL_RCC_H

#include "stm32el_sys.h"
#include "EL_IOPort.h"

namespace STM32EL {
    class Rcc {
    public:
        static void SystemClockConfig(u32 plln,u32 pllm,u32 pllp,u32 pllq);
        static void GPIOClockEnable(GpioType type);

        static u32 GetSystemClock();
        static u32 GetAHBClock();
        static u32 GetAPB1Clock();
        static u32 GetAPB2Clock();
        static u32 GetTick();
    };
}


#endif //STM32EL_STM32F427IIH6_EL_RCC_H
