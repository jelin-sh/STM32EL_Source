//
// Created by jelin on 19-8-4.
//

#include "MPU6500.h"
#include "MPU6500_Register.h"
#include <cmath>
#include <cstring>

static float inv_sqrt(float x) {
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long *)&y;

  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

MPU6500::MPU6500() {
  spi.setCPOL(false);
  spi.setCPHA(false);
  CS = 1;
  Online = Check();
  uint8_t i                        = 0;
  uint8_t MPU6500_Init_Data[10][2] = {{ MPU6500_PWR_MGMT_1, 0x80 },     /* Reset Device */
                                      { MPU6500_PWR_MGMT_1, 0x03 },     /* Clock Source - Gyro-Z */
                                      { MPU6500_PWR_MGMT_2, 0x00 },     /* Enable Acc & Gyro */
                                      { MPU6500_CONFIG, 0x00 },         /* LPF 3600Hz */
                                      { MPU6500_GYRO_CONFIG, 0x00 },    /* +-250dps*/
                                      { MPU6500_ACCEL_CONFIG, 0x10 },   /* +-8G */
                                      { MPU6500_ACCEL_CONFIG_2, 0x07 }, /* enable LowPassFilter  Set Acc LPF */
                                      { MPU6500_USER_CTRL, 0x20 },};    /* Enable AUX */
  for (i = 0; i < 8; i++)
  {
    write(MPU6500_Init_Data[i][0], MPU6500_Init_Data[i][1]);
    Delay::ms(1);
  }
  SetGyroScale(MPU6500_GYRO_FS_SEL_2000);
  SetAccelcale(MPU6500_ACCEL_FS_SEL_8);
  OffsetCall();
  InitQuaternion();
}
void MPU6500::Reset() {
  write(MPU6500_PWR_MGMT_1, 0x80); // reset
  write(MPU6500_PWR_MGMT_1, 0x03); // clock
  write(MPU6500_PWR_MGMT_2, 0x00);
}
bool MPU6500::Check() {
  u8 a = read(MPU6500_WHO_AM_I);
  return a == 0x70;
}
u8 MPU6500::read(u16 addr) {
  u8 res = 0;
  CS = 0;
  spi.readWriteByte(0x80 | addr);
  res = spi.readWriteByte(0x80 | addr);
  CS = 1;
  return res;
}
bool MPU6500::isOnline() const { return Online; }
void MPU6500::write(u16 addr, u8 data) {
  CS = 0;
  spi.readWriteByte(addr & 0x7f);
  spi.readWriteByte(data);
  CS = 1;
}
void MPU6500::GyroscopeConfig(u8 FCHOICE_B, u8 DLPF_CFG, u8 GYRO_FS_SEL) {
  u8 config = read(MPU6500_CONFIG);
  u8 gyConfig = read(MPU6500_GYRO_CONFIG);
  config &= 0xF8;
  gyConfig &= 0xE0;
  config |= DLPF_CFG;
  gyConfig |= GYRO_FS_SEL << 3;
  gyConfig |= FCHOICE_B;
  write(MPU6500_CONFIG, config);
  write(MPU6500_GYRO_CONFIG, gyConfig);
}
void MPU6500::AccelerConfig(u8 ACCEL_FCHOICE_B, u8 A_DLPF_CFG,
                            u8 ACCEL_FS_SEL) {
  u8 acclerConfig1 = read(MPU6500_ACCEL_CONFIG);
  u8 acclerConfig2 = read(MPU6500_ACCEL_CONFIG_2);
  acclerConfig1 &= 0xE0;
  acclerConfig1 |= ACCEL_FS_SEL << 3;

  acclerConfig2 = 0x00;
  acclerConfig2 |= ACCEL_FCHOICE_B << 3;
  acclerConfig2 |= A_DLPF_CFG;

  write(MPU6500_ACCEL_CONFIG, acclerConfig1);
  write(MPU6500_ACCEL_CONFIG_2, acclerConfig2);
}
void MPU6500::SetGyroScale(u8 GYRO_FS_SEL) {
  u8 gyConfig = read(MPU6500_GYRO_CONFIG);
  gyConfig &= 0xE0;
  gyConfig |= GYRO_FS_SEL << 3;
  write(MPU6500_GYRO_CONFIG, gyConfig);
}
void MPU6500::SetAccelcale(u8 ACCEL_FS_SEL) {
  u8 acclerConfig1 = read(MPU6500_ACCEL_CONFIG);
  acclerConfig1 &= 0xE0;
  acclerConfig1 |= ACCEL_FS_SEL << 3;
  write(MPU6500_ACCEL_CONFIG, acclerConfig1);
}
void MPU6500::InitQuaternion() {
  int16_t hx, hy; // hz;

  hx = imu.mx;
  hy = imu.my;
  // hz = imu.mz;

#ifdef BOARD_DOWN
  if (hx < 0 && hy < 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = (volatile float)-0.005;
      q1 = -0.199;
      q2 = 0.979;
      q3 = -0.0089;
    } else {
      q0 = -0.008;
      q1 = -0.555;
      q2 = 0.83;
      q3 = -0.002;
    }

  } else if (hx < 0 && hy > 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = 0.005;
      q1 = -0.199;
      q2 = -0.978;
      q3 = 0.012;
    } else {
      q0 = 0.005;
      q1 = -0.553;
      q2 = -0.83;
      q3 = -0.0023;
    }

  } else if (hx > 0 && hy > 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = 0.0012;
      q1 = -0.978;
      q2 = -0.199;
      q3 = -0.005;
    } else {
      q0 = 0.0023;
      q1 = -0.83;
      q2 = -0.553;
      q3 = 0.0023;
    }

  } else if (hx > 0 && hy < 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = 0.0025;
      q1 = 0.978;
      q2 = -0.199;
      q3 = 0.008;
    } else {
      q0 = 0.0025;
      q1 = 0.83;
      q2 = -0.56;
      q3 = 0.0045;
    }
  }
