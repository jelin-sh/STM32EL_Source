//
// Created by skjsn on 2019/9/25.
//

#ifndef STM32EL_STM32F427IIH6_EL_SPI_H
#define STM32EL_STM32F427IIH6_EL_SPI_H

#include "stm32el_sys.h"
#include "EL_IOPort.h"

namespace SPI_Speed{
enum speed{
  speed2,
  speed4,
  speed8,
  speed16,
  speed32,
  speed64,
  speed128,
  speed256,
};
}
namespace STM32EL_SPIBASE{
class SPI_Base{
protected:
  u32 base{};

public:
  u32 getBase() const;
protected:
  STM32EL::AfioType afType{};

public:
  STM32EL::AfioType getAfType() const;

public:
  SPI_Base() = default;
};

class SPI1_Base:public SPI_Base{
public:
  SPI1_Base();
};
class SPI2_Base:public SPI_Base{
public:
  SPI2_Base();
};
class SPI3_Base:public SPI_Base{
public:
  SPI3_Base();
};
class SPI4_Base:public SPI_Base{
public:
  SPI4_Base();
};
class SPI5_Base:public SPI_Base{
public:
  SPI5_Base();
};
class SPI6_Base:public SPI_Base{
public:
  SPI6_Base();
};
class SPI{
public:
  static void Init(u32 Instance,void** handler,bool isMaster,bool CPOL,bool CPHA,bool MSB,bool bit8);
  static void SetSpeed(void* handler,SPI_Speed::speed);
  static u8 ReadWriteByte(void* handler,u8 txData);
  static void ReadWriteByte(void* handler,u8* txBuff,u8*rxBuff,u8 len);
  static void Enable(void* handler);
};
}

namespace STM32EL{
template <class T>
class SPI {
private:
  void* handler{};
  STM32EL_SPIBASE::SPI_Base* base;
public:
  void setMaster(bool master);
  void setCPOL(bool cpol);
  void setCPHA(bool cpha);
  void setMSB(bool msb);
  void setBit8(bool bit8);
private:
  bool Master = true;
  bool CPOL = true;
  bool CPHA = true;
  bool MSB = true;
  bool Bit8 = true;

public:
  SPI(const char* SCLK,const char* MISO,const char* MOSI);
  void setSpeed(SPI_Speed::speed speed);
  u8 readWriteByte(u8 txData);
  void readWriteBytes(u8* txBuff,u8* rxBuff,u8 len);
  void enable();

private:
  void Init();

};
template <class T>
SPI<T>::SPI(const char *SCLK, const char *MISO, const char *MOSI) {
  base = new T();
  Afio::Register(SCLK,base->getAfType());
  Afio::Register(MISO,base->getAfType());
  Afio::Register(MOSI,base->getAfType());
  Init();
}
template <class T> void SPI<T>::setMaster(bool master) { Master = master; Init();}
template <class T> void SPI<T>::setCPOL(bool cpol) { CPOL = cpol; Init();}
template <class T> void SPI<T>::setCPHA(bool cpha) { CPHA = cpha; Init();}
template <class T> void SPI<T>::setMSB(bool msb) { MSB = msb; Init();}
template <class T> void SPI<T>::setBit8(bool bit8) { Bit8 = bit8; Init();}
template <class T> void SPI<T>::Init() {
  STM32EL_SPIBASE::SPI::Init(base->getBase(),&handler, Master, CPOL, CPHA,MSB,Bit8);
}
template <class T> void SPI<T>::setSpeed(SPI_Speed::speed speed) {
  STM32EL_SPIBASE::SPI::SetSpeed(handler,speed);
}
template <class T> u8 SPI<T>::readWriteByte(u8 txData) { return STM32EL_SPIBASE::SPI::ReadWriteByte(handler,txData); }
template <class T>
void SPI<T>::readWriteBytes(u8 *txBuff, u8 *rxBuff, u8 len) {
  STM32EL_SPIBASE::SPI::ReadWriteByte(handler,txBuff,rxBuff,len);
}
template <class T> void SPI<T>::enable() {
  STM32EL_SPIBASE::SPI::Enable(handler);
}
}


#endif // STM32EL_STM32F427IIH6_EL_SPI_H
