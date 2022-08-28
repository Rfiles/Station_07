# Station_07

## This is where i try to integrate everything in one project just for fun.

#### TODO:
- [X] make public
- [ ] photo of project
- [ ] schematic of project
- [X] exemple output
- [X] list of main commands
- [ ] description of archivements so far
- [ ] list of libraries and links/names
- [X] list of modules used

Dumb Photo of Device:



Note:
- Serial port is running at 500kBaud


## Modules Used In This Project:
| HardWare | What I get |
| :---: | :--- |
| BME280 | Temperature, Pressure, Humidty |
| ADS1115 | 4 channel 15 bit ADC. Channel 4 connected to 3.3V|
| BH1750 | Light Intensity |
| BMP280 | Temperature, Pressure |
| CCS811 | Equivalent CO2, Amount of Volatile Gases |
| MCP23017 | 16 pin Input/Output Expander |
| VL53L1 | Laser Distance Sensor up to 4 meters |
| VEML6070 | UV intensity |
| 23LC1024 | 128kB External SPI RAM. Store Values. Pointless for now |
| WS2812B | 3 modules full RGB configurable LEDS |
| MICS6814 | 3 Gases Sensor. Uses 3 channels of ADS1115. Measures 8 Gases |
| MPU9250 | 9 Axis Montion Sensor. Accelerometer, Gyroscope, Magnetometer (3 axis each). Also chip temperature |
| SI1145 | Light and proximity sensor. Infrared, UltraViolet, Visible, Proximity (With white LED) |
| VEML6075 | Ultraviolet Sensor. UVa, UVb, UV intensity |
| SI7021 | Temperature, Humidity  |
| TCS34725 | Light Sensor. Red, Green, Blue, Clear, IR, (LUX, K, CPL calculated) |
| TCA9548A | I2C Switch and level translator for all I2C devices |
| MLX90614 | Infrared Temperature Sensor. Object Temperature, Ambient Temperature |



| SoftWare | What I Get |
| :---: | :--- |
| WiFi | WiFi |
| INFLUX | Direct Connection to Database. DOESNT WORK |
| MQTT | Send Data to Nodered in raspberry pi 3B+ |
| OTA | Auto check for Update in raspberry pi server, webpage server for update |
| I2C Scanner | Scans TCA9548 for devices and lists in Serial Port |




## TCA9548 channels used:
| Channel | What is Connected |
| :---: | :--- |
| 0 | MCP23017, ADS1115 |
| 1 | CCS811 |
| 2 | MPU9250, BMP280, TCS34725, VEML6075 |
| 3 | VEML6070 |
| 4 |  |
| 5 |  |
| 6 | SI7021, SI1145, MLX90614, BME280 |
| 7 | VL53L1 |




## MCP23017 Pins Used:
| PIN | What is Connected |
| :---: | :--- |
| A0 | TCS34725 Interrupt |
| A1 | ADS1115 Alarm |
| A2 | CCS811 Interrupt |
| A3 | VL53L1 GPIO1 |
| A4 | SI1145 Interrupt |
| A5 | VEML6070 Ack |
| A6 | User Button Active Low |
| B0 | TCS34725 LED |
| B1 | User Red LED |




# Commands:

