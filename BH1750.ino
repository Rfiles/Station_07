#include <BH1750.h>

BH1750 lightMeter;
uint8_t bh_module_num;

bool bh_boot ( uint8_t mybootnum ) {
    bh_module_num = mybootnum;
    tcaselect(TCA_BH1);

    if (!lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE)) return false;
    return true;

}

void bh_read () {
    if (!ModuleData[bh_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_BH1);


  if (lightMeter.measurementReady(true)) {
    float lux = lightMeter.readLightLevel();

    SensorData.bhData.LightLevel = lux;

    if (lux > 40000.0) {
      lightMeter.setMTreg(32);
      return;
    }

    if (lux > 10.0) {
      lightMeter.setMTreg(69);
      return;
    }

    if (lux <= 10.0) {
      lightMeter.setMTreg(138);
      return;
    }

  }

}

void bh_print () {
    printf( "BH1: Lux[%.2f]\n", SensorData.bhData.LightLevel );
}



