#include <avm_core.h>

static unsigned char avm_mpu_init(void *);

avm_module_t avm_mpu_module_st = {
    0,
    NULL,
    avm_mpu_init,
    NULL,
    NULL
};

static unsigned char avm_mpu_init(void *arg) {
    // Delay is required after MPU6050 powered up, At least 7ms
    delay_ms(7);
    MPU_init();
    return 0;
}


void MPU_Sigle_Write(unsigned char reg_addr, unsigned char reg_data) {
    IIC_Start();
    IIC_Send(MPU6050_ADDR);
    if(!IIC_Wait_Ack())
		LED1 = 1;

    IIC_Send(reg_addr);
    IIC_Wait_Ack();

    IIC_Send(reg_data);
    IIC_Wait_Ack();

    IIC_Stop();
}

unsigned char MPU_Sigle_Read(unsigned reg_addr) {
    unsigned char reg_data;
    IIC_Start();
    IIC_Send(MPU6050_ADDR);
    IIC_Wait_Ack();

    IIC_Send(reg_addr);
    IIC_Wait_Ack();

    IIC_Start();
    IIC_Send(MPU6050_ADDR+1);
    IIC_Wait_Ack();

    reg_data = IIC_Read();
    IIC_Ack(1);
    IIC_Stop();
    return reg_data;
}

short MPU_GetData(unsigned char REG_Addr) {
    unsigned char H, L;
    H = MPU_Sigle_Read(REG_Addr);
    L = MPU_Sigle_Read(REG_Addr+1);
    return (short)(H<<8)+L;
}

void MPU_init() {
    IIC_init();
    do {
        MPU_Sigle_Write(PWR_MGMT_1, 0x00);
        MPU_Sigle_Write(SMPLRT_DIV, 0x07);
        MPU_Sigle_Write(CONFIG, 0x06);
        MPU_Sigle_Write(GYRO_CONFIG, 0x18);
        MPU_Sigle_Write(ACCEL_CONFIG, 0x01);
    } while(IMU_NOT_CONNECTED);
}

void MPU6050_getStructData(pSixAxis cache) {
    cache->gX = (float)MPU_GetData(GYRO_XOUT_H)/16.4f;
    cache->gY = (float)MPU_GetData(GYRO_YOUT_H)/16.4f;
    cache->gZ = (float)MPU_GetData(GYRO_ZOUT_H)/16.4f;

    cache->aX = (float)MPU_GetData(ACCEL_XOUT_H)/1671.83f;
    cache->aY = (float)MPU_GetData(ACCEL_YOUT_H)/1671.83f;
    cache->aZ = (float)MPU_GetData(ACCEL_ZOUT_H)/1671.83f;

#ifdef IMU_SOFTWARE_FIXED
    cache->gX += G_X_OFFSET;
    cache->gY += G_Y_OFFSET;
    cache->gZ += G_Z_OFFSET;

    cache->aX += A_X_OFFSET;
    cache->aY += A_Y_OFFSET;
    cache->aZ += A_Z_OFFSET;
#endif
}
// void MPU6050_debug(pSixAxis cache) {
//     uart_Float2Char((float)cache->gX);
//     uart_sendData(' ');
//     uart_Float2Char((float)cache->gY);
//     uart_sendData(' ');
//     uart_Float2Char((float)cache->gZ);
//     uart_sendData(' ');
//
//     uart_Float2Char((float)cache->aX);
//     uart_sendData(' ');
//     uart_Float2Char((float)cache->aY);
//     uart_sendData(' ');
//     uart_Float2Char((float)cache->aZ);
//     uart_sendData(' ');
//     uart_sendData(0x0D);
//     uart_sendData(0x0A);
// }

float g_Yaw, g_Pitch, g_Roll;
SixAxis avm_euler;

void IMU_Comput(SixAxis cache) {
	static float g_q0 = 1, g_q1 = 0, g_q2 = 0, g_q3 = 0;   //Quaternion
	static float g_exInt = 0, g_eyInt = 0, g_ezInt = 0;


    float norm;     //模
    float vx, vy, vz;
    float ex, ey, ez;

    norm = sqrt(cache.aX*cache.aX + cache.aY*cache.aY + cache.aZ*cache.aZ);     //取模

    //向量化
    cache.aX = cache.aX / norm;
    cache.aY = cache.aY / norm;
    cache.aZ = cache.aZ / norm;

    //估计方向的重力
    vx = 2 * (g_q1 * g_q3 - g_q0 * g_q2);
    vy = 2 * (g_q0 * g_q1 + g_q2 * g_q3);
    vz = g_q0*g_q0 - g_q1*g_q1 - g_q2*g_q2 + g_q3*g_q3;

    //错误的领域和方向传感器测量参考方向几件的交叉乘积的总和
    ex = (cache.aY * vz - cache.aZ * vy);
    ey = (cache.aZ * vx - cache.aX * vz);
    ez = (cache.aX * vy - cache.aY * vx);

    //积分误差比例积分增益
    g_exInt += ex * Ki;
    g_eyInt += ey * Ki;
    g_ezInt += ez * Ki;

    //调整后的陀螺仪测量
    cache.gX += Kp * ex + g_exInt;
    cache.gY += Kp * ey + g_eyInt;
    cache.gZ += Kp * ez + g_ezInt;

    //整合四元数率和正常化
    g_q0 += (-g_q1 * cache.gX - g_q2 * cache.gY - g_q3 * cache.gZ) * halfT;
    g_q1 += (g_q0 * cache.gX + g_q2 * cache.gZ - g_q3 * cache.gY) * halfT;
    g_q2 += (g_q0 * cache.gY - g_q1 * cache.gZ + g_q3 * cache.gX) * halfT;
    g_q3 += (g_q0 * cache.gZ + g_q1 * cache.gY - g_q2 * cache.gX) * halfT;

    //正常化四元
    norm = sqrt(g_q0*g_q0 + g_q1*g_q1 + g_q2*g_q2 + g_q3*g_q3);
    g_q0 = g_q0 / norm;
    g_q1 = g_q1 / norm;
    g_q2 = g_q2 / norm;
    g_q3 = g_q3 / norm;

    g_Pitch = asin(-2 * g_q1 * g_q3 + 2 * g_q0 * g_q2) * 57.3;
    g_Roll = atan2(2 * g_q2 * g_q3 + 2 * g_q0 * g_q1, -2 * g_q1*g_q1 - 2 * g_q2*g_q2 + 1) * 57.3;
    g_Yaw = atan2(2 * (g_q1 * g_q2 + g_q0 * g_q3), g_q0*g_q0 + g_q1*g_q1 - g_q2*g_q2 - g_q3*g_q3) * 57.3;
}
