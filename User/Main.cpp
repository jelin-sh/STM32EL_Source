#include "MPU6500.h"
#include "stm32el.h"

using namespace STM32EL;

class BlueTooth:public OutPutStream{
private:
  SerialPort<Usart2 > serialPort = SerialPort<Usart2 >(115200,"PD5","PD6");
public:
  void put(u8 data) override {
    serialPort.sendData(&data,1);
  }
  void put(u8 *data, u32 length) override {
    serialPort.sendData(data,length);
  }
};
void Main() {
    IOPort led = Gpio::Register("PG13");
    led = 1;
    for(;;){
        Delay::ms(1);
    }
}






