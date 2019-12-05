//
// Created by jelin on 19-8-4.
//

#ifndef STM32_EL_DEMO2_MPU6500_H
#define STM32_EL_DEMO2_MPU6500_H

#include "stm32el.h"


#define MPU6500_FCHOICE_B_0 0
#define MPU6500_FCHOICE_B_1 1
#define MPU6500_FCHOICE_B_2 2

#define MPU6500_DLPF_CFG_0 0
#define MPU6500_DLPF_CFG_1 1
#define MPU6500_DLPF_CFG_2 2
#define MPU6500_DLPF_CFG_3 3
#define MPU6500_DLPF_CFG_4 4
#define MPU6500_DLPF_CFG_5 5
#define MPU6500_DLPF_CFG_6 6
#define MPU6500_DLPF_CFG_7 7

#define MPU6500_GYRO_FS_SEL_250 0
#define MPU6500_GYRO_FS_SEL_500 1
#define MPU6500_GYRO_FS_SEL_1000 2
#define MPU6500_GYRO_FS_SEL_2000 3

#define MPU6500_ACCEL_FS_SEL_2 0
#define MPU6500_ACCEL_FS_SEL_4 1
#define MPU6500_ACCEL_FS_SEL_8 2
#define MPU6500_ACCEL_FS_SEL_16 3

#define MPU6500_ACCEL_FCHOICE_B_0 0
#define MPU6500_ACCEL_FCHOICE_B_1 1

#define BOARD_DOWN
#define MPU6500_Kp     2.0
#define MPU6500_Ki     0.01
class MPU6500 {
private:
  typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    int16_t temp;

    int16_t gx;
    int16_t gy;
    int16_t gz;

    int16_t ax_offset;
    int16_t ay_offset;
    int16_t az_offset;

    int16_t gx_offset;
    int16_t gy_offset;
    int16_t gz_offset;
  } mpu_data_t;

  typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    float temp;

    float wx; /*!< omiga, +- 2000dps => +-32768  so gx/16.384/57.3 =	rad/s */
    float wy;
    float wz;

    float vx;
    float vy;
    float vz;

    float rol;
    float pit;
    float yaw;
  } imu_t;
  float YAW = 0;

  IOPort CS = Gpio::Register("PF6");
  SPI<Spi5> spi = SPI<Spi5>("PF7","PF8","PF9");
  bool Online;
  volatile float q0 = 1.0f;
  volatile float q1 = 0.0f;
  volatile float q2 = 0.0f;
  volatile float q3 = 0.0f;
  volatile float exInt = 0, eyInt = 0, ezInt = 0; /* error integral */
  volatile float gx = 0, gy = 0, gz = 0, ax = 0, ay = 0, az = 0, mx = 0, my = 0, mz = 0;
  uint32_t last_update = 0, now_update = 0; /* Sampling cycle count, ubit ms */
  uint8_t tx_buff[14] = {0xff};
  uint8_t mpu_buff[14] = {0}; /* buffer to save imu raw data */
  mpu_data_t mpu_data = {0};
  imu_t imu = {0};
public:
  bool isOnline() const;

protected:
  void GyroscopeConfig(u8 FCHOICE_B, u8 DLPF_CFG, u8 GYRO_FS_SEL);
  void AccelerConfig(u8 ACCEL_FCHOICE_B, u8 A_DLPF_CFG, u8 ACCEL_FS_SEL);
  void Reset();
  void SetGyroScale(u8 GYRO_FS_SEL);
  void SetAccelcale(u8 ACCEL_FS_SEL);

private:
  u8 read(u16 addr);
  u8 readBytes(u16 addr, u8 *pData, u8 len);
  void write(u16 addr, u8 data);
  void InitQuaternion();
  void OffsetCall();
  void GetData();
  void AhrsUpdate();
  void AttitUpdate();

public:
  MPU6500();
  bool Check();
  void Update();
  float GetRollAngle();
  float GetPitchAngle();
  float GetYawAngle();
  float GetXAccel();
  float GetYAccel();
  float GetZAccel();
  float GetTemperature();
};

#endif // STM32_EL_DEMO2_MPU6500_H
