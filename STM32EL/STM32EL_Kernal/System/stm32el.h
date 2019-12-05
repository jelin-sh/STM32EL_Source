//
// Created by skjsn on 2019/8/27.
//

#ifndef STM32EL_STM32F427IIH6_STM32EL_H
#define STM32EL_STM32F427IIH6_STM32EL_H

#include "EL_IOPort.h"
#include "EL_InPutStream.h"
#include "EL_OutPutStream.h"
#include "EL_RCC.h"
#include "EL_SerialPort.h"
#include "EL_Thread.h"
#include "EL_Timer.h"
#include "EL_SPI.h"
#include "EL_PWM.h"
#include "EL_CAN.h"
#include "Volt.h"

namespace STM32EL {
    using DefaultSerial = Uart<STM32EL_SerialBase::SerialBase>;
    using Usart1 = Uart<STM32EL_SerialBase::Usart1Base>;
    using Usart2 = Uart<STM32EL_SerialBase::Usart2Base>;
    using Usart3 = Uart<STM32EL_SerialBase::Usart3Base>;
    using Uart4 = Uart<STM32EL_SerialBase::Uart4Base>;
    using Uart5 = Uart<STM32EL_SerialBase::Uart5Base>;
    using Usart6 = Uart<STM32EL_SerialBase::Usart6Base>;
    using Uart7 = Uart<STM32EL_SerialBase::Uart7Base>;
    using Uart8 = Uart<STM32EL_SerialBase::Uart8Base>;

    using Tim1 = STM32EL_TimerBase::Timer1Base;
    using Tim2 = STM32EL_TimerBase::Timer2Base;
    using Tim3 = STM32EL_TimerBase::Timer3Base;
    using Tim4 = STM32EL_TimerBase::Timer4Base;
    using Tim5 = STM32EL_TimerBase::Timer5Base;
    using Tim6 = STM32EL_TimerBase::Timer6Base;
    using Tim7 = STM32EL_TimerBase::Timer7Base;
    using Tim8 = STM32EL_TimerBase::Timer8Base;
    using Tim9 = STM32EL_TimerBase::Timer9Base;
    using Tim10 = STM32EL_TimerBase::Timer10Base;
    using Tim11 = STM32EL_TimerBase::Timer11Base;
    using Tim12 = STM32EL_TimerBase::Timer12Base;
    using Tim13 = STM32EL_TimerBase::Timer13Base;
    using Tim14 = STM32EL_TimerBase::Timer14Base;

    using Spi1 =  STM32EL_SPIBASE::SPI1_Base;
    using Spi2 =  STM32EL_SPIBASE::SPI2_Base;
    using Spi3 =  STM32EL_SPIBASE::SPI3_Base;
    using Spi4 =  STM32EL_SPIBASE::SPI4_Base;
    using Spi5 =  STM32EL_SPIBASE::SPI5_Base;
    using Spi6 =  STM32EL_SPIBASE::SPI6_Base;

    using Can1 = STM32EL_CAN::Can1;
    using Can2 = STM32EL_CAN::Can2;
}

using namespace STM32EL;
extern Volt volt;

#endif //STM32EL_STM32F427IIH6_STM32EL_H
