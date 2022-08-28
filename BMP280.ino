#include <Adafruit_BMP280.h>

Adafruit_BMP280     bmp;
uint8_t bmp_module_num;

bool bmp_boot ( uint8_t mybootnum ) {
    bmp_module_num = mybootnum;
    tcaselect(TCA_BMP);
    if (!bmp.begin(BMP280_ADDRESS_ALT)) return false;
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    return true;
}


void bmp_read(){
    if (!ModuleData[bmp_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_BMP);
    SensorData.BMPData.Temperature = bmp.readTemperature();
    SensorData.BMPData.Pressure = bmp.readPressure() / 100.0F;
}


void bmp_print () {

    printf( "BMP: T[%.2f] P[%.3f]\n", 
            SensorData.BMPData.Temperature, 
            SensorData.BMPData.Pressure );
}