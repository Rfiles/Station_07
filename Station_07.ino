#include <Wire.h>
#include <SPI.h>
#include "Header.h"
#include <SerialCommand.h>

#define STATION_VERSION  1

#define LED_BUILTIN 2
int MCPINTPIN = 34;

SerialCommand keycmd;

timer_def Tasks[] = {
    {   // Ping
        true,             // Enabled
        false,            // Execute Now
        500,             // Timer in Millis
        0,
        PingAlive         // task_function
    },  
    {   // Get and Store Values
        true,             // Enabled
        false,            // Execute Now
        2500,             // Timer in Millis
        0,
        GatherValues      // task_function
    },
    {   // Process Values Stored
        false,            // Enabled
        false,            // Execute Now
        1234,             // Timer in Millis
        0,
        ProcessValues     // task_function
    },  
    {   // Print Values
        true,             // Enabled
        false,            // Execute Now
        5000,             // Timer in Millis
        0,
        ListValues        // task_function
    },
    {   // Send MQTT
        false,            // Enabled
        false,            // Execute Now
        5000,             // Timer in Millis
        0,
        mqttsensordata    // task_function
    }
};

#define COUNT_TASKS sizeof(Tasks)    / sizeof(timer_def)

TimerHandle_t xTimers[ COUNT_TASKS ];

ModulesParameters ModuleData[] = {
  //SET:Name[3] BootFunction     ServerFunction    StartOnBoot
  {{},{ "SCN",  i2cscan_boot,    NULL,             true   }},  // 00
  {{},{ "RAM",  sram_boot,       NULL,             true   }},  // 01
  {{},{ "SI7",  si7021_boot,     si7021_read,      true   }},  // 02
  {{},{ "BME",  bme_boot,        bme_read,         true   }},  // 03
  {{},{ "ADS",  ads_boot,        ads_read,         true   }},  // 04
  {{},{ "MCP",  mcp_boot,        mcp_read,         true   }},  // 05
  {{},{ "CCS",  ccs_boot,        ccs_read,         true   }},  // 06
  {{},{ "MIC",  mics_boot,       mics_read,        true   }},  // 07
  {{},{ "SI1",  si1145_boot,     si1145_read,      true   }},  // 08
  {{},{ "VL5",  vl53l1_boot,     vl53l1_read,      true   }},  // 09
  {{},{ "TCS",  tcs34725_boot,   tcs34725_read,    true   }},  // 10
  {{},{ "MLX",  mlx_boot,        mlx_read,         true   }},  // 11
  {{},{ "BH1",  bh_boot,         bh_read,          true   }},  // 12
  {{},{ "MPU",  mpu_boot,        mpu_read,         true   }},  // 13
  {{},{ "VE0",  veml6075_boot,   veml6075_read,    true   }},  // 14
  {{},{ "BMP",  bmp_boot,        bmp_read,         true   }},  // 15
  {{},{ "VL0",  veml6070_boot,   veml6070_read,    true   }},  // 16
  {{},{ "WS2",  ws2812_boot,     NULL,             true   }}   // 17
};

const uint8_t NUM_MODS = ( sizeof(ModuleData) / ( sizeof(ModuleInfo) + sizeof(ModuleConfig) ) );

void BootModules() {
  bool ModuleRun = false;
  printf("Loader of %u Modules:\n", NUM_MODS);
  
  for (uint8_t TempLoop = 0; TempLoop < NUM_MODS; TempLoop++ ) {
    ModuleRun = false;
    if (!SetupDone) {
      if ( ModuleData[TempLoop].Config.StartOnBoot )  {         // boot at startup
        ModuleData[TempLoop].Register.BootStart = millis();
        ModuleData[TempLoop].Register.ModuleDetected = (*ModuleData[TempLoop].Config.BootFunction)(TempLoop);
        ModuleData[TempLoop].Register.BootTime = millis() - ModuleData[TempLoop].Register.BootStart;
        ModuleRun = true;
      }
    }
    if ( ModuleData[TempLoop].Register.BootNow ) {              // Boot Now?
        ModuleData[TempLoop].Register.BootStart = millis();
        ModuleData[TempLoop].Register.ModuleDetected = (*ModuleData[TempLoop].Config.BootFunction)(TempLoop);
        ModuleData[TempLoop].Register.BootTime = millis() - ModuleData[TempLoop].Register.BootStart;
        ModuleData[TempLoop].Register.BootNow = false;
        ModuleRun = true;
    }

    if ( ModuleRun ) {
      printf(" %02u: %s S[%4u] D[%3u] ", 
            TempLoop, 
            ModuleData[TempLoop].Config.Name, 
            ModuleData[TempLoop].Register.BootStart, 
            ModuleData[TempLoop].Register.BootTime );
      if (ModuleData[TempLoop].Register.ModuleDetected) {
        printf("OK\n");
      } else {
        printf("FAIL\n");
      }
    }
    yield();
  }
  printf("Load Done\n");
}

