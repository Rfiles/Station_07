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




