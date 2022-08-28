#include <Adafruit_BME280.h>
//#include <Adafruit_BMP085.h>

#define SEALEVELPRESSURE_HPA (1017.25)
Adafruit_BME280     bme; // I2C
//Adafruit_BMP085 bme;
uint8_t bme_module_num;

bool bme_boot ( uint8_t mybootnum ) {
    bme_module_num = mybootnum;
    tcaselect(TCA_BME);
    if (!bme.begin(0x76)) return false;
    return true;
}


void bme_read(){
    if (!ModuleData[bme_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_BME);
    SensorData.BMEData.temperature = bme.readTemperature();
    SensorData.BMEData.pressure = bme.readPressure() / 100.0F;
    SensorData.BMEData.humidity = bme.readHumidity();
}


void bme_print () {
    printf( "%s: T[%.2f] P[%.3f] H[%.2f] \n",
          ModuleData[bme_module_num].Config.Name, 
          SensorData.BMEData.temperature, 
          SensorData.BMEData.pressure, 
          SensorData.BMEData.humidity );
}



void bme_read_calc () {
    //tcaselect(4);
    //Serial.print("<BME_A=");
    //Serial.print(bme.readFloatAltitudeMeters(), 2);
    //Serial.print("><BME_D=");
    //Serial.print(bme.dewPointC(), 2);
    //Serial.println(">");
    
}



//EOF
