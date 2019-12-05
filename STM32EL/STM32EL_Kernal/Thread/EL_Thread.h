//
// Created by skjsn on 2019/8/27.
//

#ifndef STM32EL_STM32F427IIH6_EL_THREAD_H
#define STM32EL_STM32F427IIH6_EL_THREAD_H

#include "stm32el_sys.h"
namespace STM32EL {
    /*线程任务接口类*/
    class ThreadTask{
    private:
        void* threadHandle = nullptr;
    public:
        void setThreadHandle(void *threadHandle);

    public:
        void *getThreadHandle() const;

    public:
        virtual void start() = 0;
    };

    /*线程执行类*/
    template <typename T>
    class Thread {
    private:
        ThreadTask* task = nullptr;

    public:
      ThreadTask *getTask() const;

    private:
        void * handle = nullptr;
    public:
        void *getHandle();
    public:
        explicit Thread(char* name,u16 stack = 128,u8 priority = 3);
        void Login();
    };

    /**FreeRTOS封装类**/
    class FreeRTOS{
    public:
        static void StartScheduler();
        static void Delay(u16 ms);
        static bool CreateTask(
                const char* name,
                u32 stackSize,
                void* parm,
                u32 priority,
                void** handle
                );
        static void SuspendThread(Thread<ThreadTask>* handle);
        static void ResumeThread(Thread<ThreadTask>* handle);
        static void DeleteThread(Thread<ThreadTask>* handle);

        static void SuspendThread(void* handle);
        static void ResumeThread(void* handle);
        static void DeleteThread(void* handle);

        static void TaskFunction(void* ptr);
    };

    class Delay{
    public:
      static void us(u16 us);
      static void ms(u16 ms);
    };

    template<typename T>
    Thread<T>::Thread(char *name, u16 stack, u8 priority) {
        task = new T();
        if(FreeRTOS::CreateTask(name,stack,task,priority,&handle)){
            FreeRTOS::SuspendThread((Thread<ThreadTask>*)this);
            task->setThreadHandle(handle);
        }
    }

    template<typename T>
    void *Thread<T>::getHandle(){
        return handle;
    }

    template<typename T>
    void Thread<T>::Login() {
        FreeRTOS::ResumeThread((Thread<ThreadTask>*)this);
    }
    template <typename T> ThreadTask *Thread<T>::getTask() const {
      return task;
    }
}


#endif //STM32EL_STM32F427IIH6_EL_THREAD_H
