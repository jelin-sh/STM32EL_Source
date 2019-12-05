//
// Created by skjsn on 2019/8/26.
//

#include "FreeRTOS.h"
#include "Volt.h"
#include "stm32el.h"
#include "stm32f4xx_hal.h"
#include "task.h"
#include <EL_CAN.h>
#include <EL_WatchDog.h>

using namespace STM32EL;

__WEAK int Main() {
    __NOP();
}

class EL_UserMain : public ThreadTask {
public:
    void start() override {
        Main();
    }
};

void *operator new(size_t size) {
    return pvPortMalloc(size);
}

void operator delete(void *ptr) {
    vPortFree(ptr);
}

void operator delete[](void *ptr) {
    vPortFree(ptr);
}


extern "C" {
void xPortSysTickHandler(void);
void SysTick_Handler(void) {
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();
    }
    HAL_IncTick();
}

void *malloc(size_t size) {
    return pvPortMalloc(size);
}
void free(void *ptr) {
    vPortFree(ptr);
}
}

class LogStream:public OutPutStream,public InPutStream{
private:
    SerialPort<Usart1> logPort = SerialPort<Usart1>(115200, "PA9", "PA10");
public:
    void put(u8 data) override {
        logPort.sendData(&data,1);
    }

    void put(u8 *data, u32 length) override {
        logPort.sendData(data,length);
    }

    int get(u8 *buffer) override {
        return logPort.receiveData(buffer);
    }
};

Volt volt;
int main() {
    HAL_Init();
    Rcc::SystemClockConfig(360, 12, 2, 8);
    auto log =  new LogStream();
    volt.setOutStream(log);
    volt.setInStream(log);
    WatchDog::Init();
    /**创建用户主任务**/
    Thread<EL_UserMain> userMain("UserMain", Main_TOTAL_HEAP_SIZE);
    userMain.Login();
    /**进入系统托管**/
    FreeRTOS::StartScheduler();
}

void Error_Handler()
{
  while(true){
    NOP();
  }
}