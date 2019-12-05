//
// Created by skjsn on 2019/9/16.
//

#ifndef STM32EL_STM32F427IIH6_EL_WWDG_H
#define STM32EL_STM32F427IIH6_EL_WWDG_H

#include "stm32el_sys.h"

namespace STM32EL{
    class WatchDog {
    public:
        static void Init();
        static void Feed();
        static void ResetMCU();
    };
}



#endif //STM32EL_STM32F427IIH6_EL_WWDG_H
