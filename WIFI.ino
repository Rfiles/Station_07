

void connectwifi_ext () {
  connectwifi();
}

bool connectwifi() {
  if (WiFi.status() == WL_CONNECTED) return true;
  

  wifiMulti.addAP(WIFISSID1, WIFIPASS1);
  wifiMulti.addAP(WIFISSID2, WIFIPASS2);
  
  if(wifiMulti.run(3000) != WL_CONNECTED) return false;


  Serial.print(F("WIFI: OK   IP: "));
  Serial.print(WiFi.localIP());
  Serial.print("  ");
  Serial.println(WiFi.SSID());
  
  timeClient.begin();
  if ( !updateclock() ) printf("TIM: Unable to NTP\n");

  return true;
  
}

bool updateclock () {
  if (WiFi.status() != WL_CONNECTED) return false;

  timeClient.update();
  if ( !timeClient.isTimeSet()  ) return false;
  
  return true;
}

void getclock () { 

  printf("TIM: %s\n", timeClient.getFormattedTime());
  return;
}


