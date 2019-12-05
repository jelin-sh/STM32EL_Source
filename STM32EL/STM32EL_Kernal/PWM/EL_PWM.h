//
// Created by skjsn on 2019/9/24.
//

#ifndef STM32EL_STM32F427IIH6_EL_PWM_H
#define STM32EL_STM32F427IIH6_EL_PWM_H

#include "stm32el_sys.h"
#include "EL_Timer.h"

namespace STM32EL{
namespace PWM_Channel{
  enum Channel{
    CH1 = 0x00000000U,
    CH2 = 0x00000004U,
    CH3 = 0x00000008U,
    CH4 = 0x0000000CU
  };
}
template <class T>
class PWM {
private:
  Timer<T> *timer;
  PWM_Channel::Channel CHX;
public:
  explicit PWM(float ms,PWM_Channel::Channel chx,const char* port);
  void run(float rate);//max 100 min 0
  void stop();
};
template <class T>
PWM<T>::PWM(float ms, PWM_Channel::Channel chx, const char *port):CHX(chx) {
  STM32EL_TimerBase::Base::GeneralTimerBase* generalTimerBase = new T();
  timer = new Timer<T>(ms);
  Afio::Register(port,generalTimerBase->getAfioType());
  STM32_Timer::Timer::InitPWM(timer->getHandle(),(u32)chx);
  delete generalTimerBase;
}
template <class T> void PWM<T>::run(float rate) {
  if(rate>100){
    rate = 100;
  }
  if(rate < 0){
    rate = 0;
  }
  STM32_Timer::Timer::ConfigPWM(timer->getHandle(),CHX,timer->getArr()*(rate/100.0));
  STM32_Timer::Timer::StartPWM(timer->getHandle(),CHX);
}
template <class T> void PWM<T>::stop() {
  STM32_Timer::Timer::StopPWM(timer->getHandle(),CHX);
}

}

#endif // STM32EL_STM32F427IIH6_EL_PWM_H