void setup(){
    //setlocale(LC_NUMERIC, "German");  //para o floating point ser com virgula
    Serial.begin(500000);
    while(!Serial);
    Serial.println();
    printf("Station_07 ");
    Serial.print(__DATE__);  Serial.print(F(" "));  Serial.print(__TIME__); Serial.println(STATION_VERSION);
        //Serial.println(__FILE__); 
    SPI.begin();
    //SPI.SPISettings(400000, MSBFIRST, SPI_MODE0)
    Wire.begin();
    pinMode( LED_BUILTIN, OUTPUT );
    pinMode( MCPINTPIN  , INPUT  );
    attachInterrupt( MCPINTPIN , eventHappened, FALLING );

    BootModules();
    
    sram_print_info();
    //connectwifi();
    //setupota();
    //AutoUpdate();
    //setuptelnet();
    SetupKeyCmds();
    CreateTimers();

    SetupDone = true;

}

void loop() {
  keycmd.readSerial(); /* Process data from serial port each iteration */

  for (uint8_t loop = 0; loop < COUNT_TASKS; loop++ ) {
    if ( Tasks[loop].ExecuteNow ) {
      (Tasks[loop].task_function)();
      Tasks[loop].ExecuteNow = false;
    }
    if (mcpintevent) mcp_eventcheck();

  }


}

void tcaselect( uint8_t i ) {
  if (i > 7) return;
    if (i == TCA_LastChannel) return;
    TCA_LastChannel = i;
    if (tcaDetected) {        
      //Wire.beginTransmission(TCAADDR);
      //Wire.write(0);
      //Wire.endTransmission();
      Wire.beginTransmission(TCAADDR);
      Wire.write(1 << i);
      Wire.endTransmission();
    }
}

void PingAlive() {
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    mcpTogglePin(MCPOUT_LED);
    // ws2812B

}

void StoreValues ( uint16_t index ) {
    uint32_t baseaddr;
    baseaddr = (index * SensorData_size);
    //Serial.print(F("StoreValues @: ")); Serial.println( baseaddr );

    sram_write(baseaddr, SensorData_size, &SensorData);

}

void ReadValues ( uint16_t index ) {
    uint32_t baseaddr;
    baseaddr = (index * SensorData_size);
    DataPacket RetrievedSensorData;
    sram_read(baseaddr, SensorData_size, &RetrievedSensorData);

}

void Copy2Active() {
 //    memcpy( &SensorData, &RetrievedSensorData, SensorData_size );
    /////memcpy( &SimSensorData, &RetrievedSensorData, SensorData_size );
}

void AddValues2Mem () {
    //Serial.print(F("AddValues2Mem IDX: ")); Serial.println( MemIndex );
    //Serial.print(F("MemIndex: ")); Serial.print(MemIndex);
    StoreValues( MemIndex );
    MemIndex++; //
    if ( MemIndex > MaxReg ) MemIndex = 0;

}

void ListValues() {
    //PrintLastValues( MemIndex, 5 );
    //printf("Ram Index = %u\n", MemIndex);

    printf("\nNOC: %06u   TOC: %06u   ", SensorData.SamplesCounter, SensorData.toc / 1000);
    getclock();
    
    if (ModuleData[3].Register.ModuleDetected) bme_print();
    if (ModuleData[15].Register.ModuleDetected) bmp_print();
    if (ModuleData[2].Register.ModuleDetected) si7021_print();

    if (ModuleData[6].Register.ModuleDetected) ccs_print();
    if (ModuleData[4].Register.ModuleDetected) ads_print();
    if (ModuleData[7].Register.ModuleDetected) mics_print();
    if (ModuleData[5].Register.ModuleDetected) mcp_print();

    if (ModuleData[12].Register.ModuleDetected) bh_print();
    if (ModuleData[8].Register.ModuleDetected) si1145_print();
    if (ModuleData[10].Register.ModuleDetected) tcs34725_print();
    if (ModuleData[14].Register.ModuleDetected) veml6075_print();
    if (ModuleData[16].Register.ModuleDetected) veml6070_print();

    if (ModuleData[9].Register.ModuleDetected) vl53l1_print();
    if (ModuleData[11].Register.ModuleDetected) mlx_print();

    if (ModuleData[13].Register.ModuleDetected) mpu_print();

}

