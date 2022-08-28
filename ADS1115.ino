#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115    ads;
uint8_t ads_module_num;
uint8_t ads_status;

bool ads_boot ( uint8_t mybootnum ) {
    ads_module_num = mybootnum;
    tcaselect(TCA_ADS);
    ads_status = 0;
    if (!ads.begin()) return false;
    ads_status = 1;
    return true;
}

void ads_read () {
    if (ModuleData[ads_module_num].Register.ModuleDetected) {
        tcaselect(TCA_ADS);
        SensorData.adsData.ch1 = ads.readADC_SingleEnded(0);
        SensorData.adsData.ch2 = ads.readADC_SingleEnded(1);
        SensorData.adsData.ch3 = ads.readADC_SingleEnded(2);
        SensorData.adsData.ch4 = ads.readADC_SingleEnded(3);
    }
}

void ads_print () {
  float VoltagemCalc;
  
    switch (ads_status) {
      case 0:
        printf( "ADS: Not Detected\n" );
        ads_status = 2;
        break;
      case 1:
        VoltagemCalc = (float)SensorData.adsData.ch4 * 0.000188f;
        printf("ADS: 1[%5u] 2[%5u] 3[%5u] 4[%5u](%0.2fV)\n", SensorData.adsData.ch1, SensorData.adsData.ch2, SensorData.adsData.ch3, SensorData.adsData.ch4, VoltagemCalc );
        break;
      case 2:   // silent stop
        break;
    }
    
}

//set alarms

//EOF
