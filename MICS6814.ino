

uint8_t mics_module_num;
uint8_t cal_count = 0;
uint8_t mics_status = 0;

uint16_t Ave_CO = 0;
uint16_t Ave_NH3 = 0;
uint16_t Ave_NO2 = 0;

const uint16_t Cal_CO  = 100; // valores medios definidos por min
const uint16_t Cal_NH3 = 600;
const uint16_t Cal_NO2 = 70;

bool UseConsts = true;

const uint8_t Calib_Counts = 10;

//uint32_t dbg_calc_time_1, dbg_calc_time_2;


bool mics_boot ( uint8_t mybootnum ) {
    mics_module_num = mybootnum;
    cal_count = 0;
    mics_status = 0;
    return true;
}

void mics_read () {
  
  if (mics_status == 2) return;   // silent stop
  
  if (cal_count < Calib_Counts) {
    mics_status = 0;
    PrepCalib();
    return;
  }
  mics_status = 1;

}

void mics_print () {
  switch (mics_status) {
    case 0:
      printf("MIC: Calibrating.. %u/10\n", cal_count);
      break;
    case 1:
      CalcGases();
      //printf("MIC: calc time = [%u]\n", dbg_calc_time_2 - dbg_calc_time_1);
      printf("MIC: Carb Mono[%.2f]  Diox Nitr[%.2f]  Amonio[%.2f] Propano[%.2f]\n",
      SensorData.micsData.CO,
      SensorData.micsData.NO2,
      SensorData.micsData.NH3,
      SensorData.micsData.C3H8 );
      printf("MIC: Butano[%.2f] Metano[%.2f] Hidrog[%.2f] Etanol[%.2f]\n",
      SensorData.micsData.C4H10,
      SensorData.micsData.CH4,
      SensorData.micsData.H2,
      SensorData.micsData.C2H5OH );
      break;
    case 2:
      break;
  }
}

void CalcGases() {
  //dbg_calc_time_1 = millis();   4ms
  float Ratio_CO;
  float Ratio_NH3;
  float Ratio_NO2;

  if (mics_status == 2) return;   // silent stop

  if ( !Ave_CO || !Ave_NH3 || !Ave_NO2 ) {
    printf("MIC: Calib Error. ADS not plausible\n");
    mics_status = 2;
    return;
  }


  if (UseConsts) {
    Ratio_CO  = (float)SensorData.adsData.ch1 / (float)Cal_CO;
    Ratio_NH3 = (float)SensorData.adsData.ch2 / (float)Cal_NH3;
    Ratio_NO2 = (float)SensorData.adsData.ch3 / (float)Cal_NO2;
  }else{
    Ratio_CO  = (float)SensorData.adsData.ch1 / (float)Ave_CO;  // * (32768.0 - Ave_CO)  / (32768.0 - SensorData.adsData.ch1);
    Ratio_NH3 = (float)SensorData.adsData.ch2 / (float)Ave_NH3; // * (32768.0 - Ave_NH3) / (32768.0 - SensorData.adsData.ch2);
    Ratio_NO2 = (float)SensorData.adsData.ch3 / (float)Ave_NO2; // * (32768.0 - Ave_NO2) / (32768.0 - SensorData.adsData.ch3);
  }

  //printf("MIC: ratio u CO[%u] NH3[%u] NO2[%u]\n", Ratio_CO, Ratio_NH3, Ratio_NO2);
  //printf("MIC: ratio f CO[%f] NH3[%f] NO2[%f]\n", (float)Ratio_CO, (float)Ratio_NH3, (float)Ratio_NO2);
  //printf("MIC: Ratio CO[%.4f] NH3[%.4f] NO2[%.4f]\n", Ratio_CO, Ratio_NH3, Ratio_NO2);
  
  SensorData.micsData.CO     = pow( Ratio_CO,  -1.179) * 4.385;
  SensorData.micsData.NO2    = pow( Ratio_NO2,  1.007) / 6.855;
  SensorData.micsData.NH3    = pow( Ratio_NH3, -1.67)  / 1.47;
  SensorData.micsData.C3H8   = pow( Ratio_NH3, -2.518) * 570.164; 
  SensorData.micsData.C4H10  = pow( Ratio_NH3, -2.138) * 398.107;
  SensorData.micsData.CH4    = pow( Ratio_CO,  -4.363) * 630.957;
  SensorData.micsData.H2     = pow( Ratio_CO,  -1.8)   * 0.73;
  SensorData.micsData.C2H5OH = pow( Ratio_CO,  -1.552) * 1.622;
  //dbg_calc_time_2 = millis();
}

void PrepCalib () {

  Ave_CO  += SensorData.adsData.ch1;
  Ave_NH3 += SensorData.adsData.ch2;
  Ave_NO2 += SensorData.adsData.ch3;
  //printf("MIC: [%u] += : CO[%u] NH3[%u] NO2[%u]\n", cal_count, Ave_NH3, Ave_NH3, Ave_NO2);
  
  cal_count++;

  if (cal_count == Calib_Counts) {
    CalcCalib();
  }
}

void CalcCalib () {

  Ave_CO  /= Calib_Counts;  // average
  Ave_NO2 /= Calib_Counts;
  Ave_NH3 /= Calib_Counts;
  if (!UseConsts) printf("MIC: Average : CO[%u] NH3[%u] NO2[%u]\n", Ave_CO, Ave_NH3, Ave_NO2);

}

void SwitchCalib (){
  UseConsts = !UseConsts;

  if (UseConsts) {
    printf("MICS: Calib->Stored Constants\n");
  }else{
    printf("MICS: Calib->Average Offset Since Start\n");
  }


}





//EOF
