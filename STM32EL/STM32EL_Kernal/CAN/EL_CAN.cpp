//
// Created by skjsn on 2019/9/25.
//

#include "EL_CAN.h"
#include "stm32f4xx_hal.h"
#include "vector"
#include <EL_RCC.h>
#include <algorithm>
using namespace std;
using namespace STM32EL;

class AutoBaudPram{
private:
public:
  u8 getBs1() const { return BS1; }
  void setBs1(u8 bs1) { BS1 = bs1; }
  u8 getBs2() const { return BS2; }
  void setBs2(u8 bs2) { BS2 = bs2; }
  u8 getPsc() const { return PSC; }
  void setPsc(u8 psc) { PSC = psc; }
  float getCia() const { return CIA; }
  void setCia(float cia) { CIA = cia; }
private:
  u8 BS1 = 0;
  u8 BS2 = 0;
  u8 PSC = 0;
  float CIA = 0;

public:
  AutoBaudPram(u8 bs1,u8 bs2,u8 psc, float cia){
    setBs1(bs1);
    setBs2(bs2);
    setPsc(psc);
    setCia(cia);
  }
};

bool comp(const AutoBaudPram& a,const AutoBaudPram& b){
  return a.getCia()>b.getCia();
}

u32 STM32EL_CAN::CanBase::getBase() const { return base; }
STM32EL::AfioType STM32EL_CAN::CanBase::getAfioType() const { return afioType; }
STM32EL_CAN::Can1::Can1() {
  base = CAN1_BASE;
  afioType = STM32EL::AfioType::can1;
}
STM32EL_CAN::Can2::Can2() {
  base = CAN2_BASE;
  afioType = STM32EL::AfioType::can2;
}
void STM32EL_CAN::Can::Init(u32 base, void **handler,u8 BS1,u8 BS2,u8 psc) {
  u32 bs1 = 0 ,bs2 = 0;
  auto canHandleTypeDef = new CAN_HandleTypeDef;
  canHandleTypeDef->Instance = (CAN_TypeDef*)base;
  canHandleTypeDef->Init.Prescaler = psc;
  canHandleTypeDef->Init.Mode = CAN_MODE_NORMAL;
  canHandleTypeDef->Init.SyncJumpWidth = CAN_SJW_1TQ;
  switch (BS1){
  case 1:
    bs1 = CAN_BS1_1TQ;
    break;
  case 2:
    bs1 = CAN_BS1_2TQ;
    break;
  case 3:
    bs1 = CAN_BS1_3TQ;
    break;
  case 4:
    bs1 = CAN_BS1_4TQ;
    break;
  case 5:
    bs1 = CAN_BS1_5TQ;
    break;
  case 6:
    bs1 = CAN_BS1_6TQ;
    break;
  case 7:
    bs1 = CAN_BS1_7TQ;
    break;
  case 8:
    bs1 = CAN_BS1_8TQ;
    break;
  case 9:
    bs1 = CAN_BS1_9TQ;
    break;
  case 10:
    bs1 = CAN_BS1_10TQ;
    break;
  case 11:
    bs1 = CAN_BS1_11TQ;
    break;
  case 12:
    bs1 = CAN_BS1_12TQ;
    break;
  case 13:
    bs1 = CAN_BS1_13TQ;
    break;
  case 14:
    bs1 = CAN_BS1_14TQ;
    break;
  case 15:
    bs1 = CAN_BS1_15TQ;
    break;
  case 16:
    bs1 = CAN_BS1_16TQ;
    break;
  default:
    bs1 = 0;
  }

  switch (BS2){
  case 1:
    bs2 = CAN_BS2_1TQ;
    break;
  case 2:
    bs2 = CAN_BS2_2TQ;
    break;
  case 3:
    bs2 = CAN_BS2_3TQ;
    break;
  case 4:
    bs2 = CAN_BS2_4TQ;
    break;
  case 5:
    bs2 = CAN_BS2_5TQ;
    break;
  case 6:
    bs2 = CAN_BS2_6TQ;
    break;
  case 7:
    bs2 = CAN_BS2_7TQ;
    break;
  case 8:
    bs2 = CAN_BS2_8TQ;
    break;
  default:
    bs2 = 0;
  }
  canHandleTypeDef->Init.TimeSeg1 = bs1;
  canHandleTypeDef->Init.TimeSeg2 = bs2;
  canHandleTypeDef->Init.TimeTriggeredMode = DISABLE;
  canHandleTypeDef->Init.AutoBusOff = ENABLE;
  canHandleTypeDef->Init.AutoWakeUp = DISABLE;
  canHandleTypeDef->Init.AutoRetransmission = ENABLE;
  canHandleTypeDef->Init.ReceiveFifoLocked = DISABLE;
  canHandleTypeDef->Init.TransmitFifoPriority = DISABLE;
  if(HAL_CAN_Init(canHandleTypeDef) != HAL_OK){
    Error_Handler();
  }
  CAN_FilterTypeDef filter;
  filter.FilterIdHigh = 0;
  filter.FilterIdLow = 0;
  filter.FilterMaskIdHigh = 0;
  filter.FilterMaskIdLow = 0;
  if (canHandleTypeDef->Instance == CAN1) {
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank = 0;

  } else {
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filter.FilterBank = 16;
  }
  filter.SlaveStartFilterBank = 13;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterActivation = ENABLE;

  if (HAL_CAN_ConfigFilter(canHandleTypeDef, &filter) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_CAN_Start(canHandleTypeDef) != HAL_OK) {
    Error_Handler();
  }

  *handler = canHandleTypeDef;
}