#else
  if (hx < 0 && hy < 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = 0.195;
      q1 = -0.015;
      q2 = 0.0043;
      q3 = 0.979;
    } else {
      q0 = 0.555;
      q1 = -0.015;
      q2 = 0.006;
      q3 = 0.829;
    }

  } else if (hx < 0 && hy > 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = -0.193;
      q1 = -0.009;
      q2 = -0.006;
      q3 = 0.979;
    } else {
      q0 = -0.552;
      q1 = -0.0048;
      q2 = -0.0115;
      q3 = 0.8313;
    }

  } else if (hx > 0 && hy > 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = -0.9785;
      q1 = 0.008;
      q2 = -0.02;
      q3 = 0.195;
    } else {
      q0 = -0.9828;
      q1 = 0.002;
      q2 = -0.0167;
      q3 = 0.5557;
    }

  } else if (hx > 0 && hy < 0) {
    if (fabs(hx / hy) >= 1) {
      q0 = -0.979;
      q1 = 0.0116;
      q2 = -0.0167;
      q3 = -0.195;
    } else {
      q0 = -0.83;
      q1 = 0.014;
      q2 = -0.012;
      q3 = -0.556;
    }
  }
#endif
}
void MPU6500::OffsetCall() {
  int i;
  int dz = 0;
  mpu_data_t tem = {0};
  for (i = 0; i < 300; i++) {
    readBytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);
    mpu_data.ax_offset = mpu_buff[0] << 8 | mpu_buff[1];
    mpu_data.ay_offset = mpu_buff[2] << 8 | mpu_buff[3];
    mpu_data.az_offset = mpu_buff[4] << 8 | mpu_buff[5];

    mpu_data.gx_offset = mpu_buff[8] << 8 | mpu_buff[9];
    mpu_data.gy_offset = mpu_buff[10] << 8 | mpu_buff[11];
    mpu_data.gz_offset = mpu_buff[12] << 8 | mpu_buff[13];
    Delay::ms(5);
  }

  write(MPU6500_ZG_OFFSET_L,0);
  write(MPU6500_ZG_OFFSET_H,0);
}
u8 MPU6500::readBytes(u16 addr, u8 *pData, u8 len) {
  CS = 0;
  u8 tx = 0x80 | addr;
  spi.readWriteByte(tx);
  spi.readWriteBytes(&tx, pData, len);
  CS = 1;
  return 0;
}
void MPU6500::GetData() {
  readBytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);

  mpu_data.ax = mpu_buff[0] << 8 | mpu_buff[1];
  mpu_data.ay = mpu_buff[2] << 8 | mpu_buff[3];
  mpu_data.az = mpu_buff[4] << 8 | mpu_buff[5];
  mpu_data.temp = mpu_buff[6] << 8 | mpu_buff[7];

  mpu_data.gx = ((mpu_buff[8] << 8 | mpu_buff[9]) - mpu_data.gx_offset);
  mpu_data.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - mpu_data.gy_offset);
  mpu_data.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - mpu_data.gz_offset);
  //out<<"MPU6500_ax:"<<mpu_data.ax<<"\r\n";
  mpu_data.mx = 0;
  memcpy(&imu.ax, &mpu_data.ax, 6 * sizeof(int16_t));

  imu.temp = 21 + mpu_data.temp / 333.87f;
  /* 2000dps -> rad/s */
  imu.wx = mpu_data.gx / 16.384f / 57.3f;
  imu.wy = mpu_data.gy / 16.384f / 57.3f;
  imu.wz = mpu_data.gz / 16.384f / 57.3f;
}
void MPU6500::AhrsUpdate() {
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez, halfT;
  float tempq0, tempq1, tempq2, tempq3;

  float q0q0 = q0 * q0;
  float q0q1 = q0 * q1;
  float q0q2 = q0 * q2;
  float q0q3 = q0 * q3;
  float q1q1 = q1 * q1;
  float q1q2 = q1 * q2;
  float q1q3 = q1 * q3;
  float q2q2 = q2 * q2;
  float q2q3 = q2 * q3;
  float q3q3 = q3 * q3;

  gx = imu.wx;
  gy = imu.wy;
  gz = imu.wz;
  ax = imu.ax;
  ay = imu.ay;
  az = imu.az;
  mx = imu.mx;
  my = imu.my;
  mz = imu.mz;

  now_update = Rcc::GetTick(); // ms
  halfT = ((float)(now_update - last_update) / 2000.0f);
  last_update = now_update;

  /* Fast inverse square-root */
  norm = inv_sqrt(ax * ax + ay * ay + az * az);
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;

#ifdef IST8310
  norm = inv_sqrt(mx * mx + my * my + mz * mz);
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;
#else
  mx = 0;
  my = 0;
  mz = 0;
#endif
  /* compute reference direction of flux */
  hx = 2.0f * mx * (0.5f - q2q2 - q3q3) + 2.0f * my * (q1q2 - q0q3) +
       2.0f * mz * (q1q3 + q0q2);
  hy = 2.0f * mx * (q1q2 + q0q3) + 2.0f * my * (0.5f - q1q1 - q3q3) +
       2.0f * mz * (q2q3 - q0q1);
  hz = 2.0f * mx * (q1q3 - q0q2) + 2.0f * my * (q2q3 + q0q1) +
       2.0f * mz * (0.5f - q1q1 - q2q2);
  bx = sqrt((hx * hx) + (hy * hy));
  bz = hz;

  /* estimated direction of gravity and flux (v and w) */
  vx = 2.0f * (q1q3 - q0q2);
  vy = 2.0f * (q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
  wx = 2.0f * bx * (0.5f - q2q2 - q3q3) + 2.0f * bz * (q1q3 - q0q2);
  wy = 2.0f * bx * (q1q2 - q0q3) + 2.0f * bz * (q0q1 + q2q3);
  wz = 2.0f * bx * (q0q2 + q1q3) + 2.0f * bz * (0.5f - q1q1 - q2q2);

  /*
   * error is sum of cross product between reference direction
   * of fields and direction measured by sensors
   */
  ex = (ay * vz - az * vy) + (my * wz - mz * wy);
  ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
  ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

  /* PI */
  if (ex != 0.0f && ey != 0.0f && ez != 0.0f) {
    exInt = exInt + ex * 0.01 * halfT;
    eyInt = eyInt + ey * 0.01 * halfT;
    ezInt = ezInt + ez * 0.01 * halfT;

    gx = gx + 2 * ex + exInt;
    gy = gy + 2 * ey + eyInt;
    gz = gz + 2 * ez + ezInt;
  }

  tempq0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
  tempq1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
  tempq2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
  tempq3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

  /* normalise quaternion */
  norm = inv_sqrt(tempq0 * tempq0 + tempq1 * tempq1 + tempq2 * tempq2 +
                  tempq3 * tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;
}
void MPU6500::AttitUpdate() {
  /* yaw    -pi----pi */
  imu.yaw = (float) (-atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3);
  /* pitch  -pi/2----pi/2 */
  imu.pit = (float) (-asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3);
  /* roll   -pi----pi  */
  imu.rol = (float) (atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3);

}
void MPU6500::Update() {
  GetData();
  AhrsUpdate();
  AttitUpdate();
}
float MPU6500::GetRollAngle() { return imu.rol; }
float MPU6500::GetPitchAngle() { return imu.pit; }
float MPU6500::GetYawAngle() { return imu.yaw; }
float MPU6500::GetXAccel() { return imu.wx; }
float MPU6500::GetYAccel() { return imu.wy; }
float MPU6500::GetZAccel() { return imu.wz; }
float MPU6500::GetTemperature() {
  float res;
  u16 temp = 0;
  temp = read(MPU6500_TEMP_OUT_H);
  temp <<= 8;
  temp += read(MPU6500_TEMP_OUT_L);
  res = 21.0f+((float)temp)/333.87;
  NOP();
  return res;
}
