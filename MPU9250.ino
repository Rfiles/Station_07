#include <MPU9250_WE.h>
#define MPU9250_ADDR 0x68

MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

uint8_t mpu_module_num;
uint8_t LastInterrupt;
bool    MontionDetected = false;

bool mpu_boot ( uint8_t mybootnum ) {
    mpu_module_num = mybootnum;
    tcaselect(TCA_MPU);

    if (!myMPU9250.init()) return false;
    myMPU9250.initMagnetometer();
    myMPU9250.autoOffsets();
    myMPU9250.enableGyrDLPF();
    myMPU9250.setGyrDLPF(MPU9250_DLPF_6);
    myMPU9250.setSampleRateDivider(5);
    myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);
    myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);
    myMPU9250.enableAccDLPF(true);
    myMPU9250.setAccDLPF(MPU9250_DLPF_6);
    myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);

    myMPU9250.enableIntLatch(true);
    myMPU9250.enableClearIntByAnyRead(false);
    myMPU9250.enableInterrupt(MPU9250_WOM_INT); 
    myMPU9250.setWakeOnMotionThreshold(128);  // 128 = ~0.5 g
    myMPU9250.enableWakeOnMotion(MPU9250_WOM_ENABLE, MPU9250_WOM_COMP_DISABLE);
        
    return true;

}

void mpu_read () {
    if (!ModuleData[mpu_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_MPU);
    xyzFloat gValue = myMPU9250.getGValues();
    xyzFloat gyr = myMPU9250.getGyrValues();
    xyzFloat magValue = myMPU9250.getMagValues();
    float temp = myMPU9250.getTemperature();
    float resultantG = myMPU9250.getResultantG(gValue);

    SensorData.mpuData.Ax   = gValue.x;
    SensorData.mpuData.Ay   = gValue.y;
    SensorData.mpuData.Az   = gValue.z;
    SensorData.mpuData.Gx   = gyr.x;
    SensorData.mpuData.Gy   = gyr.y;
    SensorData.mpuData.Gz   = gyr.z;
    SensorData.mpuData.Mx   = magValue.x;
    SensorData.mpuData.My   = magValue.y;
    SensorData.mpuData.Mz   = magValue.z;
    SensorData.mpuData.Temperature = temp;
    SensorData.mpuData.TotalG = resultantG;

    LastInterrupt = myMPU9250.readAndClearInterrupts();
    if(myMPU9250.checkInterrupt(LastInterrupt, MPU9250_WOM_INT)) {
      MontionDetected = true;
    }

    xyzFloat angles = myMPU9250.getAngles();
    float pitch = myMPU9250.getPitch();
    float roll  = myMPU9250.getRoll();

    SensorData.mpuData.AngleX   = angles.x;
    SensorData.mpuData.AngleY   = angles.y;
    SensorData.mpuData.AngleZ   = angles.z;
    SensorData.mpuData.Pitch    = pitch;
    SensorData.mpuData.Roll     = roll;

    
}

void mpu_print () {
    printf( "MPU: Ax[%0.2f] Ay[%0.2f] Az[%0.2f] - Gx[%0.2f] Gy[%0.2f] Gz[%0.2f]\n", 
            SensorData.mpuData.Ax,
            SensorData.mpuData.Ay,
            SensorData.mpuData.Az,
            SensorData.mpuData.Gx,
            SensorData.mpuData.Gy,
            SensorData.mpuData.Gz );
    printf( "MPU: Mx[%0.2f] My[%0.2f] Mz[%0.2f] SumG[%0.2f] T[%0.2f'C]\n", 
            SensorData.mpuData.Mx,
            SensorData.mpuData.My,
            SensorData.mpuData.Mz,
            SensorData.mpuData.TotalG,
            SensorData.mpuData.Temperature );
    printf( "MPU: ANx[%0.2f] ANy[%0.2f] ANz[%0.2f] Pitch[%0.2f] Roll[%0.2f] M[%u]\n", 
            SensorData.mpuData.AngleX,
            SensorData.mpuData.AngleY,
            SensorData.mpuData.AngleZ,
            SensorData.mpuData.Pitch,
            SensorData.mpuData.Roll,
            MontionDetected );
            

    MontionDetected = false;
}
