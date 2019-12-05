//
// Created by skjsn on 2019/9/16.
//

#ifndef STM32EL_STM32F427IIH6_EL_INPUTSTREAM_H
#define STM32EL_STM32F427IIH6_EL_INPUTSTREAM_H

#include <stm32el_sys.h>

namespace STM32EL{
    class InPutStream {
    public:
        virtual int get(u8* buffer) = 0;
    };
}


#endif //STM32EL_STM32F427IIH6_EL_INPUTSTREAM_H
