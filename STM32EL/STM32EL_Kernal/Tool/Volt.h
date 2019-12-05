//
// Created by skjsn on 2019/9/15.
//

#ifndef STM32EL_STM32F427IIH6_VOLT_H
#define STM32EL_STM32F427IIH6_VOLT_H

#include <cstring>
#include <EL_InPutStream.h>
#include "EL_OutPutStream.h"

namespace STM32EL{
    class Volt {
    private:
        OutPutStream* out{};
        InPutStream* in{};
        char buffer[200] = {0};
        void put(const char* data);
        void put(float data);
        void put(double data);
        void put(int data);
        void put(short data);
        void put(u16 data);
        void put(u32 data);
        void put(u8 data);
        void finish();

    public:
        explicit Volt() = default;
        void setOutStream(OutPutStream* outPutStream);
        void setInStream(InPutStream* inPutStream);
        int getInPutData(u8* buffer);
        template <typename..._Elements> void fireWater(_Elements&&... args) {
            strcpy(buffer,"Data:");
            int arr[] = {(put(args), 0)...};
            finish();
        }

        template <typename..._Elements> void operator () (_Elements&&... args){
            strcpy(buffer,"Data:");
            int arr[] = {(put(args), 0)...};
            finish();
        }
        int operator >> (u8* buffer);
    };

}

#endif //STM32EL_STM32F427IIH6_VOLT_H
