/*#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 myMCP;

uint8_t mcp_module_num;
uint8_t mcp_status;
uint16_t IntCounter;
uint8_t intCapReg;

bool mcp_boot ( uint8_t mybootnum ) {
    mcp_module_num = mybootnum;
    tcaselect(TCA_MCP);
    mcp_status = 0;
    if (!myMCP.begin_I2C()) return false;

    for (uint8_t loop = 0; loop < 16; loop++) {
      if (loop < 8) {
        myMCP.pinMode(loop, INPUT_PULLUP);  //Port A: all pins are INPUT
      }else{
        myMCP.pinMode(loop, OUTPUT);        //Port B: all pins are OUTPUT
      }
    }
    myMCP.writeGPIOB ( 0b00000000 );
    myMCP.setupInterrupts( true, false, HIGH);   //mirror, open drain, polarity
    myMCP.setupInterruptPin( 9, CHANGE );   //only button for now

    return true;
}

void mcp_read () {
  if (!ModuleData[mcp_module_num].Get.ModuleDetected) return;
  mcp_status = 1;
  tcaselect(TCA_MCP);

  SensorData.mcpData.LastInt = intCapReg;
  SensorData.mcpData.PortA = myMCP.readGPIOA();
  SensorData.mcpData.PortB = myMCP.readGPIOB();
  
}

void mcp_print () {
      switch (mcp_status) {
      case 0:
        printf( "MCP: Not Detected\n" );
        mcp_status = 2;
        break;
      case 1:
        printf( "MCP: IN(A)[" BYTE_TO_BINARY_PATTERN "] OUT(B)[" BYTE_TO_BINARY_PATTERN "] Int[" BYTE_TO_BINARY_PATTERN "](%u)(%u) \n",
          BYTE_TO_BINARY(SensorData.mcpData.PortA),
          BYTE_TO_BINARY(SensorData.mcpData.PortB),
          BYTE_TO_BINARY(SensorData.mcpData.LastInt),
          IntCounter, 
          inthit );
        break;
      case 2:   //silent stop
        break;
    }
}

void mcpWritePin(uint8_t pin, uint8_t value) {
  myMCP.digitalWrite( (pin && 0x07) + 0x07, value );
}

bool mcpReadPin(uint8_t pin) {
  return myMCP.digitalRead( pin );
}

void mcpPinMode(uint8_t pin, uint8_t mode) {
  //myMCP.setPinMode( pin, B, mode );           // este nao faz muito sentido
}

void mcpTogglePin(uint8_t pin) {
  //printf("MCP: Toggle pin B%u\n", pin);
  uint8_t pinfilter;
  pinfilter = (pin && 0x07) + 0x07;
  printf("MCP: pinfilter %X, pin %X\n", pinfilter, pin);
  myMCP.digitalWrite( pinfilter, !myMCP.digitalRead(pinfilter) );
  //myMCP.togglePin( pin, B );
}

void mcp_eventcheck(){
    intCapReg = myMCP.getLastInterruptPin();

    if (mcpintevent) {
      IntCounter++;

      //printf("MCP: Int Event Ocurred %u times.\n", IntCounter);
      mcpTogglePin( MCPOUT_LED );
      mcpintevent = false;
    }
}
*/







#include <MCP23017.h>
#define MCP_ADDRESS 0x20

MCP23017 myMCP = MCP23017(MCP_ADDRESS);

uint8_t mcp_module_num;
uint8_t mcp_status;
uint16_t IntCounter;
uint8_t intCapReg;


bool mcp_boot ( uint8_t mybootnum ) {
    mcp_module_num = mybootnum;
    tcaselect(TCA_MCP);
    mcp_status = 0;
    if (!myMCP.Init()) return false;
    myMCP.setPortMode(0b00000000, A, INPUT_PULLUP);     //Port A: all pins are INPUT
    //myMCP.setPortPullUp(MCPINT_BUTTON, A);
    myMCP.setPortMode(0b11111111, B);                   //Port B: all pins are OUTPUT
    //myMCP.setAllPins (B, OFF);
    myMCP.setInterruptPinPol(LOW);
    myMCP.setIntMirror( ON );
    myMCP.setIntOdr(true);
    myMCP.setInterruptOnChangePort(0b01000000, A);  //only button for now
    return true;
}

void mcp_read () {
  if (!ModuleData[mcp_module_num].Register.ModuleDetected) return;
  mcp_status = 1;
  tcaselect(TCA_MCP);

  SensorData.mcpData.LastInt = intCapReg;
  SensorData.mcpData.PortA = myMCP.getPort(A);
  SensorData.mcpData.PortB = myMCP.getPort(B);
  
}

void mcp_print () {
      switch (mcp_status) {
      case 0:
        printf( "MCP: Not Detected\n" );
        mcp_status = 2;
        break;
      case 1:
        printf( "MCP: IN(A)[" BYTE_TO_BINARY_PATTERN "] OUT(B)[" BYTE_TO_BINARY_PATTERN "] Int[" BYTE_TO_BINARY_PATTERN "](%u)(%u) \n",
          BYTE_TO_BINARY(SensorData.mcpData.PortA),
          BYTE_TO_BINARY(SensorData.mcpData.PortB),
          BYTE_TO_BINARY(SensorData.mcpData.LastInt),
          IntCounter, inthit);
        break;
      case 2:   //silent stop
        break;
    }
}

void mcpWritePin(uint8_t pin, uint8_t value) {
  //printf("MCP: Write pin B%u - %u\n", pin, value);
  myMCP.setPin(pin, B, value);
}

bool mcpReadPin(uint8_t pin) {
  return myMCP.getPin( pin, A );
}

void mcpPinMode(uint8_t pin, uint8_t mode) {
  myMCP.setPinMode( pin, B, mode );           // este nao faz muito sentido
}

void mcpTogglePin(uint8_t pin) {
  //printf("MCP: Toggle pin B%u\n", pin);
  myMCP.togglePin( pin, B );
}

bool ledstatus;
void mcpSetALL() {
  //only toggle all for now
  //printf("MCP: setall %u\n", ledstatus);
  myMCP.setAllPins (B, ledstatus);
  ledstatus = !ledstatus;
}

void mcp_eventcheck(){
    intCapReg = myMCP.getIntCap(A);

    if (mcpintevent) {
      IntCounter++;

      //printf("MCP: Int Event Ocurred %u times.\n", IntCounter);
      mcpTogglePin( MCPOUT_LED );
    }
    mcpintevent = false;
}






//EOF
