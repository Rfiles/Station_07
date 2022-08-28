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
| c |  |
| v |  |
| i |  |
| b |  |
| n |  |
| m |  |

All other commands are ignored with an error message


