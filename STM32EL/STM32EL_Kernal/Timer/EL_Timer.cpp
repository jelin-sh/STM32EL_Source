//
// Created by skjsn on 2019/9/24.
//


#include "EL_Timer.h"
#include "stm32f4xx_hal.h"


STM32EL_TimerBase::Timer1Base::Timer1Base() {
    base = TIM1_BASE;
}
u32 STM32EL_TimerBase::Timer1Base::getClock() const {
  return STM32EL::Rcc::GetAPB2Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer1Base::getAfioType() {
  return STM32EL::AfioType::tim1;
}

STM32EL_TimerBase::Timer2Base::Timer2Base() {
    base = TIM2_BASE;
}
u32 STM32EL_TimerBase::Timer2Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer2Base::getAfioType() {
  return STM32EL::AfioType::tim2;
}

STM32EL_TimerBase::Timer3Base::Timer3Base() {
    base = TIM3_BASE;
}
u32 STM32EL_TimerBase::Timer3Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer3Base::getAfioType() { return STM32EL::AfioType::tim3; }

STM32EL_TimerBase::Timer4Base::Timer4Base() {
    base = TIM4_BASE;
}
u32 STM32EL_TimerBase::Timer4Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer4Base::getAfioType() {return STM32EL::AfioType::tim4; }

STM32EL_TimerBase::Timer5Base::Timer5Base() {
    base = TIM5_BASE;
}
u32 STM32EL_TimerBase::Timer5Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer5Base::getAfioType() { return STM32EL::AfioType::tim5; }

STM32EL_TimerBase::Timer6Base::Timer6Base() {
    base = TIM6_BASE;
}
u32 STM32EL_TimerBase::Timer6Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}

STM32EL_TimerBase::Timer7Base::Timer7Base() {
    base = TIM7_BASE;
}
u32 STM32EL_TimerBase::Timer7Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}

