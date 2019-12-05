//
// Created by skjsn on 2019/9/10.
//

#ifndef STM32EL_STM32F427IIH6_EL_OUTPUTSTREAM_H
#define STM32EL_STM32F427IIH6_EL_OUTPUTSTREAM_H

#include "stm32el_sys.h"

namespace STM32EL{
class OutPutStream {
public:
  virtual void put(u8 data) = 0;
  virtual void put(u8* data,u32 length) = 0;
};
}


#endif // STM32EL_STM32F427IIH6_EL_OUTPUTSTREAM_H