| Command | Description |
| :---: | :--- |
| 0 | Force Listing of stored Values |
| 1 | Force Gather Values from Sensors |
| e | Force Boot Modules |
| s | Execute Create Timers Function |
| g | Start / Stop Timers (g <Timer #> on/off )|
| r | Force Send Data to Influx Database (Does Not Word) |
| i | Force Send Sensors Data to MQTT server (Nodered in my case) |
| o | Connect to configured WIFI network |
| w | Sync with NTP server and Show Clock |
| h | Toggle MCP23017 Pin (h <pin # in port B> [optional 1/0, default is 1] |
| j | Set Si1145 Led Current (j <0..15>) Its from 1mA to 300mA |
| p | Set WS2812 Color (p (Red) (Green) (Blue) [Optional Led #, default 0] Colors in 8bit |
| k | Set WS2812 Intensity (k <intensity 8bits>) |
| l | Toggle MICS6814 Calibration Source. Preset/ADC mean since boot in 10 samples. |
| x | Set MCP23017 pin. (x <1/0> [Pin# in port B] Default is Pin B1 |
| c | Toggle all MCP23017 Port B pins on/off. |
| v | Set MPC23017 Port B Pin input or output. (v <1/0> [Port B pin. Default 1]) |
| b | Boot Specific Module Number. (b <module #>) |
| n | Change first 3 WS2812 leds to Red, Green, Blue. |
| d | Read VEML6070 sensor data. CRASHES ESP32. DONT KNOW WHY! |
| z | Erases 23LC1024 RAM. CRASHES ESP32. DONT KNOW WHY! |

All other commands are ignored with an error message.



## Timers:
| Number | Time in ms |Description |
| :---: | :---: | :--- |
| 0 | 500 | MCP23017 Port B1 Led Blink |
| 1 | 2500 | Get And Store All Sensors Data |
| 2 | 1234 | Process Values. NOT USED |
| 3 | 5000 | List All Sensors Data in Terminal |
| 4 | 5000 | Send All Sensors Data to MQTT Server |



### Example of Serial Output:
```
Aug 28 2022 16:57:561
Station_07 Loader of 18 Modules:
Primary I2C Scan
 Got = 23  38  39  70  
Primary I2C Scan Done
TCA I2C Scan
 Port 0 Got =	00	20	48	
 Port 1 Got =	5A	
 Port 2 Got =	10	29	68	76	
 Port 3 Got =	23	38	39	
 Port 4 Got =	
 Port 5 Got =	
 Port 6 Got =	00	40	5A	60	76	
 Port 7 Got =	29	
TCA I2C Scan Done
 00: SCN S[  23] D[158] OK
 01: RAM S[ 181] D[  1] OK
 02: SI7 S[ 182] D[ 53] OK
 03: BME S[ 235] D[123] OK
 04: ADS S[ 358] D[  0] OK
 05: MCP S[ 358] D[ 10] OK
 06: CCS S[ 368] D[204] OK
 07: MIC S[ 572] D[  0] OK
 08: SI1 S[ 572] D[ 52] OK
E (626) gpio: gpio_set_level(226): GPIO output gpio_num error
E (636) gpio: gpio_set_level(226): GPIO output gpio_num error
 09: VL5 S[ 624] D[ 72] OK
 10: TCS S[ 696] D[620] OK
 11: MLX S[1316] D[  1] OK
 12: BH1 S[1317] D[ 20] OK
 13: MPU S[1337] D[639] OK
 14: VE0 S[1976] D[  4] OK
 15: BMP S[1980] D[108] OK
 16: VL0 S[2088] D[2001] OK
 17: WS2 S[4089] D[  1] OK
Load Done
RAM: Total 0528 regs of 0248 bytes each.
Starting Timed Tasks

OC: 000000   TOC: 000000   TIM: 01:00:12
BME: T[0.00] P[0.000] H[0.00] 
BMP: T[0.00] P[0.000]
SI7: T[0.00] H[0.00] 
CCS: Not Detected
ADS: 1[    0] 2[    0] 3[    0] 4[    0](0.00V)
MIC: Calibrating.. 0/10
BH1: Lux[0.00]
SI1: VIS[    0] IR[    0] UV[    0] PROX[    0]
TCS1: R[    0] G[    0] B[    0] C[    0] IR[    0]
TCS2: C_R[    0] C_G[    0] C_B[    0] R_C[    0] LUX[0.00]
TCS3: CR[0.00] AGX[    0] ATMS[    0] CPL[0.00]
TCS4: MAX_LUX[0.00] K[0.00]
VE5: UVA[0.00] UVB[0.00] UVI[0.00]
VL5: out of range 0
MLX: Amb[0.00] Obj[0.00]
MPU: Ax[0.00] Ay[0.00] Az[0.00] - Gx[0.00] Gy[0.00] Gz[0.00]
MPU: Mx[0.00] My[0.00] Mz[0.00] SumG[0.00] T[0.00'C]
MPU: ANx[0.00] ANy[0.00] ANz[0.00] Pitch[0.00] Roll[0.00] M[0]


```