void STM32EL_CAN::Can::AutoBaud(u32 baud, u8 *psc, u8 *bs1, u8 *bs2) {
  u32 Fpclk1 = Rcc::GetAPB1Clock();
  u8 BS1 = 0;
  u8 BS2 = 0;
  u8 PSC = 0;
  float CIA;
  vector<AutoBaudPram> prams;
  for(int i = 1 ;i<=16;i++){
    BS1 = i;
    for(int j = 1 ;j<=8;j++){
      BS2 = j;
      for(int z = 1;z<=10;z++){
        PSC = z;
        u32 res = Fpclk1/(1+BS1+BS2)/PSC;
        if(res == baud){
          CIA = ((float)(1+BS1)/(float)(1+BS1+BS2));
          if(baud>800000){
            if(CIA >0.75){
              prams.emplace_back(BS1,BS2,PSC,CIA);
            }
          }else if(baud>500000){
            if(CIA >0.8){
              prams.emplace_back(BS1,BS2,PSC,CIA);
            }
          }else{
            if(CIA >0.875){
              prams.emplace_back(BS1,BS2,PSC,CIA);
            }
          }
        }
      }
    }
  }
  sort(prams.begin(),prams.end(),comp);
  *psc = prams[0].getPsc();
  *bs1 = prams[0].getBs1();
  *bs2 = prams[0].getBs2();
}
void STM32EL_CAN::Can::SetNormalMode(void *handler) {
  ((CAN_HandleTypeDef*)handler)->Init.Mode = CAN_MODE_NORMAL;
  HAL_CAN_Init(((CAN_HandleTypeDef*)handler));
}
void STM32EL_CAN::Can::SetLoopBackMode(void *handler) {
  ((CAN_HandleTypeDef*)handler)->Init.Mode = CAN_MODE_LOOPBACK;
  HAL_CAN_Init(((CAN_HandleTypeDef*)handler));
}
void STM32EL_CAN::Can::SendData(void *handler,u16 id, u8 *data) {
  CAN_TxHeaderTypeDef txHandel;
  txHandel.StdId = id;
  txHandel.DLC = 8;
  txHandel.IDE = CAN_ID_STD;
  txHandel.RTR = CAN_RTR_DATA;
  txHandel.TransmitGlobalTime = ENABLE;
  if (HAL_CAN_GetTxMailboxesFreeLevel(((CAN_HandleTypeDef*)handler)) > 0) {
    if (HAL_CAN_AddTxMessage(((CAN_HandleTypeDef*)handler), &txHandel, data,
                             (uint32_t *)GetFreeTxMail(handler)) != HAL_OK) {
      NOP();
    }
  }
}
u32 STM32EL_CAN::Can::GetFreeTxMail(void *handler) {
  if(HAL_CAN_GetTxMailboxesFreeLevel((CAN_HandleTypeDef*)handler) > 0)
  {
    if ((((CAN_HandleTypeDef*)handler)->Instance->TSR & CAN_TSR_TME0) != 0U)
    {
      return CAN_TX_MAILBOX0;
    }
    if ((((CAN_HandleTypeDef*)handler)->Instance->TSR & CAN_TSR_TME1) != 0U)
    {
      return CAN_TX_MAILBOX1;
    }
    if ((((CAN_HandleTypeDef*)handler)->Instance->TSR & CAN_TSR_TME2) != 0U)
    {
      return CAN_TX_MAILBOX2;
    }
  }
  return 0;
}

typedef struct
{
  CAN_RxHeaderTypeDef rxMessage = {0};
  uint8_t data[8]{};
  uint32_t tim{};
}CAN_RxMessage;

u16 STM32EL_CAN::Can::ReceiveData(void *handler, u8 *buf) {
  auto RxHandle = new CAN_RxMessage;

  if(((CAN_HandleTypeDef*)handler)->Instance == CAN1){
    HAL_CAN_GetRxMessage(((CAN_HandleTypeDef*)handler), CAN_RX_FIFO0, &RxHandle->rxMessage,
                         RxHandle->data);
  } else{
    HAL_CAN_GetRxMessage(((CAN_HandleTypeDef*)handler), CAN_RX_FIFO1, &RxHandle->rxMessage,
                         RxHandle->data);
  }
  if (RxHandle->rxMessage.DLC == 8) {
    for (int i = 0; i < 8; i++) {
      buf[i] = RxHandle->data[i];
    }
  }
  u16 id = RxHandle->rxMessage.StdId;
  delete(RxHandle);
  return id;
}
extern "C" void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
    __HAL_RCC_CAN1_CLK_ENABLE();
  }
  if(canHandle->Instance==CAN2)
  {
    __HAL_RCC_CAN2_CLK_ENABLE();
  }

}