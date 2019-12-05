//
// Created by skjsn on 2019/9/15.
//

#include <cstdarg>
#include "Volt.h"
#include "string"
using namespace std;
string float2string(float val){
    string res;
    if(val<0){
      res.push_back('-');
    }
    res += to_string(abs((int)val));
    res.push_back('.');

    float a = abs(val - (int)val);
    for(int i = 0;i<7;i++){
        a*=10;
        int zs = (int)a%10;
        res+=to_string(zs);
    }
    return res;
}


void STM32EL::Volt::setOutStream(STM32EL::OutPutStream* outPutStream) {
    out = outPutStream;
}

void STM32EL::Volt::put(const char *data) {
    strcat(buffer,data);
    strcat(buffer,",");
}

void STM32EL::Volt::put(float data) {

    strcat(buffer,float2string(data).data());
    strcat(buffer,",");
    NOP();
}

void STM32EL::Volt::put(double data) {
    strcat(buffer,float2string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::put(int data) {
    strcat(buffer,std::to_string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::put(short data) {
    strcat(buffer,std::to_string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::put(u16 data) {
    strcat(buffer,std::to_string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::put(u32 data) {
    strcat(buffer,std::to_string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::put(u8 data) {
    strcat(buffer,std::to_string(data).data());
    strcat(buffer,",");
}

void STM32EL::Volt::finish() {
    buffer[strlen(buffer) - 1] = 0;
    strcat(buffer,"\r\n");
    out->put((u8*)buffer,strlen(buffer));
    NOP();
    memset(buffer,0,200);

}

void STM32EL::Volt::setInStream(STM32EL::InPutStream *inPutStream) {
    in = inPutStream;
}

int STM32EL::Volt::getInPutData(u8 *buffer) {
    return in->get(buffer);
}

int STM32EL::Volt::operator>>(u8 *buffer) {
    return getInPutData(buffer);
}