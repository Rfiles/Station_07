#include "Adafruit_TCS34725.h"

#define TCS34725_R_Coef 0.136
#define TCS34725_G_Coef 1.000
#define TCS34725_B_Coef -0.444
#define TCS34725_GA 1.0
#define TCS34725_DF 310.0
#define TCS34725_CT_Coef 3810.0
#define TCS34725_CT_Offset 1391.0

// Autorange class for TCS34725
class tcs34725 {
private:
  struct tcs_agc {
    tcs34725Gain_t ag;
    uint8_t at;
    uint16_t mincnt;
    uint16_t maxcnt;
  };
  static const tcs_agc agc_lst[];
  uint16_t agc_cur;

  void setGainTime(void);
  Adafruit_TCS34725 tcs;

public:
  tcs34725(void);

  boolean begin(void);
  void getData(void);

  boolean isAvailable, isSaturated;
  uint16_t againx, atime, atime_ms;
  uint16_t r, g, b, c;
  uint16_t ir;
  uint16_t r_comp, g_comp, b_comp, c_comp;
  uint16_t saturation, saturation75;
  float cratio, cpl, ct, lux, maxlux;
};
//
// Gain/time combinations to use and the min/max limits for hysteresis
// that avoid saturation. They should be in order from dim to bright.
//
// Also set the first min count and the last max count to 0 to indicate
// the start and end of the list.
//
const tcs34725::tcs_agc tcs34725::agc_lst[] = {
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_614MS,     0, 20000 },
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_154MS,  4990, 63000 },
  { TCS34725_GAIN_16X, TCS34725_INTEGRATIONTIME_154MS, 16790, 63000 },
  { TCS34725_GAIN_4X,  TCS34725_INTEGRATIONTIME_154MS, 15740, 63000 },
  { TCS34725_GAIN_1X,  TCS34725_INTEGRATIONTIME_154MS, 15740, 0 }
};
tcs34725::tcs34725() : agc_cur(0), isAvailable(0), isSaturated(0) {
}

// initialize the sensor
boolean tcs34725::begin(void) {
  tcs = Adafruit_TCS34725(agc_lst[agc_cur].at, agc_lst[agc_cur].ag);
  if ((isAvailable = tcs.begin()))
    setGainTime();
  return(isAvailable);
}

// Set the gain and integration time
void tcs34725::setGainTime(void) {
  tcs.setGain(agc_lst[agc_cur].ag);
  tcs.setIntegrationTime(agc_lst[agc_cur].at);
  atime = int(agc_lst[agc_cur].at);
  atime_ms = ((256 - atime) * 2.4);
  switch(agc_lst[agc_cur].ag) {
  case TCS34725_GAIN_1X:
    againx = 1;
    break;
  case TCS34725_GAIN_4X:
    againx = 4;
    break;
  case TCS34725_GAIN_16X:
    againx = 16;
    break;
  case TCS34725_GAIN_60X:
    againx = 60;
    break;
  }
}

// Retrieve data from the sensor and do the calculations
void tcs34725::getData(void) {
  // read the sensor and autorange if necessary
  tcs.getRawData(&r, &g, &b, &c);
  while(1) {
    if (agc_lst[agc_cur].maxcnt && c > agc_lst[agc_cur].maxcnt)
      agc_cur++;
    else if (agc_lst[agc_cur].mincnt && c < agc_lst[agc_cur].mincnt)
      agc_cur--;
    else break;

    setGainTime();
    delay((256 - atime) * 2.4 * 2); // shock absorber
    tcs.getRawData(&r, &g, &b, &c);
    break;
  }

  // DN40 calculations
  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;
  r_comp = r - ir;
  g_comp = g - ir;
  b_comp = b - ir;
  c_comp = c - ir;
  cratio = float(ir) / float(c);

  saturation = ((256 - atime) > 63) ? 65535 : 1024 * (256 - atime);
  saturation75 = (atime_ms < 150) ? (saturation - saturation / 4) : saturation;
  isSaturated = (atime_ms < 150 && c > saturation75) ? 1 : 0;
  cpl = (atime_ms * againx) / (TCS34725_GA * TCS34725_DF);
  maxlux = 65535 / (cpl * 3);

  lux = (TCS34725_R_Coef * float(r_comp) + TCS34725_G_Coef * float(g_comp) + TCS34725_B_Coef * float(b_comp)) / cpl;
  ct = TCS34725_CT_Coef * float(b_comp) / float(r_comp) + TCS34725_CT_Offset;
}

tcs34725 rgb_sensor;

uint8_t tcs_module_num;

bool tcs34725_boot ( uint8_t mybootnum ) {
    tcs_module_num = mybootnum;
    tcaselect(TCA_TCS);

    if (!rgb_sensor.begin()) return false;
    return true;

}

void tcs34725_read () {
    if (!ModuleData[tcs_module_num].Register.ModuleDetected) return;
    tcaselect(TCA_TCS);
    rgb_sensor.getData();

    SensorData.tcsData.RAW_R   = rgb_sensor.r;
    SensorData.tcsData.RAW_G   = rgb_sensor.g;
    SensorData.tcsData.RAW_B   = rgb_sensor.b;
    SensorData.tcsData.RAW_C   = rgb_sensor.c;
    SensorData.tcsData.RAW_IR   = rgb_sensor.ir;
    SensorData.tcsData.C_RATIO   = rgb_sensor.cratio;
    SensorData.tcsData.AGAINX   = rgb_sensor.againx;
    SensorData.tcsData.ATIMEMS   = rgb_sensor.atime_ms;
    SensorData.tcsData.CPL   = rgb_sensor.cpl;
    SensorData.tcsData.MAX_LUX   = rgb_sensor.maxlux;
    SensorData.tcsData.COMP_R   = rgb_sensor.r_comp;
    SensorData.tcsData.COMP_G   = rgb_sensor.g_comp;
    SensorData.tcsData.COMP_B   = rgb_sensor.b_comp;
    SensorData.tcsData.COMP_C   = rgb_sensor.c_comp;
    SensorData.tcsData.LUX   = rgb_sensor.lux;
    SensorData.tcsData.COLOR_TEMP   = rgb_sensor.ct;

}

void tcs34725_print () {
    printf( "TCS1: R[%5u] G[%5u] B[%5u] C[%5u] IR[%5u]\n", 
      SensorData.tcsData.RAW_R,
      SensorData.tcsData.RAW_G,
      SensorData.tcsData.RAW_B,
      SensorData.tcsData.RAW_C,
      SensorData.tcsData.RAW_IR );

    printf( "TCS2: C_R[%5u] C_G[%5u] C_B[%5u] R_C[%5u] LUX[%.2f]\n", 
      SensorData.tcsData.COMP_R,
      SensorData.tcsData.COMP_G,
      SensorData.tcsData.COMP_B,
      SensorData.tcsData.COMP_C,
      SensorData.tcsData.LUX );

    printf( "TCS3: CR[%.2f] AGX[%5u] ATMS[%5u] CPL[%.2f]\n", 
      SensorData.tcsData.C_RATIO,
      SensorData.tcsData.AGAINX,
      SensorData.tcsData.ATIMEMS,
      SensorData.tcsData.CPL );
      

    printf( "TCS4: MAX_LUX[%.2f] K[%.2f]\n", 
      SensorData.tcsData.MAX_LUX,
      SensorData.tcsData.COLOR_TEMP );





}



