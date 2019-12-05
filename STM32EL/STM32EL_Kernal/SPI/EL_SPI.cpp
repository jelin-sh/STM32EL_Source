//
// Created by skjsn on 2019/9/25.
//

#include "EL_SPI.h"
#include "stm32f4xx_hal.h"
u32 STM32EL_SPIBASE::SPI_Base::getBase() const { return base; }
STM32EL::AfioType STM32EL_SPIBASE::SPI_Base::getAfType() const {
  return afType;
}
STM32EL_SPIBASE::SPI1_Base::SPI1_Base() {
  base = SPI1_BASE;
  afType = STM32EL::AfioType::spi1;
}
STM32EL_SPIBASE::SPI2_Base::SPI2_Base() {
  base = SPI2_BASE;
  afType = STM32EL::AfioType::spi2;
}
STM32EL_SPIBASE::SPI3_Base::SPI3_Base() {
  base = SPI3_BASE;
  afType = STM32EL::AfioType::spi3;
}
STM32EL_SPIBASE::SPI4_Base::SPI4_Base() {
  base = SPI4_BASE;
  afType = STM32EL::AfioType::spi4;
}
STM32EL_SPIBASE::SPI5_Base::SPI5_Base() {
  base = SPI5_BASE;
  afType = STM32EL::AfioType::spi5;
}
STM32EL_SPIBASE::SPI6_Base::SPI6_Base() {
  base = SPI6_BASE;
  afType = STM32EL::AfioType::spi6;
}
void STM32EL_SPIBASE::SPI::Init(u32 Instance, void **handler, bool isMaster,bool CPOL,
                                bool CPHA,bool MSB,bool bit8) {
  SPI_HandleTypeDef* spiHandleTypeDef;
  if(*handler != nullptr){
    spiHandleTypeDef = (SPI_HandleTypeDef*)*handler;
  }else{
    spiHandleTypeDef = new SPI_HandleTypeDef;
  }

  spiHandleTypeDef->Instance=(SPI_TypeDef *)Instance;

  if(isMaster){
    spiHandleTypeDef->Init.Mode=SPI_MODE_MASTER;
  }
  else{
    spiHandleTypeDef->Init.Mode=SPI_MODE_SLAVE;
  }

  if(!CPOL && !CPHA){
    spiHandleTypeDef->Init.CLKPolarity=SPI_POLARITY_LOW; //空闲状态
    spiHandleTypeDef->Init.CLKPhase=SPI_PHASE_1EDGE;//数据被采样
  }else if(!CPOL && CPHA){
    spiHandleTypeDef->Init.CLKPolarity=SPI_POLARITY_LOW; //空闲状态
    spiHandleTypeDef->Init.CLKPhase=SPI_PHASE_2EDGE;//数据被采样
  }else if(CPOL && !CPHA){
    spiHandleTypeDef->Init.CLKPolarity=SPI_POLARITY_HIGH; //空闲状态
    spiHandleTypeDef->Init.CLKPhase=SPI_PHASE_1EDGE;//数据被采样
  }else if(CPOL && CPHA){
    spiHandleTypeDef->Init.CLKPolarity=SPI_POLARITY_HIGH; //空闲状态
    spiHandleTypeDef->Init.CLKPhase=SPI_PHASE_2EDGE;//数据被采样
  }
  if(MSB){
    spiHandleTypeDef->Init.FirstBit=SPI_FIRSTBIT_MSB; //指定数据传输从 MSB 位开始
  } else{
    spiHandleTypeDef->Init.FirstBit=SPI_FIRSTBIT_LSB; //指定数据传输从 MSB 位开始
  }
  if(bit8){
    spiHandleTypeDef->Init.DataSize=SPI_DATASIZE_8BIT;
  } else{
    spiHandleTypeDef->Init.DataSize=SPI_DATASIZE_16BIT;
  }


  spiHandleTypeDef->Init.Direction=SPI_DIRECTION_2LINES;
  spiHandleTypeDef->Init.NSS=SPI_NSS_SOFT;
  spiHandleTypeDef->Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256; //定义波特率预分频的值:波特率预分频值为 256
  spiHandleTypeDef->Init.TIMode=SPI_TIMODE_DISABLE; //关闭 TI 模式
  spiHandleTypeDef->Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE; //关闭 CRC
  spiHandleTypeDef->Init.CRCPolynomial=7; //CRC 值计算的多项式
  HAL_SPI_Init(spiHandleTypeDef);//初始化

  *handler = spiHandleTypeDef;
}
void STM32EL_SPIBASE::SPI::SetSpeed(void *handler, SPI_Speed::speed speed) {
  auto spiHandleTypeDef =(SPI_HandleTypeDef*) handler;
  switch (speed){

  case SPI_Speed::speed2:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    break;
  case SPI_Speed::speed4:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    break;
  case SPI_Speed::speed8:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    break;
  case SPI_Speed::speed16:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    break;
  case SPI_Speed::speed32:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    break;
  case SPI_Speed::speed64:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    break;
  case SPI_Speed::speed128:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    break;
  case SPI_Speed::speed256:
    spiHandleTypeDef->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    break;
  }
  HAL_SPI_Init(spiHandleTypeDef);//初始化
}
u8 STM32EL_SPIBASE::SPI::ReadWriteByte(void *handler, u8 txData) {
  u8 Rxdata;
  HAL_SPI_TransmitReceive((SPI_HandleTypeDef*) handler,&txData,&Rxdata,1, 1000);
  return Rxdata; //返回收到的数据
}
void STM32EL_SPIBASE::SPI::ReadWriteByte(void *handler, u8 *txBuff, u8 *rxBuff,
                                         u8 len) {
  HAL_SPI_TransmitReceive((SPI_HandleTypeDef*) handler,txBuff,rxBuff,len, 1000);
}
void STM32EL_SPIBASE::SPI::Enable(void *handler) {
  __HAL_SPI_ENABLE((SPI_HandleTypeDef*) handler);
}
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi){
  if(hspi->Instance == SPI1){
    __HAL_RCC_SPI1_CLK_ENABLE();
  } else if(hspi->Instance == SPI2){
    __HAL_RCC_SPI2_CLK_ENABLE();
  }else if(hspi->Instance == SPI3){
    __HAL_RCC_SPI3_CLK_ENABLE();
  }else if(hspi->Instance == SPI4){
    __HAL_RCC_SPI4_CLK_ENABLE();
  }else if(hspi->Instance == SPI5){
    __HAL_RCC_SPI5_CLK_ENABLE();
  }else if(hspi->Instance == SPI6){
    __HAL_RCC_SPI6_CLK_ENABLE();
  }
}
