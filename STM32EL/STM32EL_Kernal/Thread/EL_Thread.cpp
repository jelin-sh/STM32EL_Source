//
// Created by skjsn on 2019/8/27.
//

#include "EL_Thread.h"
#include "FreeRTOS.h"
#include "task.h"

void STM32EL::FreeRTOS::StartScheduler() {
    vTaskStartScheduler();
}

void STM32EL::FreeRTOS::Delay(u16 ms) {
    vTaskDelay(ms);
}

void STM32EL::FreeRTOS::TaskFunction(void* ptr) {
    auto task = static_cast<ThreadTask*>(ptr);
    task->start();
    DeleteThread(task->getThreadHandle());
}

void STM32EL::FreeRTOS::SuspendThread(Thread<ThreadTask> *handle) {
    vTaskSuspend((TaskHandle_t)handle->getHandle());
}

bool STM32EL::FreeRTOS::CreateTask(const char *name, u32 stackSize, void *parm, u32 priority,
                               void **handle) {
    return xTaskCreate(
            TaskFunction,
            name,
            stackSize,
            parm,
            priority,
            reinterpret_cast<TaskHandle_t *const>(handle)
    ) == pdPASS;
}

void STM32EL::FreeRTOS::DeleteThread(STM32EL::Thread<STM32EL::ThreadTask> *handle) {
    vTaskDelete((TaskHandle_t)handle->getHandle());
}

void STM32EL::FreeRTOS::ResumeThread(STM32EL::Thread<STM32EL::ThreadTask> *handle) {
    vTaskResume((TaskHandle_t)handle->getHandle());
}

void STM32EL::FreeRTOS::DeleteThread(void *handle) {
    vTaskDelete((TaskHandle_t)handle);
}

void STM32EL::FreeRTOS::SuspendThread(void *handle) {
    vTaskSuspend((TaskHandle_t)handle);
}

void STM32EL::FreeRTOS::ResumeThread(void *handle) {
    vTaskResume((TaskHandle_t)handle);
}

void *STM32EL::ThreadTask::getThreadHandle() const {
    return threadHandle;
}

void STM32EL::ThreadTask::setThreadHandle(void *_threadHandle) {
    ThreadTask::threadHandle = _threadHandle;
}
void STM32EL::Delay::ms(u16 ms) {
  FreeRTOS::Delay(ms);
}
