//
// Created by skjsn on 2019/9/9.
//

#ifndef STM32EL_STM32F427IIH6_EL_SERIALPORT_H
#define STM32EL_STM32F427IIH6_EL_SERIALPORT_H

#include "EL_IOPort.h"
#include "EL_DMA.h"
#include <cstring>
#include <stm32el_sys.h>

namespace STM32EL_SerialBase {
class SerialBase {
protected:
  u32 base{};

public:
  u32 getBase() const;

public:
  SerialBase();
};
class Usart1Base : public SerialBase {
public:
  Usart1Base();
};
class Usart2Base : public SerialBase {
public:
  Usart2Base();
};
class Usart3Base : public SerialBase {
public:
  Usart3Base();
};
class Uart4Base : public SerialBase {
public:
  Uart4Base();
};
class Uart5Base : public SerialBase {
public:
  Uart5Base();
};
class Usart6Base : public SerialBase {
public:
  Usart6Base();
};
class Uart7Base : public SerialBase {
public:
  Uart7Base();
};
class Uart8Base : public SerialBase {
public:
  Uart8Base();
};
} // namespace STM32EL_SerialBase

namespace STM32EL {
enum class SerialPort_WordLength { WordLength_8, WordLength_9 };
enum class SerialPort_StopBits { StopBits_1, StopBits_2 };
enum class SerialPort_Parity { None, Even, Odd };
enum class SerialPort_HWControl { None, RTS, CTS, RTS_CTS };
enum class SerialPort_Mode { RX, TX, RX_TX };
enum class SerialPort_OverSampling{OVERSAMPLING_16,OVERSAMPLING_8};
template <class T> class Uart {
private:
public:
  u32 getBaudRate() const;
  void setBaudRate(u32 baudRate);
  SerialPort_WordLength getWordLength() const;
  void setWordLength(SerialPort_WordLength wordLength);
  SerialPort_HWControl getHwControl() const;
  void setHwControl(SerialPort_HWControl hwControl);
  SerialPort_StopBits getStopBits() const;
  void setStopBits(SerialPort_StopBits stopBits);
  SerialPort_Parity getParity() const;
  void setParity(SerialPort_Parity parity);
  SerialPort_Mode getMode() const;
  void setMode(SerialPort_Mode mode);

private:
  u32 baudRate = 115200;
  SerialPort_WordLength wordLength = SerialPort_WordLength::WordLength_8;
  SerialPort_HWControl hwControl = SerialPort_HWControl ::None;
  SerialPort_StopBits stopBits = SerialPort_StopBits ::StopBits_1;
  SerialPort_Parity parity = SerialPort_Parity ::None;
  SerialPort_Mode mode = SerialPort_Mode ::RX_TX;
  SerialPort_OverSampling overSampling = SerialPort_OverSampling ::OVERSAMPLING_8;

public:
  SerialPort_OverSampling getOverSampling() const;
  void setOverSampling(SerialPort_OverSampling overSampling);
private:
  u32 *type;

public:
  u32 *getType() const;

public:
  Uart();
  explicit Uart(u32 _baudRate);
};
template <class T> Uart<T>::Uart() {
  STM32EL_SerialBase::SerialBase *base = new T();
  type = (u32 *)base->getBase();
}
template <class T> u32 Uart<T>::getBaudRate() const { return baudRate; }
template <class T> void Uart<T>::setBaudRate(u32 baudRate) {
  Uart::baudRate = baudRate;
}
template <class T> SerialPort_WordLength Uart<T>::getWordLength() const {
  return wordLength;
}
template <class T>
void Uart<T>::setWordLength(SerialPort_WordLength wordLength) {
  Uart::wordLength = wordLength;
}
template <class T> SerialPort_HWControl Uart<T>::getHwControl() const {
  return hwControl;
}
template <class T> void Uart<T>::setHwControl(SerialPort_HWControl hwControl) {
  Uart::hwControl = hwControl;
}
template <class T> SerialPort_StopBits Uart<T>::getStopBits() const {
  return stopBits;
}
template <class T> void Uart<T>::setStopBits(SerialPort_StopBits stopBits) {
  Uart::stopBits = stopBits;
}
template <class T> SerialPort_Parity Uart<T>::getParity() const {
  return parity;
}
template <class T> void Uart<T>::setParity(SerialPort_Parity parity) {
  Uart::parity = parity;
}
template <class T> SerialPort_Mode Uart<T>::getMode() const { return mode; }
template <class T> void Uart<T>::setMode(SerialPort_Mode mode) {
  Uart::mode = mode;
}
template <class T> u32 *Uart<T>::getType() const { return type; }
template <class T> Uart<T>::Uart(u32 _baudRate) {
  baudRate = _baudRate;
  STM32EL_SerialBase::SerialBase *base = new T();
  type = (u32 *)base->getBase();
}
template <class T> SerialPort_OverSampling Uart<T>::getOverSampling() const {
  return overSampling;
}
template <class T>
void Uart<T>::setOverSampling(SerialPort_OverSampling overSampling) {
  Uart::overSampling = overSampling;
}
class SerialPortManager {
public:
  static void Register(Uart<STM32EL_SerialBase::SerialBase>* port,const char* txPort,const char* rxPort);
  static void SendData(Uart<STM32EL_SerialBase::SerialBase>* port,u8* data,u32 length);
  static int ReceiveData(Uart<STM32EL_SerialBase::SerialBase>* port,u8* data);
  static bool ReceiveData(Uart<STM32EL_SerialBase::SerialBase>* port,u8* data, u32 length);
  static bool ReceiveDataDMA(Uart<STM32EL_SerialBase::SerialBase>* port,DMA* dma,u8* data, u32 length);

private:
  static u32 GetMode(Uart<STM32EL_SerialBase::SerialBase>* port);
  static u32 GetWordLength(Uart<STM32EL_SerialBase::SerialBase>* port);
  static u32 GetStopBits(Uart<STM32EL_SerialBase::SerialBase>* port);
  static u32 GetHWControl(Uart<STM32EL_SerialBase::SerialBase>* port);
  static u32 GetParity(Uart<STM32EL_SerialBase::SerialBase>* port);
  static u32 GetOverSampling(Uart<STM32EL_SerialBase::SerialBase>* port);
};


template <class T> class SerialPort {
private:
  Uart<STM32EL_SerialBase::SerialBase> *uart;
  char* txPortName = nullptr;
  char* rxPortName = nullptr;
public:
  Uart<STM32EL_SerialBase::SerialBase> *getUart() const;

public:
  SerialPort(u32 rate,const char* txPort,const char* rxPort);
  void reloadUart();
  void sendData(u8* data,u32 length);
  bool receiveData(u8* data,u32 length);
  void receiveData(DMA* dma,u8* data,u32 length);
  int receiveData(u8* data);
};

template <class T> SerialPort<T>::SerialPort(u32 rate,const char* txPort, const char* rxPort) {
  txPortName = new char[strlen(txPort)];
  rxPortName = new char[strlen(rxPort)];
  strcpy(txPortName,txPort);
  strcpy(rxPortName,rxPort);
  uart = (Uart<STM32EL_SerialBase::SerialBase> *)(new T(rate));
  SerialPortManager::Register(uart,txPort,rxPort);
}
template <class T>
Uart<STM32EL_SerialBase::SerialBase> *SerialPort<T>::getUart() const {
  return uart;
}
template <class T> void SerialPort<T>::sendData(u8 *data, u32 length) {
  SerialPortManager::SendData(uart,data,length);
}
template <class T> int SerialPort<T>::receiveData(u8 *data) {
  return SerialPortManager::ReceiveData(uart,data);
}
template <class T> bool SerialPort<T>::receiveData(u8 *data, u32 length) {
  return SerialPortManager::ReceiveData(uart,data,length);
}
template <class T> void SerialPort<T>::reloadUart() {
  SerialPortManager::Register(uart,txPortName,rxPortName);
}
template <class T>
void SerialPort<T>::receiveData(DMA *dma, u8 *data, u32 length) {
  SerialPortManager::ReceiveDataDMA(uart,dma,data,length);
}

} // namespace STM32EL

#endif // STM32EL_STM32F427IIH6_EL_SERIALPORT_H
