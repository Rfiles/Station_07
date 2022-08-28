# Station_07

## This is where i try to integrate everything in one project just for fun.

#### TODO:
- [X] make public
- [ ] photo of project
- [ ] schematic of project
- [ ] exemple output
- [X] list of main commands
- [ ] description of archivements so far
- [ ] list of libraries and links/names

Dumb Photo of Device:



Note:
- Serial port is running at 500kBaud

Commands:

| Command | Description |
| --- | --- |
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
| --- | --- |
| d | Read VEML6070 sensor data. CRASHES ESP32. DONT KNOW WHY! |
| z | Erases 23LC1024 RAM. CRASHES ESP32. DONT KNOW WHY! |

All other commands are ignored with an error message.

Timers:
| Number | Time in ms |Description |
| --- | --- | --- |
| 0 | 500 | MCP23017 Port B1 Led Blink |
| 1 | 2500 | Get And Store All Sensors Data |
| 2 | 1234 | Process Values. NOT USED |
| 3 | 5000 | List All Sensors Data in Terminal |
| 4 | 5000 | Send All Sensors Data to MQTT Server |

Example of Serial Output:
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
```


