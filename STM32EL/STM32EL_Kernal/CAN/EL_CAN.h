//
// Created by skjsn on 2019/9/25.
//

#ifndef STM32EL_STM32F427IIH6_EL_CAN_H
#define STM32EL_STM32F427IIH6_EL_CAN_H

#include "stm32el_sys.h"
#include "EL_IOPort.h"

namespace STM32EL_CAN{
class CanBase{
protected:
  u32 base{};
  STM32EL::AfioType afioType{};

public:
  STM32EL::AfioType getAfioType() const;

public:
  u32 getBase() const;

};

class Can1:public CanBase{
public:
  Can1();
};

class Can2:public CanBase{
public:
  Can2();
};

class Can{
public:
  static void Init(u32 base,void** handler,u8 BS1,u8 BS2,u8 psc);
  static void AutoBaud(u32 baud,u8* psc,u8* bs1,u8* bs2);
  static void SetNormalMode(void* handler);
  static void SetLoopBackMode(void* handler);
  static void SendData(void* handler,u16 id,u8* data);
  static u16 ReceiveData(void* handler,u8 *buf);
  static u32 GetFreeTxMail(void* handler);
};
}
namespace STM32EL{
template <class C>
class CanBus {
private:
  void* handler{};

public:
  CanBus(u32 baud,const char* txPort,const char* rxPort);
  void sendData(u16 destination,u8* data);
  u16 receiveDate(u8* data);
};
template <class C>
CanBus<C>::CanBus(u32 baud,const char* txPort,const char* rxPort) {
  u8 psc,bs1,bs2;
  auto base = (STM32EL_CAN::CanBase*)new C;
  Afio::Register(txPort,base->getAfioType());
  Afio::Register(rxPort,base->getAfioType());
  STM32EL_CAN::Can::AutoBaud(baud,&psc,&bs1,&bs2);
  STM32EL_CAN::Can::Init(base->getBase(),&handler,bs1,bs2,psc);
}
template <class C>
void CanBus<C>::sendData(u16 destination, u8 *data) {
  STM32EL_CAN::Can::SendData(handler,destination,data);
}
template <class C> u16 CanBus<C>::receiveDate(u8*data) {
  return STM32EL_CAN::Can::ReceiveData(handler,data);
}

}


#endif // STM32EL_STM32F427IIH6_EL_CAN_H