STM32EL_TimerBase::Timer8Base::Timer8Base() {
    base = TIM8_BASE;
}
u32 STM32EL_TimerBase::Timer8Base::getClock() const {
  return STM32EL::Rcc::GetAPB2Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer8Base::getAfioType() { return STM32EL::AfioType::tim8; }

STM32EL_TimerBase::Timer9Base::Timer9Base() {
    base = TIM9_BASE;
}
u32 STM32EL_TimerBase::Timer9Base::getClock() const {
  return STM32EL::Rcc::GetAPB2Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer9Base::getAfioType() { return STM32EL::AfioType::tim9; }

STM32EL_TimerBase::Timer10Base::Timer10Base() {
    base = TIM10_BASE;
}
u32 STM32EL_TimerBase::Timer10Base::getClock() const {
  return STM32EL::Rcc::GetAPB2Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer10Base::getAfioType() { return STM32EL::AfioType::tim10; }

STM32EL_TimerBase::Timer11Base::Timer11Base() {
    base = TIM11_BASE;
}
u32 STM32EL_TimerBase::Timer11Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer11Base::getAfioType() { return STM32EL::AfioType::tim11; }

STM32EL_TimerBase::Timer12Base::Timer12Base() {
    base = TIM12_BASE;
}
u32 STM32EL_TimerBase::Timer12Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer12Base::getAfioType() { return STM32EL::AfioType::tim12; }

STM32EL_TimerBase::Timer13Base::Timer13Base() {
    base = TIM13_BASE;
}
u32 STM32EL_TimerBase::Timer13Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer13Base::getAfioType() { return STM32EL::AfioType::tim13; }

STM32EL_TimerBase::Timer14Base::Timer14Base() {
    base = TIM14_BASE;
}
u32 STM32EL_TimerBase::Timer14Base::getClock() const {
  return STM32EL::Rcc::GetAPB1Clock();
}
STM32EL::AfioType STM32EL_TimerBase::Timer14Base::getAfioType() { return STM32EL::AfioType::tim14; }

void STM32_Timer::Timer::Init(STM32EL_TimerBase::Base::TimerBase* base, u32 psc, u32 arr, void **handle) {
    auto* mHandle = new TIM_HandleTypeDef;
    mHandle->Instance = reinterpret_cast<TIM_TypeDef *>(base->getBase());
    mHandle->Init.Period = arr;
    mHandle->Init.Prescaler = psc;
    mHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
    mHandle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(mHandle);
    *handle = mHandle;
}

void STM32_Timer::Timer::Start(void *handle) {
    HAL_TIM_Base_Start((TIM_HandleTypeDef*) handle);
}

bool STM32_Timer::Timer::IsRun(void *handle) {
   return GetCNT(handle) != 0;
}
u32 STM32_Timer::Timer::GetCNT(void *handle) {
  return ((TIM_HandleTypeDef*) handle)->Instance->CNT;
}
void STM32_Timer::Timer::Stop(void *handle) {
  HAL_TIM_Base_Stop((TIM_HandleTypeDef*)handle);
}
void STM32_Timer::Timer::InitPWM(void *handle,u32 chx) {
  TIM_OC_InitTypeDef pwmHandler;
  HAL_TIM_PWM_Init((TIM_HandleTypeDef*)handle);
  pwmHandler.OCMode=TIM_OCMODE_PWM2; //模式选择 PWM2
  pwmHandler.Pulse=0; //设置比较值,此值用来确定占空比
  pwmHandler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低
  HAL_TIM_PWM_ConfigChannel((TIM_HandleTypeDef*)handle,&pwmHandler,chx);
}
void STM32_Timer::Timer::StartPWM(void *handle, u32 chx) {
  HAL_TIM_PWM_Start((TIM_HandleTypeDef*)handle,chx);
}
void STM32_Timer::Timer::StopPWM(void *handle, u32 chx) {
  HAL_TIM_PWM_Stop((TIM_HandleTypeDef*)handle,chx);
}
void STM32_Timer::Timer::ConfigPWM(void *handle, u32 chx, u32 val) {
  __HAL_TIM_SET_COMPARE((TIM_HandleTypeDef*)handle,chx,val);
}

u32 STM32EL_TimerBase::Base::TimerBase::getBase() const {
    return base;
}
u32 STM32EL_TimerBase::Base::TimerBase::getClock() const {
  return 0;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM1){
        __HAL_RCC_TIM1_CLK_ENABLE();
    } else if(htim->Instance == TIM2){
        __HAL_RCC_TIM2_CLK_ENABLE();
    }else if(htim->Instance == TIM3){
        __HAL_RCC_TIM3_CLK_ENABLE();
    }else if(htim->Instance == TIM4){
        __HAL_RCC_TIM4_CLK_ENABLE();
    }else if(htim->Instance == TIM5){
        __HAL_RCC_TIM5_CLK_ENABLE();
    }else if(htim->Instance == TIM6){
        __HAL_RCC_TIM6_CLK_ENABLE();
    }else if(htim->Instance == TIM7){
        __HAL_RCC_TIM7_CLK_ENABLE();
    }else if(htim->Instance == TIM8){
        __HAL_RCC_TIM8_CLK_ENABLE();
    }else if(htim->Instance == TIM9){
        __HAL_RCC_TIM9_CLK_ENABLE();
    }else if(htim->Instance == TIM10){
        __HAL_RCC_TIM10_CLK_ENABLE();
    }else if(htim->Instance == TIM11){
        __HAL_RCC_TIM12_CLK_ENABLE();
    }else if(htim->Instance == TIM13){
        __HAL_RCC_TIM13_CLK_ENABLE();
    }else if(htim->Instance == TIM14){
        __HAL_RCC_TIM14_CLK_ENABLE();
    }
}

void STM32EL::TimerListener::start() {
  for(;;){
    if(_timer->getLastCnt() >= _timer->getCNT()){
      _timer->OnUpdate();
    }
    _timer->setLastCnt(_timer->getCNT());
    Delay::ms(1);
  }
}
void STM32EL::TimerListener::setTimer(
    STM32EL::Timer<STM32EL_TimerBase::Base::TimerBase> *timer) {
  _timer = timer;
}