void GatherValues () {

    SensorData.toc = millis();
    SensorData.SamplesCounter++;

    for (uint8_t Loop=0; Loop < NUM_MODS; Loop++) {
      if (ModuleData[Loop].Register.ModuleDetected) {
        if (ModuleData[Loop].Config.ServerFunction) (*ModuleData[Loop].Config.ServerFunction)();
      }
    }

    AddValues2Mem();

}

void ProcessValues () {
    //bme_stats();
    //si7021_stats();
    //ads_stats();
    //si7021_triggers();
    //bme_triggers();
    //ads_triggers();
    //ads_print();
}

void CmdTasks () {
  int aNumber;
  char *arg;
  uint8_t Task_Num;

  //printf("Task Switch\n");

  arg = keycmd.next();
  if (arg != NULL) {
    Task_Num = atoi(arg);
    if ( Task_Num > COUNT_TASKS ) {
      printf("CMD: Task Num Exceeds\n");
      return;
    }
  } else {
    printf("CMD: Missing Task Num\n");
    return;
  }

  arg = keycmd.next();
  if (arg != NULL) {
    printf("CMD: Task[%u]=", Task_Num);
      if (!strcmp( arg, "on" )) {
        printf("ON\n");
        Tasks[Task_Num].Enabled = true;
        xTimerStart( xTimers[ Task_Num ], 0 );
      }else{
        printf("OFF\n");
        Tasks[Task_Num].Enabled = false;
        xTimerStop( xTimers[ Task_Num ], 0 );
      }
  } else {
    Tasks[Task_Num].Enabled = !Tasks[Task_Num].Enabled;

    if ( Tasks[Task_Num].Enabled ) {
      xTimerStart( xTimers[ Task_Num ], 0 );
    }else{
      xTimerStop( xTimers[ Task_Num ], 0 );
    }

    printf("CMD: Task[%u]=%b\n", Task_Num, Tasks[Task_Num].Enabled);
    return;
  }


}

void SetupKeyCmds () {
  // this file stuff
  keycmd.addCommand("0", ListValues);
  keycmd.addCommand("1", GatherValues);
  keycmd.addCommand("e", BootModules);
  keycmd.addCommand("s", CreateTimers );
  keycmd.addCommand("g", CmdTasks );

  // simple modules print commands
  keycmd.addCommand("2", bme_print);
  keycmd.addCommand("3", ads_print);
  keycmd.addCommand("4", si7021_print);
  keycmd.addCommand("5", ccs_print);
  keycmd.addCommand("6", mics_print);
  keycmd.addCommand("7", mcp_print);
  keycmd.addCommand("8", si1145_print);
  keycmd.addCommand("9", vl53l1_print);
  keycmd.addCommand("q", tcs34725_print);
  keycmd.addCommand("t", mlx_print);
  keycmd.addCommand("y", bh_print);
  keycmd.addCommand("u", mpu_print);
  keycmd.addCommand("a", veml6075_print );
  keycmd.addCommand("f", bmp_print );

  // wifi stuff
  keycmd.addCommand("r", influx );
  keycmd.addCommand("i", mqttsensordata);
  keycmd.addCommand("o", connectwifi_ext);
  keycmd.addCommand("w", getclock);
  
  // crashes esp32
  keycmd.addCommand("d", veml6070_read );
  keycmd.addCommand("z", EraseRamChip );

  // this file commands
  keycmd.addCommand("h", ToggleMCPpin );   // h [Pin B num]  <default is 1>
  keycmd.addCommand("j", Change_SI1_Led ); // j (current)
  keycmd.addCommand("p", ws2812_SetC );    // p (Red) (Green) (Blue) [LED num] <default is led 0>
  keycmd.addCommand("k", ws2812_SetI );    // k (intensity)
  keycmd.addCommand("l", SwitchCalib );    // l <default is preset values>
  keycmd.addCommand("x", SetMCPpin );      // x (1/0) [Pin B num] <default is pin 1>
  keycmd.addCommand("c", mcpSetALL );
  keycmd.addCommand("v", SetMCPpinMode );
  keycmd.addCommand("b", BootModuleCmd );  // b (Index)
  keycmd.addCommand("n", ws2812_print );

  // spare commands
  keycmd.addCommand("m", mcpSetALL );
  
  // what was that command
  keycmd.addDefaultHandler( unrecognized );

}

void unrecognized() {
  printf("cmd?\n");
}

