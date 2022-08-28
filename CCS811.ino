#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;

uint8_t ccs_module_num;
uint8_t ccs_status;
uint8_t ccs_error;

bool ccs_boot ( uint8_t mybootnum ) {
    ccs_module_num = mybootnum;
    tcaselect(TCA_CCS);
    if (!ccs.begin()) return false;
    ccs.setEnvironmentalData(SensorData.si7021Data.humidity, SensorData.si7021Data.temperature);
    //ccs.setDriveMode(CCS811_DRIVE_MODE_250MS); aqui nao processa o algoritmo interno
    return true;

}

void ccs_read () {
    ccs_status = 0;
    if (!ModuleData[ccs_module_num].Register.ModuleDetected) return;
    ccs_status = 1;
    tcaselect(TCA_CCS);
    ccs_error = ccs.readData();
    if (ccs_error) return;
    ccs_status = 2;
    ccs.setEnvironmentalData(SensorData.si7021Data.humidity, SensorData.si7021Data.temperature);
    SensorData.CCSData.co2 = ccs.geteCO2();
    SensorData.CCSData.volatiles = ccs.getTVOC();
    
}

void ccs_print () {
    switch (ccs_status) {
      case 0:
        printf( "CCS: Not Detected\n" );
        break;
      case 1:
        printf( "CCS: Read Error: [" BYTE_TO_BINARY_PATTERN "]\n", BYTE_TO_BINARY(ccs_error));
        break;
      case 2:
        printf( "CCS: eCO2[%4u] VOL[%4u] \n", SensorData.CCSData.co2, SensorData.CCSData.volatiles );
        break;
    }
    
}
