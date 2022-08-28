#include "credentials.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
WiFiClient espClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 3600*6);


#define TCA_MCP  0
#define TCA_ADS  0
#define TCA_CCS  1
#define TCA_MPU  2
#define TCA_BMP  2
#define TCA_TCS  2
#define TCA_VE5  2
#define TCA_VE0  3
#define TCA_BH1  3
#define TCA_SI7  6
#define TCA_BME  6
#define TCA_MLX  6
#define TCA_SI1  6
#define TCA_VL3  7

#define MCPINT_TCS      0
#define MCPINT_ADS      1
#define MCPINT_CCS      2
#define MCPINT_VL5      3
#define MCPINT_SI1      4
#define MCPINT_VE0      5
#define MCPINT_BUTTON   6

#define MCPOUT_TCS      0
#define MCPOUT_LED      1



bool bme_boot      ( uint8_t mybootnum );
bool sram_boot     ( uint8_t mybootnum );
bool i2cscan_boot  ( uint8_t mybootnum );
bool si7021_boot   ( uint8_t mybootnum );
bool ads_boot      ( uint8_t mybootnum );
bool ccs_boot      ( uint8_t mybootnum );
bool mcp_boot      ( uint8_t mybootnum );
bool mics_boot     ( uint8_t mybootnum );
bool si1145_boot   ( uint8_t mybootnum );
bool vl53l1_boot   ( uint8_t mybootnum );
bool tcs34725_boot ( uint8_t mybootnum );
bool mlx_boot      ( uint8_t mybootnum );
bool bh_boot       ( uint8_t mybootnum );
bool mpu_boot      ( uint8_t mybootnum );
bool veml6075_boot ( uint8_t mybootnum );
bool veml6070_boot ( uint8_t mybootnum );
bool bmp_boot      ( uint8_t mybootnum );
bool ws2812_boot   ( uint8_t mybootnum );


void PingAlive       ( void );
void bme_read        ( void );
void bme_print       ( void );
void bme_cmds        ( void );
void si7021_cmds     ( void );
void si7021_read     ( void );
void si7021_print    ( void );
void ads_read        ( void );
void ads_print       ( void );
void ccs_read        ( void );
void ccs_print       ( void );
void mcp_read        ( void );
void mcp_print       ( void );
void sram_cmds       ( void );
void sram_print_info ( void );
void scan_i2c        ( void );
void CmdTasks        ( void );
void mics_read       ( void );
void mics_print      ( void );
void si1145_read     ( void );
void si1145_print    ( void );
void vl53l1_read     ( void );
void vl53l1_print    ( void );
void tcs34725_read   ( void );
void tcs34725_print  ( void );
void mlx_read        ( void );
void mlx_print       ( void );
void bh_read         ( void );
void bh_print        ( void );
void mpu_read        ( void );
void mpu_print       ( void );
void veml6075_read   ( void );
void veml6075_print  ( void );
void veml6070_read   ( void );
void veml6070_print  ( void );
void bmp_read        ( void );
void bmp_print       ( void );
void ws2812_read     ( void );
void ws2812_print    ( void );



void i2ctca_scanner  ( void );
void AddValues2Mem   ( void );
void ListValues      ( void );
void GatherValues    ( void );
void ProcessValues   ( void );
void ReadValues      ( uint16_t index );
void mqttsensordata  ( void );
void mqttcmd         ( String topic, String message );
void influx          (  void );
void connectwifi_ext ( void );
bool checkComms      ( uint8_t addr );
void ws2812_SetRGB   (uint8_t n, uint8_t r, uint8_t g, uint8_t b);
void SetSI1Led       (uint8_t current);
void ws2812_SetIntensity(uint8_t Intensity);
void IRAM_ATTR eventHappened( void );
void SwitchCalib     ( void );
void sram_write      (uint32_t addr, uint32_t dsize, void* data);
void sram_read       (uint32_t addr, uint32_t dsize, void* dest);


void  mcpWritePin   (uint8_t pin, uint8_t value);
bool  mcpReadPin    (uint8_t pin);
void  mcpPinMode    (uint8_t pin, uint8_t mode);
void  mcpTogglePin  (uint8_t pin);
void  mcpSetALL     ( void );

#define TCAADDR 0x70
#define I2Cclock 400000


