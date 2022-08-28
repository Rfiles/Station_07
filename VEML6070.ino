#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv0 = Adafruit_VEML6070();
uint8_t ve0_module_num;

bool veml6070_boot ( uint8_t mybootnum ) {
    ve0_module_num = mybootnum;
    tcaselect(TCA_VE0);
    if (!checkComms(0x38)) return false;
    if (!checkComms(0x39)) return false;
    uv0.begin(VEML6070_1_T);
    //uv0.clearAck();
    uv0.setInterrupt(false);
    return true;

}

void veml6070_read () {
    if (!ModuleData[ve0_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_VE0);
    if (uv0.clearAck()) printf("VE0: Interrupt Trigger Unexpected.\n");
    SensorData.ve0Data.UVI = uv0.readUV();
}

void veml6070_print () {
    printf( "VE0: UVI[%u]\n", SensorData.ve0Data.UVI );
}



//usar outro driver ou driver directo