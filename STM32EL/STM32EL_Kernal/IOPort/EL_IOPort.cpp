//
// Created by skjsn on 2019/8/27.
//

#include "EL_IOPort.h"
#include "EL_RCC.h"
#include "stm32f4xx_hal.h"
#include <cstring>
#include <cstdlib>

STM32EL::IOPort STM32EL::Gpio::Register(const char* Port) noexcept {
  GpioType type = GpioType ::NO;
  GpioPin pin = GpioPin ::NO;
  if (Port[0] == 'P') {
    int a = Port[1] - 'A';
    if ((a <= (int)GpioType::PI) && (a >= 0)) {
      type = static_cast<GpioType>(a);
    }
    char *tem = new char[100];
    memset(tem,0,100);
    strncpy(tem, Port + 2, strlen(Port) - 2);
    int b = atoi(tem);
    delete[] tem;
    if (b >= 0 && b <= (int)GpioPin ::Pin15) {
      pin = static_cast<GpioPin>(b);
    }
  }
  if (type == GpioType::NO || pin == GpioPin::NO) {
    throw EL_GpioRegisterErr();
  }
  return {type, pin};
}

STM32EL::IOPort::IOPort(STM32EL::GpioType _type, STM32EL::GpioPin _pin,
                        STM32EL::GpioMode _mode,AfioType _af) {
  type = _type;
  pin = _pin;
  mode = _mode;
  afType = _af;
  Rcc::GPIOClockEnable(type);
  Init();
}
u32 *STM32EL::IOPort::GetSysType() {
  return (u32 *)(AHB1PERIPH_BASE + (0x400UL * static_cast<u8>(type)));
}
void STM32EL::IOPort::OutStatus(bool status) {
  HAL_GPIO_WritePin((GPIO_TypeDef *)GetSysType(), GetSysPin(),
                    (GPIO_PinState)status);
}
u32 STM32EL::IOPort::GetSysPin() {
    return static_cast<uint32_t>(1UL << static_cast<u8>(pin));
}
void STM32EL::IOPort::Init() {
  GPIO_InitTypeDef initTypeDef;
  initTypeDef.Speed = static_cast<uint32_t>(speed);
  initTypeDef.Mode = static_cast<uint32_t>(mode);
  initTypeDef.Pin = GetSysPin();
  initTypeDef.Pull = static_cast<uint32_t>(pull);
  initTypeDef.Alternate = static_cast<uint32_t>(afType);
  HAL_GPIO_Init((GPIO_TypeDef *)GetSysType(), &initTypeDef);
}
bool STM32EL::IOPort::GetStatus() {
  return static_cast<bool>(
      HAL_GPIO_ReadPin((GPIO_TypeDef *)GetSysType(), GetSysPin()));
}
void STM32EL::IOPort::setSpeed(STM32EL::GpioSpeed speed) {
  IOPort::speed = speed;
  Init();
}
void STM32EL::IOPort::setMode(STM32EL::GpioMode mode) {
  IOPort::mode = mode;
  Init();
}
void STM32EL::IOPort::setPull(STM32EL::GpioPull pull) {
  IOPort::pull = pull;
  Init();
}

STM32EL::IOPort& STM32EL::IOPort::operator=(bool status) {
    OutStatus(status);
    return *this;
}

STM32EL::IOPort STM32EL::Afio::Register(const char* Port, STM32EL::AfioType _type) {
  GpioType type = GpioType ::NO;
  GpioPin pin = GpioPin ::NO;
  if (Port[0] == 'P') {
    int a = Port[1] - 'A';
    if ((a <= (int)GpioType::PI) && (a >= 0)) {
      type = static_cast<GpioType>(a);
    }
    char *tem = new char[100];
    strncpy(tem, Port + 2, strlen(Port) - 2);
    int b = atoi(tem);
    delete[] tem;
    if (b >= 0 && b <= (int)GpioPin ::Pin15) {
      pin = static_cast<GpioPin>(b);
    }
  }
  if (type == GpioType::NO || pin == GpioPin::NO) {
    throw EL_GpioRegisterErr();
  }
  return {type, pin,GpioMode::Af_PP,_type};
}
