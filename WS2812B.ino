#include <Adafruit_NeoPixel.h>

#define LED_PIN    33
#define LED_COUNT  3

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint8_t ws2_module_num;

enum ColorName {
  BLACK   = 0x0000,
  BLUE    = 0x001F,
  RED     = 0xF800,
  GREEN   = 0x07E0,
  CYAN    = 0x07FF,
  MAGENTA = 0xF81F,
  YELLOW  = 0xFFE0, 
  WHITE   = 0xFFFF
};

bool ws2812_boot ( uint8_t mybootnum ) {
    ws2_module_num = mybootnum;
    //tcaselect(TCA_SI7);

    //if (!si7021.begin()) return false;

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
    strip.fill(0, 0, 3);
    strip.show();            // Turn OFF all pixels ASAP
    return true;

}

void ws2812_read () {
    if (!ModuleData[ws2_module_num].Register.ModuleDetected) return;
    //tcaselect(TCA_SI7);
    //SensorData.si7021Data.temperature = si7021.readTemperature();
    //SensorData.si7021Data.humidity = si7021.readHumidity();

}

void ws2812_print () {
    //printf( "SI7: T[%.2f] H[%.2f] \n", SensorData.si7021Data.temperature, SensorData.si7021Data.humidity );

    strip.setPixelColor(0, 0, 0, 40);
    strip.setPixelColor(1, 0, 40, 0);
    strip.setPixelColor(2, 40, 0, 0);

    strip.setPixelColor(0, BLUE);
    strip.setPixelColor(1, GREEN);
    strip.setPixelColor(2, RED);


    strip.show();

}

void ws2812_SetRGB(uint8_t n, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(n, g, r, b);
  printf("WS2: RGB set.\n");
  strip.show();
}

void ws2812_SetIntensity(uint8_t Intensity) {
  strip.setBrightness(Intensity); // Set BRIGHTNESS to about 1/5 (max = 255)
  printf("WS2: Intensity set.\n");
  strip.show();            // Turn OFF all pixels ASAP

}



// usar a ColorHSV para funçoes do mpu9550