void CreateTimers () {

  printf("Starting Timed Tasks\n");
  for( uint8_t x = 0; x < COUNT_TASKS; x++ ) {
      //printf("Timer[%u]: Create. (%u)ms\n", x, Tasks[x].Timer_ * 100);
      xTimers[ x ] = xTimerCreate(  "Timer",                // Just a text name, not used by the kernel.
                                  Tasks[x].Timer_,          // The timer period in ms (1 tick/ms in esp32).
                                  pdTRUE,                   // The timers will auto-reload themselves when they expire.
                                  ( void * ) x,             // Assign each timer a unique id equal to its array index.
                                  timer_callback            // Each timer calls the same callback when it expires.
                                );
      if( xTimers[ x ] == NULL ) {
        printf( "Timer[%u]: Creation Error.\n", x );
      } else {
        if ( Tasks[x].Enabled ) { 
          //printf("Timer[%u]: Start\n", x);
          if( xTimerStart( xTimers[ x ], 0 ) != pdPASS )  {
            printf( "Timer[%u]: Activation Error.\n", x );
          }
        }
      }
  }
}

void timer_callback ( TimerHandle_t pxTimer ) {
  int32_t lArrayIndex, xTimerPeriod;
  configASSERT( pxTimer );
  lArrayIndex = ( int32_t ) pvTimerGetTimerID( pxTimer );
  xTimerPeriod = xTimerGetPeriod( pxTimer );

  Tasks[(uint8_t)lArrayIndex].HitCount++;
  Tasks[(uint8_t)lArrayIndex].ExecuteNow = true;

   //  printf("Timer[%d] Executed. Period: %u ms.\n", lArrayIndex, xTimerPeriod);
}

bool checkComms ( uint8_t addr ) {
    Wire.beginTransmission(addr);
    return !Wire.endTransmission();
}

void IRAM_ATTR eventHappened(){
  inthit++;
  mcpintevent = true;
}

void ToggleMCPpin() {
  char *arg;
  uint8_t ch = 1;

  arg = keycmd.next(); 
  if (arg != NULL) ch = atoi(arg);

  mcpTogglePin(ch);

}

void ws2812_SetC() {
  char *arg;
  uint8_t r, g, b;
  uint8_t ch = 0;

  arg = keycmd.next(); 
  if (arg != NULL) {
    r = atoi(arg);
  } else {
    Serial.println("Falta: R G B\n");
    return;
  }

  arg = keycmd.next(); 
  if (arg != NULL) {
    g = atoi(arg);
  } else {
    Serial.println("Falta: G B\n");
    return;
  }

  arg = keycmd.next(); 
  if (arg != NULL) {
    b = atoi(arg);
  } else {
    printf("Falta: B\n");
    return;
  }

  arg = keycmd.next(); 
  if (arg != NULL) ch = atoi(arg);

  ws2812_SetRGB(ch, r, g, b);

}

void ws2812_SetI() {
  char *arg;
  int valor;

  arg = keycmd.next(); 
  if (arg != NULL) {
    valor = atoi(arg);
  } else {
    Serial.println("CMD: Value Missing\n");
    return;
  }

  ws2812_SetIntensity(valor);
}

void Change_SI1_Led () {
  char *arg;
  int valor;


  arg = keycmd.next(); 
  if (arg != NULL) {
    valor = atoi(arg);
  } else {
    Serial.println("CMD: Value Missing\n");
    return;
  }

  SetSI1Led( valor );

}

void SetMCPpin() {
  char *arg;
  uint8_t ch = 1;
  uint8_t mode = 0;

  arg = keycmd.next(); 
  if (arg != NULL) {
    mode = atoi(arg);
    if (mode > 0) mode = 1;
  } else {
    Serial.println("Falta: <1/0> [pin=1]\n");
    return;
  }

  arg = keycmd.next(); 
  if (arg != NULL) ch = atoi(arg);

  mcpWritePin(ch, mode);

}

void SetMCPpinMode() {
  char *arg;
  uint8_t ch = 1;
  uint8_t mode = 0;

  arg = keycmd.next(); 
  if (arg != NULL) {
    mode = atoi(arg);
    if (mode > 0) mode = 1;
  } else {
    Serial.println("Falta: <1/0> [pin=1]\n");
    return;
  }

  arg = keycmd.next(); 
  if (arg != NULL) ch = atoi(arg);

  mcpPinMode(ch, mode);

}

void BootModuleCmd () {
  char *arg;
  int indice;

  arg = keycmd.next(); 
  if (arg != NULL) {
    indice = atoi(arg);
  } else {
    printf("CMD: Missing Module Index\n");
    return;
  }

  if (indice > NUM_MODS) {
    printf("CMD: Invalid Module Index\n");
  }else{
    ModuleData[indice].Register.BootNow = true;
    BootModules();
  }
}
//EOF
