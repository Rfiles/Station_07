#include "Adafruit_VL53L1X.h"


// tentar usar o sparkfun

#define IRQ_PIN 25
#define XSHUT_PIN 26
//Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);
Adafruit_VL53L1X vl53 = Adafruit_VL53L1X();

uint8_t vl53_module_num;
bool GotData = false;
uint16_t ErrorStatus;

bool vl53l1_boot ( uint8_t mybootnum ) {
    vl53_module_num = mybootnum;
    tcaselect(TCA_VL3);

    if (!vl53.begin()) return false;
    vl53.setIntPolarity(true);
    return true;

}

void vl53l1_read () {
  if (!ModuleData[vl53_module_num].Register.ModuleDetected) return;
  tcaselect(TCA_VL3);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
  }

  vl53.setTimingBudget(50);
  int16_t distance;

  if (vl53.dataReady()) { 
    distance = vl53.distance();
    if (distance == -1) {
      ErrorStatus =  vl53.vl_status;
      GotData = false;
      return;
    }
    GotData = true;
    SensorData.vl53l1Data.Distance = distance;
    vl53.clearInterrupt();
    vl53.stopRanging();
  }

}

void vl53l1_print () {
    if (!ModuleData[vl53_module_num].Register.ModuleDetected) return;

    if (GotData) {
      printf( "VL5: Distance [%4u] mm\n",  SensorData.vl53l1Data.Distance );
    } else {
      printf( "VL5: out of range %u\n",  ErrorStatus);
    }

}