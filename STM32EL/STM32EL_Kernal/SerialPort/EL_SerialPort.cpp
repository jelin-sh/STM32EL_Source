//
// Created by skjsn on 2019/9/9.
//

#include "EL_SerialPort.h"
#include "map"
#include "stm32f4xx_hal.h"

std::map<STM32EL::Uart<STM32EL_SerialBase::SerialBase> *, UART_HandleTypeDef>
    __EL_SerialPortHandleMap;

STM32EL_SerialBase::Usart1Base::Usart1Base() { base = USART1_BASE; }

STM32EL_SerialBase::Usart2Base::Usart2Base() { base = USART2_BASE; }

STM32EL_SerialBase::Usart3Base::Usart3Base() { base = USART3_BASE; }

STM32EL_SerialBase::Uart4Base::Uart4Base() { base = UART4_BASE; }

STM32EL_SerialBase::Uart5Base::Uart5Base() { base = UART5_BASE; }

STM32EL_SerialBase::Usart6Base::Usart6Base() { base = USART6_BASE; }

STM32EL_SerialBase::Uart7Base::Uart7Base() { base = UART7_BASE; }

STM32EL_SerialBase::Uart8Base::Uart8Base() { base = UART8_BASE; }

u32 STM32EL_SerialBase::SerialBase::getBase() const { return base; }

STM32EL_SerialBase::SerialBase::SerialBase() = default;

u32 STM32EL::SerialPortManager::GetMode(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart) {
  switch (uart->getMode()) {
  case SerialPort_Mode::RX:
    return UART_MODE_RX;
  case SerialPort_Mode::TX:
    return UART_MODE_TX;
  case SerialPort_Mode::RX_TX:
    return UART_MODE_TX_RX;
  }
}

u32 STM32EL::SerialPortManager::GetWordLength(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart) {
  switch (uart->getWordLength()) {
  case SerialPort_WordLength::WordLength_8:
    return UART_WORDLENGTH_8B;
  case SerialPort_WordLength::WordLength_9:
    return UART_WORDLENGTH_9B;
  }
}

u32 STM32EL::SerialPortManager::GetStopBits(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart) {
  switch (uart->getStopBits()) {
  case SerialPort_StopBits::StopBits_1:
    return UART_STOPBITS_1;
  case SerialPort_StopBits::StopBits_2:
    return UART_STOPBITS_2;
  }
}

u32 STM32EL::SerialPortManager::GetHWControl(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart) {
  switch (uart->getHwControl()) {
  case SerialPort_HWControl::None:
    return UART_HWCONTROL_NONE;
  case SerialPort_HWControl::RTS:
    return UART_HWCONTROL_RTS;
  case SerialPort_HWControl::CTS:
    return UART_HWCONTROL_CTS;
  case SerialPort_HWControl::RTS_CTS:
    return UART_HWCONTROL_RTS_CTS;
  }
}

u32 STM32EL::SerialPortManager::GetParity(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart) {
  switch (uart->getParity()) {
  case SerialPort_Parity::None:
    return UART_PARITY_NONE;
  case SerialPort_Parity::Even:
    return UART_PARITY_EVEN;
  case SerialPort_Parity::Odd:
    return UART_PARITY_ODD;
  }
}

