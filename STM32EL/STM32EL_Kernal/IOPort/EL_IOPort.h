//
// Created by skjsn on 2019/8/27.
//

#ifndef STM32EL_STM32F427IIH6_EL_IOPORT_H
#define STM32EL_STM32F427IIH6_EL_IOPORT_H

#include "EL_GpioRegisterErr.h"
#include "stm32el_sys.h"


namespace STM32EL {
enum class GpioType { NO = -1, PA = 0, PB, PC, PD, PE, PF, PG, PH, PI };
enum class GpioPin {
  NO = -1,
  Pin0 = 0,
  Pin1,
  Pin2,
  Pin3,
  Pin4,
  Pin5,
  Pin6,
  Pin7,
  Pin8,
  Pin9,
  Pin10,
  Pin11,
  Pin12,
  Pin13,
  Pin14,
  Pin15
};
enum class GpioMode {
  In = 0x00000000U,               /*!< Input Floating Mode                   */
  Out_PP = 0x00000001U,           /*!< Output Push Pull Mode                 */
  Out_OD = 0x00000011U,           /*!< Output Open Drain Mode                */
  Af_PP = 0x00000002U,            /*!< Alternate Function Push Pull Mode     */
  Af_OD = 0x00000012U,            /*!< Alternate Function Open Drain Mode    */
  Analog = 0x00000003U,           /*!< Analog Mode  */
  IT_Rising = 0x10110000U,        /*!< External Interrupt Mode with Rising edge
                                     trigger detection  */
  IT_Falling = 0x10210000U,       /*!< External Interrupt Mode with Falling edge
                                   * trigger detection
                                   */
  IT_RisingFalling = 0x10310000U, /*!< External Interrupt Mode with
                       Rising/Falling edge trigger detection  */
  EVT_Rising = 0x10120000U,  /*!< External Event Mode with Rising edge trigger
                                detection  */
  EVT_Falling = 0x10220000U, /*!< External Event Mode with Falling edge
                                trigger detection */
  EVT_RisingFalling = 0x10320000U /*!< External Event Mode with Rising/Falling
                       edge trigger detection       */
};
enum class GpioSpeed { Low = 0, Medium, High, VeryHigh };
enum class GpioPull { NoPull = 0, PullDown, PullUp };
enum class AfioType {
  no = 0,
  tim1 = 0x01,
  tim2 = 0x01,
  tim3 = 0x02,
  tim4 = 0x02,
  tim5 = 0x02,
  tim8 = 0x03,
  tim9 = 0x03,
  tim10 = 0x03,
  tim11 = 0x03,
  i2c1 = 0x04,
  i2c2 = 0x04,
  i2c3 = 0x04,
  spi1 = 0x05,
  spi2 = 0x05,
  spi3 = 0x05,
  spi4 = 0x05,
  spi5 = 0x05,
  spi6 = 0x05,
  i2s3ext = 0x05,
  i2s2ext = 0x06,
  sai1 = 0x06,
  usart1 = 0x07,
  usart2 = 0x07,
  usart3 = 0x07,
  uart4 = 0x08,
  uart5 = 0x08,
  usart6 = 0x08,
  uart7 = 0x08,
  uart8 = 0x08,
  can1 = 0x09,
  can2 = 0x09,
  tim12 = 0x09,
  tim13 = 0x09,
  tim14 = 0x09,
  otg_fs = 0x0A,
  otg_hs = 0x0A,
  eth = 0x0B,
  fmc = 0x0C,
  otg_hs_fs = 0x0C,
  sdio = 0x0C,
  dcmi = 0x0D,
  eventout = 0x0F
};

class IOPort {
public:
  void setSpeed(GpioSpeed speed);
  void setMode(GpioMode mode);
  void setPull(GpioPull pull);

private:
  GpioMode mode = GpioMode ::Out_PP;
  GpioType type = GpioType ::NO;
  GpioPin pin = GpioPin ::NO;
  GpioPull pull = GpioPull ::NoPull;
  AfioType afType = AfioType::no;
  GpioSpeed speed = GpioSpeed::High;
public:
  IOPort(GpioType _type, GpioPin _pin, GpioMode _mode = GpioMode ::Out_PP,
         AfioType _af = AfioType::no);
  IOPort() = default;
  void OutStatus(bool status);
  bool GetStatus();

  IOPort& operator = (bool status);
private:
  u32 *GetSysType();
  u32 GetSysPin();
  void Init();
};

class Gpio {
public:
  static IOPort Register(const char* Port) noexcept;
};

class Afio {
public:
  static IOPort Register(const char* Port, AfioType type);
};

} // namespace STM32EL

#endif // STM32EL_STM32F427IIH6_EL_IOPORT_H
