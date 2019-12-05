//
// Created by skjsn on 2019/10/28.
//

#ifndef STM32EL_STM32F427IIH6_EL_DMA_H
#define STM32EL_STM32F427IIH6_EL_DMA_H

#include "stm32el_sys.h"

namespace STM32EL{
  enum class DMA_Mode{
    Circular,Normal,PeripheralFlowControl
  };
  enum class DMA_Direction{
    Per2Mem,Mem2Per
  };
  enum class DMA_Channel{
    CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7
  };
  enum class DMA_Type{
    Dma1_Stream0 = 1,
    Dma1_Stream1 = 3,
    Dma1_Stream2 = 5,
    Dma1_Stream3 = 7,
    Dma1_Stream4 = 9,
    Dma1_Stream5 = 11,
    Dma1_Stream6 = 13,
    Dma1_Stream7 = 15,
    Dma2_Stream0 = 0,
    Dma2_Stream1 = 2,
    Dma2_Stream2 = 4,
    Dma2_Stream3 = 6,
    Dma2_Stream4 = 8,
    Dma2_Stream5 = 10,
    Dma2_Stream6 = 12,
    Dma2_Stream7 = 14,
  };
  class DMA {
  private:
    void* handle;

  public:
    void *getHandle() const;

  public:
    DMA(DMA_Type,DMA_Channel,DMA_Mode,DMA_Direction);
  };
}

#endif // STM32EL_STM32F427IIH6_EL_DMA_H
