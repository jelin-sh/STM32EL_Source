//
// Created by skjsn on 2019/9/24.
//

#ifndef STM32EL_STM32F427IIH6_EL_TIMER_H
#define STM32EL_STM32F427IIH6_EL_TIMER_H

#include "stm32el_sys.h"
#include <EL_RCC.h>
#include <EL_Thread.h>
#include <cmath>
#include "EL_IOPort.h"

namespace STM32EL_TimerBase{
    namespace Base{
        class TimerBase{
        protected:
            u32 base{};
        public:
            u32 getBase() const;
            virtual u32 getClock() const;
            TimerBase() = default;
        };
        class GeneralTimerBase:public TimerBase{
        public:
          virtual STM32EL::AfioType getAfioType() = 0;
        };
        class SeniorTimerBase:public GeneralTimerBase{};
    }
    class Timer1Base:public Base::SeniorTimerBase{
    public:
        Timer1Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer2Base:public Base::GeneralTimerBase{
    public:
        Timer2Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer3Base:public Base::GeneralTimerBase{
    public:
        Timer3Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer4Base:public Base::GeneralTimerBase{
    public:
        Timer4Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer5Base:public Base::GeneralTimerBase{
    public:
        Timer5Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer6Base:public Base::TimerBase{
    public:
        Timer6Base();
        u32 getClock() const override;
    };
    class Timer7Base:public Base::TimerBase{
    public:
        Timer7Base();
        u32 getClock() const override;
    };
    class Timer8Base:public Base::SeniorTimerBase{
    public:
        Timer8Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer9Base:public Base::GeneralTimerBase{
    public:
        Timer9Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer10Base:public Base::GeneralTimerBase{
    public:
        Timer10Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer11Base:public Base::GeneralTimerBase{
    public:
        Timer11Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer12Base:public Base::GeneralTimerBase{
    public:
        Timer12Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer13Base:public Base::GeneralTimerBase{
    public:
        Timer13Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
    class Timer14Base:public Base::GeneralTimerBase{
    public:
        Timer14Base();
        u32 getClock() const override;
        STM32EL::AfioType getAfioType() override;
    };
}
namespace STM32_Timer{
    class Timer{
    public:
        static void Init(STM32EL_TimerBase::Base::TimerBase* base,u32 psc,u32 arr,void** handle);
        static void Start(void* handle);
        static bool IsRun(void* handle);
        static u32 GetCNT(void* handle);
        static void Stop(void* handle);
        static void InitPWM(void* handle,u32 chx);
        static void StartPWM(void* handle,u32 chx);
        static void StopPWM(void* handle,u32 chx);
        static void ConfigPWM(void* handle,u32 chx,u32 val);
    };

}

namespace STM32EL{
    class TimerListener;
    template <typename C>
    class Timer{
    private:
        void* handle{};

    public:
      void *getHandle() const;
    private:
      u32 _arr;

    public:
      const Thread<TimerListener> &getListener() const;

    private:
      u32 lastCNT = 0;

      public:
        u32 getLastCnt() const;
        void setLastCnt(u32 lastCnt);
      private:
        Thread<TimerListener> listener = Thread<TimerListener>("TimerListener");
      public:
        u32 getArr() const;
        virtual void OnUpdate();
      public:
        explicit Timer(float ms);
        void run();
        void stop();
        u32 getCNT();
    };
    template<typename C>
    Timer<C>::Timer(float ms) {
        STM32EL_TimerBase::Base::TimerBase* c = new C();
        u32 Tout = ms * 1000;
        u16 Ft = c->getClock() / 1000000;
        Ft *= 2;
        u32 arr = Tout;
        u8 n = 0;
        for (; arr > 0xFFFF; n++) {
          arr /= 10;
        }
        u16 psc = Ft * pow(10, n);
        if (psc > 0xFFFF) {
          Error_Handler();
        }
        arr--;
        psc--;
        _arr = arr;
        STM32_Timer::Timer::Init(c,psc,arr,&handle);
        ((TimerListener*)listener.getTask())->setTimer((STM32EL::Timer<STM32EL_TimerBase::Base::TimerBase> *)this);
    }
    template <typename C> void Timer<C>::run() {
      STM32_Timer::Timer::Start(handle);
      listener.Login();
    }
    template <typename C> u32 Timer<C>::getCNT() { return STM32_Timer::Timer::GetCNT(handle); }
    template <typename C> void Timer<C>::stop() {
      STM32_Timer::Timer::Stop(handle);
    }
    template <typename C> u32 Timer<C>::getArr() const { return _arr; }
    template <typename C> void Timer<C>::OnUpdate() {
      NOP();
    }
    template <typename C> u32 Timer<C>::getLastCnt() const { return lastCNT; }
    template <typename C> void Timer<C>::setLastCnt(u32 lastCnt) {
      lastCNT = lastCnt;
    }
    template <typename C> void *Timer<C>::getHandle() const { return handle; }
    template <typename C>
    const Thread<TimerListener> &Timer<C>::getListener() const {
      return listener;
    }

    class TimerListener : public STM32EL::ThreadTask{
    private:
      STM32EL::Timer<STM32EL_TimerBase::Base::TimerBase> * _timer{};
    public:
      void start() override;
      void setTimer(STM32EL::Timer<STM32EL_TimerBase::Base::TimerBase> *timer);
    };


}
#endif //STM32EL_STM32F427IIH6_EL_TIMER_H
