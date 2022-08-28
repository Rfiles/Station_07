
#include <PubSubClient.h>
#include <PubSubClientTools.h>
//#define MQTT_SERVER "raspberrypi"

#include <WiFi.h>
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

const char* mqttUser = MQTT_User;
const char* mqttPassword = MQTT_Pass;

#define TamanhoDados 300

void mqttsensordata() {

  //influx(); 

  if (!connectwifi()) {
    printf("MQTT: Wifi Fail..\n");
    return;
  }
  if (!connectmqtt()) {
    ///printf("MQTT: Server Fail..\n");
    return;
  }

  char DadosCSV[TamanhoDados] = "";
  Data2String( DadosCSV );
  //printf("MQTT: %s ", DadosCSV);

  if (client.publish("sensors/test", DadosCSV) == true) {
    //printf("MQTT: Success\n");
  } else {
    printf("MQTT: Part 1 fail\n");
  }
  Data2String_2( DadosCSV );

  if (client.publish("sensors/test2", DadosCSV) == true) {
    //printf("MQTT: Success\n");
  } else {
    printf("MQTT: Part 2 fail\n");
  }
  
  Data2String_3( DadosCSV );
  if (client.publish("sensors/test3", DadosCSV) == true) {
    //printf("MQTT: Success\n");
  } else {
    printf("MQTT: Part 3 fail\n");
  }
  
}

void Data2String (char dest[]) {
    memset ( dest, 0, TamanhoDados );

    sprintf ( dest, "%u,%u,%u,", timeClient.getEpochTime(), SensorData.SamplesCounter, SensorData.toc / 1000 );
    sprintf ( dest, "%s%0.2f,%0.3f,%0.2f,", dest, SensorData.BMEData.temperature, SensorData.BMEData.pressure, SensorData.BMEData.humidity );
    sprintf ( dest, "%s%u,%u,%u,%u,", dest, SensorData.adsData.ch1, SensorData.adsData.ch2, SensorData.adsData.ch3, SensorData.adsData.ch4 );
    sprintf ( dest, "%s%0.2f,%0.2f,", dest, SensorData.si7021Data.temperature, SensorData.si7021Data.humidity );
    sprintf ( dest, "%s%u,%u,", dest, SensorData.CCSData.co2, SensorData.CCSData.volatiles );
    sprintf ( dest, "%s%0.2f,%0.2f,%0.2f,%0.2f,", dest,
        SensorData.micsData.CO,
        SensorData.micsData.NO2,
        SensorData.micsData.NH3,
        SensorData.micsData.C3H8 );
    sprintf ( dest, "%s%0.2f,%0.2f,%0.2f,%0.2f,", dest,
        SensorData.micsData.C4H10,
        SensorData.micsData.CH4,
        SensorData.micsData.H2,
        SensorData.micsData.C2H5OH );
    sprintf( dest, "%s%u,%u,%u,%u,", dest,
        SensorData.si1145Data.Visible,
        SensorData.si1145Data.IR,
        SensorData.si1145Data.UV,
        SensorData.si1145Data.Proximity );
    sprintf( dest, "%s%u,", dest, SensorData.vl53l1Data.Distance );
    sprintf( dest, "%s%0.2f,%0.2f,%0.2f", dest,
        SensorData.ve5Data.UVA,
        SensorData.ve5Data.UVB,
        SensorData.ve5Data.UVI );

    
    //printf ( "MQTT1: Data2String[%u]: .... \n", strlen(dest) );
}

void Data2String_2 (char dest[]) {
    memset ( dest, 0, TamanhoDados );

    sprintf( dest, "%u,%u,%u,", timeClient.getEpochTime(), SensorData.SamplesCounter, SensorData.toc / 1000 );
    sprintf( dest, "%s%0.2f,%0.2f,", dest, SensorData.mlxData.Ambient, SensorData.mlxData.Object );
    sprintf( dest, "%s%0.2f", dest, SensorData.bhData.LightLevel );

    sprintf( dest, "%s%u,%u,%u,%u,%u,", dest,
      SensorData.tcsData.RAW_R,
      SensorData.tcsData.RAW_G,
      SensorData.tcsData.RAW_B,
      SensorData.tcsData.RAW_C,
      SensorData.tcsData.RAW_IR );

    sprintf( dest, "%s%u,%u,%u,%u,%.2f,", dest,
      SensorData.tcsData.COMP_R,
      SensorData.tcsData.COMP_G,
      SensorData.tcsData.COMP_B,
      SensorData.tcsData.COMP_C,
      SensorData.tcsData.LUX );

    sprintf( dest, "%s%.2f,%.2f,%.2f,%.2f,", dest,
      SensorData.tcsData.C_RATIO,
      SensorData.tcsData.MAX_LUX,
      SensorData.tcsData.COLOR_TEMP,
      SensorData.tcsData.CPL );

    sprintf( dest, "%s%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,", dest,
      SensorData.mpuData.Ax,
      SensorData.mpuData.Ay,
      SensorData.mpuData.Az,
      SensorData.mpuData.Gx,
      SensorData.mpuData.Gy,
      SensorData.mpuData.Gz );

    sprintf( dest, "%s%0.2f,%0.2f,%0.2f,%0.2f,", dest,
      SensorData.mpuData.Mx,
      SensorData.mpuData.My,
      SensorData.mpuData.Mz,
      SensorData.mpuData.Temperature );

    sprintf( dest, "%s%u", dest, SensorData.ve0Data.UVI );

      //tcs
    //printf ( "MQTT2: Data2String[%u]: .... \n", strlen(dest) );
}

void Data2String_3 (char dest[]) {
    memset ( dest, 0, TamanhoDados );

    sprintf( dest, "%s%0.2f,%0.2f", dest, SensorData.BMPData.Temperature, SensorData.BMPData.Pressure );

}

bool connectmqtt() {

  uint8_t retries = 3;
 
  while (!client.connected()) {
    Serial.print("MQTT: Connecting.. ");
     if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
     } else {
       printf(" Err[%u]", client.state());
       delay(1000);
       retries--;
       if (retries == 0) {
        mqtt_FailCount--;
        return false;
       }
     }
  }

  mqtt.subscribe("esp32/cmd",  mqttcmd);
  mqtt_FailCount = mqtt_GiveUp;
  return true;
}

void mqttcmd ( String topic, String message ) {
  //Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
  printf("mqttcmd T[%s] M[%s]", topic, message);
}











//EOF
