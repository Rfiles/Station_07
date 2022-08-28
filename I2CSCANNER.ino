


void scan_i2c() {
  byte error, address;

  Serial.println(F("Primary I2C Scan"));
  //configMPU6050(0x68);
  //configMPU6050(0x69);
  Serial.print(F(" Got = "));

  for(address = 1; address < 127; address++ ) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0){
      if (address < 16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.print(F("  "));
      if (address == TCAADDR) tcaDetected = true;
    } 
    //delay(5); //Scan not too fast
  }
  Serial.println();
  Serial.println(F("Primary I2C Scan Done"));
}

void i2ctca_scanner() {
  //byte error, addr;
  Serial.print(F("TCA I2C Scan"));

  for (uint8_t t=0; t<8; t++) {
    tcaselect(t);
    //configMPU6050(0x68);
    //configMPU6050(0x69);
    
    Serial.println();
    Serial.print(F(" Port "));
    Serial.print(t);
    Serial.print(F(" Got =\t"));

    for (uint8_t addr = 0; addr<=127; addr++) {
      if (addr == TCAADDR) continue;

      //uint8_t data;
      Wire.beginTransmission(addr);
      byte error = Wire.endTransmission();
      if (error == 0) {
        if (addr < 16) Serial.print("0");
        Serial.print(addr,HEX);
        Serial.print(F("\t"));
      }
      //delay(5); //Scan not too fast
    }
  }
  Serial.println();
  Serial.println(F("TCA I2C Scan Done"));
}

bool i2cscan_boot (  uint8_t mybootnum ) {
    tcaselect(4);
    scan_i2c();
    if (!tcaDetected) {
        Serial.println(F("TCA not found"));
        return(false);
    }
    i2ctca_scanner();
    return(true);
    
}



//EOF