uint8_t   ListCount;
uint8_t   statsloop;
uint16_t  MaxReg;
uint16_t  MemIndex;
bool      SetupDone = false;
bool      tcaDetected;
uint8_t   TCA_LastChannel;
const uint8_t mqtt_GiveUp = 3;
uint8_t   mqtt_FailCount = mqtt_GiveUp;
volatile bool mcpintevent = false; 
uint16_t inthit=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef bool (*boot_function_t)(uint8_t);
typedef void (*server_function_t)(void);

struct ModuleInfo {
      bool  ModuleBooted;
  uint32_t  BootStart;
  uint32_t  BootTime;
      bool  ModuleDetected;
      bool  BootNow;
};

struct ModuleConfig {
             char  Name[4];
  boot_function_t  BootFunction;   
server_function_t  ServerFunction;
             bool  StartOnBoot;
};

struct ModulesParameters {
    ModuleInfo  Register;
  ModuleConfig  Config;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct bmeStruct {
  float   temperature;
  float   pressure;
  float   humidity;
};

struct ccsstruct {
  uint16_t co2;
  uint16_t volatiles;
};

struct si7021struct {
  float   temperature;
  float   humidity;
};

struct adsstruct {
  uint16_t  ch1;
  uint16_t  ch2;
  uint16_t  ch3;
  uint16_t  ch4;
};

struct micsStruct {
  float   CO;
  float   NO2;
  float   NH3;
  float   C3H8;
  float   C4H10;
  float   CH4;
  float   H2;
  float   C2H5OH;
};

struct mcpStruct {
  uint8_t   PortA;
  uint8_t   PortB;
  uint8_t   LastInt;
};

struct si1145Struct {
  uint16_t  Visible;
  uint16_t  IR;
  uint16_t  UV;
  uint16_t  Proximity;
};

struct vl53l1Struct {
  uint16_t  Distance;
};

struct tcsStruct {
  uint16_t RAW_R;
  uint16_t RAW_G;
  uint16_t RAW_B;
  uint16_t RAW_C;
  uint16_t RAW_IR;
  float    C_RATIO;
  uint16_t AGAINX;
  uint16_t ATIMEMS;
  float    CPL;
  float    MAX_LUX;
  uint16_t COMP_R;
  uint16_t COMP_G;
  uint16_t COMP_B;
  uint16_t COMP_C;
  float    LUX;
  float    COLOR_TEMP;
};

struct mlxStruct {
  double  Ambient;
  double  Object;
};

struct bhStruct {
  float   LightLevel;
};

struct mpuStruct {
  float   Ax;
  float   Ay;
  float   Az;
  float   Gx;
  float   Gy;
  float   Gz;
  float   Mx;
  float   My;
  float   Mz;
  float   Temperature;
  float   TotalG;
  float   AngleX;
  float   AngleY;
  float   AngleZ;
  float   Pitch;
  float   Roll;

};

struct ve5Struct {
  float   UVA;
  float   UVB;
  float   UVI;
};

struct ve0Struct {
  uint16_t   UVI;
};

struct bmpStruct {
  float   Temperature;
  float   Pressure;
};

struct DataPacket {
  uint32_t        toc;
  uint32_t        SamplesCounter;
  bmeStruct       BMEData;
  si7021struct    si7021Data;
  ccsstruct       CCSData;
  adsstruct       adsData;
  mcpStruct       mcpData;
  micsStruct      micsData;
  si1145Struct    si1145Data;
  vl53l1Struct    vl53l1Data;
  tcsStruct       tcsData;
  mlxStruct       mlxData;
  bhStruct        bhData;
  mpuStruct       mpuData;
  ve5Struct       ve5Data;
  ve0Struct       ve0Data;
  bmpStruct       BMPData;

};


typedef void (*task_function_t)(void);

struct timer_def {
    bool            Enabled;
    bool            ExecuteNow;
    uint32_t        Timer_;
    uint32_t        HitCount;
    task_function_t task_function;  // function pointer
};


DataPacket  SensorData;  //, RetrievedSensorData;
const uint8_t SensorData_size = sizeof(SensorData);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '#' : '-'), \
  (byte & 0x40 ? '#' : '-'), \
  (byte & 0x20 ? '#' : '-'), \
  (byte & 0x10 ? '#' : '-'), \
  (byte & 0x08 ? '#' : '-'), \
  (byte & 0x04 ? '#' : '-'), \
  (byte & 0x02 ? '#' : '-'), \
  (byte & 0x01 ? '#' : '-') 









//EOF
