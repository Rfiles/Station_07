#include "Adafruit_Si7021.h"

Adafruit_Si7021     si7021 = Adafruit_Si7021();
uint8_t si7_module_num;

bool si7021_boot ( uint8_t mybootnum ) {
    si7_module_num = mybootnum;
    tcaselect(TCA_SI7);

    if (!si7021.begin()) return false;
    return true;

}

void si7021_read () {
    if (!ModuleData[si7_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_SI7);
    SensorData.si7021Data.temperature = si7021.readTemperature();
    SensorData.si7021Data.humidity = si7021.readHumidity();

}

void si7021_print () {
    printf( "SI7: T[%.2f] H[%.2f] \n", SensorData.si7021Data.temperature, SensorData.si7021Data.humidity );
}


void si7021_cmds () {
  
}




//EOF