void STM32EL::SerialPortManager::Register(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart, const char *txPort,
    const char *rxPort) {
  UART_HandleTypeDef handleTypeDef = {0};
  handleTypeDef.Instance = (USART_TypeDef * )(uart->getType());
  handleTypeDef.Init.Mode = GetMode(uart);
  handleTypeDef.Init.BaudRate = uart->getBaudRate();
  handleTypeDef.Init.HwFlowCtl = GetHWControl(uart);
  handleTypeDef.Init.Parity = GetParity(uart);
  handleTypeDef.Init.StopBits = GetStopBits(uart);
  handleTypeDef.Init.WordLength = GetWordLength(uart);
  handleTypeDef.Init.OverSampling = GetOverSampling(uart);
  HAL_UART_Init(&handleTypeDef);

  AfioType type = AfioType::no;
  if (handleTypeDef.Instance == USART1) {
    __HAL_RCC_USART1_CLK_ENABLE();
    type = AfioType::usart1;
  } else if (handleTypeDef.Instance == USART2) {
    __HAL_RCC_USART2_CLK_ENABLE();
    type = AfioType::usart2;
  } else if (handleTypeDef.Instance == USART3) {
    __HAL_RCC_USART3_CLK_ENABLE();
    type = AfioType::usart3;
  } else if (handleTypeDef.Instance == UART4) {
    __HAL_RCC_UART4_CLK_ENABLE();
    type = AfioType::uart4;
  } else if (handleTypeDef.Instance == UART5) {
    __HAL_RCC_UART5_CLK_ENABLE();
    type = AfioType::uart5;
  } else if (handleTypeDef.Instance == USART6) {
    __HAL_RCC_USART6_CLK_ENABLE();
    type = AfioType::usart6;
  } else if (handleTypeDef.Instance == UART7) {
    __HAL_RCC_UART7_CLK_ENABLE();
    type = AfioType::uart7;
  } else if (handleTypeDef.Instance == UART8) {
    __HAL_RCC_UART8_CLK_ENABLE();
    type = AfioType::uart8;
  }
  auto tx = Afio::Register(txPort, type);
  auto rx = Afio::Register(rxPort, type);
  tx.setPull(GpioPull::PullUp);
  tx.setSpeed(GpioSpeed::VeryHigh);
  rx.setPull(GpioPull::PullUp);
  rx.setSpeed(GpioSpeed::VeryHigh);
  __EL_SerialPortHandleMap[uart] = handleTypeDef;
}

void STM32EL::SerialPortManager::SendData(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart, u8 *data, u32 length) {
  __HAL_UNLOCK(&__EL_SerialPortHandleMap[uart]);
  __EL_SerialPortHandleMap[uart].gState = HAL_UART_STATE_READY;
  __HAL_UART_CLEAR_FLAG(&__EL_SerialPortHandleMap[uart],UART_FLAG_TC);
  HAL_UART_Transmit(&__EL_SerialPortHandleMap[uart], data, length, 100);
}

int STM32EL::SerialPortManager::ReceiveData(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *uart, u8 *data) {
  int len = 0;
  while (HAL_UART_Receive(&__EL_SerialPortHandleMap[uart], data++, 1, 2) == HAL_OK){
    len++;
  }
  return len;
}
bool STM32EL::SerialPortManager::ReceiveData(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *port, u8 *data, u32 length) {
  int len = 0;
  while (HAL_UART_Receive(&__EL_SerialPortHandleMap[port], data++, 1, 2) == HAL_OK){
    len++;
    if(len == length){
      break;
    }
  }
  return true;
}
u32 STM32EL::SerialPortManager::GetOverSampling(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *port) {
  switch (port->getOverSampling()) {
  case SerialPort_OverSampling::OVERSAMPLING_16:
    return UART_OVERSAMPLING_16;
  case SerialPort_OverSampling::OVERSAMPLING_8:
    return UART_OVERSAMPLING_8;
  }
}
bool STM32EL::SerialPortManager::ReceiveDataDMA(
    STM32EL::Uart<STM32EL_SerialBase::SerialBase> *port, STM32EL::DMA * dma,
    u8 *data, u32 length) {
  DMA_HandleTypeDef* dmaHandle =
      static_cast<DMA_HandleTypeDef *>(dma->getHandle());
  __HAL_LINKDMA(&__EL_SerialPortHandleMap[port],hdmarx,*dmaHandle);
  HAL_UART_DMAStop(&__EL_SerialPortHandleMap[port]);
  HAL_UART_Receive_DMA(&__EL_SerialPortHandleMap[port],data,length);
  return true;
}

extern "C" void HAL_UART_MspInit(UART_HandleTypeDef *handleTypeDef) {
  if (handleTypeDef->Instance == USART1) {
    __HAL_RCC_USART1_CLK_ENABLE();
  } else if (handleTypeDef->Instance == USART2) {
    __HAL_RCC_USART2_CLK_ENABLE();
  } else if (handleTypeDef->Instance == USART3) {
    __HAL_RCC_USART3_CLK_ENABLE();
  } else if (handleTypeDef->Instance == UART4) {
    __HAL_RCC_UART4_CLK_ENABLE();
  } else if (handleTypeDef->Instance == UART5) {
    __HAL_RCC_UART5_CLK_ENABLE();
  } else if (handleTypeDef->Instance == USART6) {
    __HAL_RCC_USART6_CLK_ENABLE();
  } else if (handleTypeDef->Instance == UART7) {
    __HAL_RCC_UART7_CLK_ENABLE();
  } else if (handleTypeDef->Instance == UART8) {
    __HAL_RCC_UART8_CLK_ENABLE();
  }
}