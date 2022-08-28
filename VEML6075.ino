#include "Adafruit_VEML6075.h"

Adafruit_VEML6075 uv5 = Adafruit_VEML6075();
uint8_t ve5_module_num;

bool veml6075_boot ( uint8_t mybootnum ) {
    ve5_module_num = mybootnum;
    tcaselect(TCA_VE5);
    if (!uv5.begin()) return false;
    uv5.setIntegrationTime(VEML6075_50MS);
    uv5.setHighDynamic(true);
    uv5.setForcedMode(false);
    uv5.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                        2.95, 1.74,  // UVB_C and UVB_D coefficients
                        0.001461, 0.002591); // UVA and UVB responses
    return true;

}

void veml6075_read () {
    if (!ModuleData[ve5_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_VE5);

    SensorData.ve5Data.UVA = uv5.readUVA();
    SensorData.ve5Data.UVB = uv5.readUVB();
    SensorData.ve5Data.UVI = uv5.readUVI();

}

void veml6075_print () {
    printf( "VE5: UVA[%.2f] UVB[%.2f] UVI[%.2f]\n", 
            SensorData.ve5Data.UVA,
            SensorData.ve5Data.UVB,
            SensorData.ve5Data.UVI );
}

