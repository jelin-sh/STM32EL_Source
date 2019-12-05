//
// Created by skjsn on 2019/10/28.
//

#include "EL_DMA.h"
#include "stm32f4xx_hal.h"

STM32EL::DMA::DMA(STM32EL::DMA_Type t, STM32EL::DMA_Channel c, STM32EL::DMA_Mode m,
                  STM32EL::DMA_Direction d){
  u32 mode = 0;
  DMA_Stream_TypeDef *type = 0;
  u32 channel = 0;
  u32 direction = 0;
  if((int)t%2){
    __HAL_RCC_DMA1_CLK_ENABLE();
  } else{
    __HAL_RCC_DMA2_CLK_ENABLE();
  }
  switch (t){
  case DMA_Type::Dma1_Stream0:
    type = DMA1_Stream0;
    break;
  case DMA_Type::Dma1_Stream1:
    type = DMA1_Stream1;
    break;
  case DMA_Type::Dma1_Stream2:
    type = DMA1_Stream2;
    break;
  case DMA_Type::Dma1_Stream3:
    type = DMA1_Stream3;
    break;
  case DMA_Type::Dma1_Stream4:
    type = DMA1_Stream4;
    break;
  case DMA_Type::Dma1_Stream5:
    type = DMA1_Stream5;
    break;
  case DMA_Type::Dma1_Stream6:
    type = DMA1_Stream6;
    break;
  case DMA_Type::Dma1_Stream7:
    type = DMA1_Stream7;
    break;
  case DMA_Type::Dma2_Stream0:
    type = DMA2_Stream0;
    break;
  case DMA_Type::Dma2_Stream1:
    type = DMA2_Stream1;
    break;
  case DMA_Type::Dma2_Stream2:
    type = DMA2_Stream2;
    break;
  case DMA_Type::Dma2_Stream3:
    type = DMA2_Stream3;
    break;
  case DMA_Type::Dma2_Stream4:
    type = DMA2_Stream4;
    break;
  case DMA_Type::Dma2_Stream5:
    type = DMA2_Stream5;
    break;
  case DMA_Type::Dma2_Stream6:
    type = DMA2_Stream6;
    break;
  case DMA_Type::Dma2_Stream7:
    type = DMA2_Stream7;
    break;
  }
  switch (c){

  case DMA_Channel::CH0:
    channel = DMA_CHANNEL_0;
    break;
  case DMA_Channel::CH1:
    channel = DMA_CHANNEL_1;
    break;
  case DMA_Channel::CH2:
    channel = DMA_CHANNEL_2;
    break;
  case DMA_Channel::CH3:
    channel = DMA_CHANNEL_3;
    break;
  case DMA_Channel::CH4:
    channel = DMA_CHANNEL_4;
    break;
  case DMA_Channel::CH5:
    channel = DMA_CHANNEL_5;
    break;
  case DMA_Channel::CH6:
    channel = DMA_CHANNEL_6;
    break;
  case DMA_Channel::CH7:
    channel = DMA_CHANNEL_7;
    break;
  }
  switch (m){
  case DMA_Mode::Circular:
    mode = DMA_CIRCULAR;
    break;
  case DMA_Mode::Normal:
    mode = DMA_NORMAL;
    break;
  case DMA_Mode::PeripheralFlowControl:
    mode = DMA_PFCTRL;
    break;
  }
  switch (d){
  case DMA_Direction::Per2Mem:
    direction = DMA_PERIPH_TO_MEMORY;
    break;
  case DMA_Direction::Mem2Per:
    direction = DMA_MEMORY_TO_PERIPH;
    break;
  }
  DMA_HandleTypeDef* dma = new DMA_HandleTypeDef;
  dma->Instance = type;
  dma->Init.Channel = channel;
  dma->Init.Direction = direction;
  dma->Init.PeriphInc = DMA_PINC_DISABLE;
  dma->Init.MemInc = DMA_MINC_ENABLE;
  dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  dma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  dma->Init.Mode = mode;
  dma->Init.Priority = DMA_PRIORITY_VERY_HIGH;
  dma->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  handle = (void*)dma;
  auto a = HAL_DMA_Init(dma);
}
void *STM32EL::DMA::getHandle() const { return handle; }
