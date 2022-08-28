#include <SI1145_WE.h>

SI1145_WE mySI1145 = SI1145_WE();
uint8_t si1_module_num;


bool si1145_boot ( uint8_t mybootnum ) {
    si1_module_num = mybootnum;
    tcaselect(TCA_SI1);

    if (!checkComms(0x60)) return false;

    mySI1145.init();
    mySI1145.enableHighSignalVisRange();  // Gain divided by 14.5
    mySI1145.enableHighSignalIrRange();   // Gain divided by 14.5
    mySI1145.enableMeasurements(PSALSUV_TYPE, AUTO);
    mySI1145.enableInterrupt(PS_INT);
    mySI1145.setLEDCurrent(5);            // (0 ... 15)

    return true;
}


void si1145_read () {
  if (!ModuleData[si1_module_num].Register.ModuleDetected) return;
  tcaselect(TCA_SI1);
  SensorData.si1145Data.Visible   = mySI1145.getAlsVisData();
  SensorData.si1145Data.IR        = mySI1145.getAlsIrData();
  SensorData.si1145Data.UV        = mySI1145.getUvIndex();
  SensorData.si1145Data.Proximity = mySI1145.getPsData();
}


void si1145_print () {
    printf( "SI1: VIS[%5u] IR[%5u] UV[%5u] PROX[%5u]\n", 
      SensorData.si1145Data.Visible,
      SensorData.si1145Data.IR,
      SensorData.si1145Data.UV,
      SensorData.si1145Data.Proximity );
}


void SetSI1Led (uint8_t current) {
  mySI1145.setLEDCurrent(current);
  printf("SI1: Led Current %u\n", current);

}
