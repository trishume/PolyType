#include "MCPMatrixSource.h"
#include "Arduino.h"
#include <i2c_t3.h>

#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01
#define GPPUA  0x0C  // GPIO pull-up resistor register
#define GPPUB  0x0D
#define GPIOA  0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB  0x13
#define OLATA  0x14  // output latch register
#define OLATB  0x15

// the setup routine runs once when you press reset:
static byte read8 (byte mcpregister) {  // read 8 bits from MCP register;
  byte controlbyte  = B0100000;  // note that bit 0 not used b/c 7-bit opcode for Wire library
  byte tempdata;

  Wire.beginTransmission(controlbyte);
  Wire.write(mcpregister);
  Wire.endTransmission();

  Wire.requestFrom(controlbyte,(byte)1);  // otherwise function gets one byte and one int & chokes
  while (Wire.available() < 1) { }
  tempdata = Wire.read();      // get lower 8 bytes
  return tempdata;
}

static void store8 (byte mcpregister, byte mcpdata)    // Store 8 bits in specified register of MCP23017
{
  byte controlbyte  = B0100000;  // note that bit 0 not used b/c 7-bit opcode for Wire library

  Wire.beginTransmission(controlbyte);
  Wire.write(mcpregister);
  Wire.write(mcpdata);
  Wire.endTransmission();
}

void MCPMatrixSource::start() {
  // IO A is columns/inputs
  store8(IODIRA, 0xFF);
  // IO B is rows/outputs
  store8(IODIRB, 0b11000000);
  // pull ups
  store8(GPPUA, 0xFF);
  store8(GPPUB, 0b11000000);
  // output latches
  store8(OLATA, 0xFF);
  store8(OLATB, 0xFF);
}

void MCPMatrixSource::update() {
  // TODO maybe reset once in N scans, like every second
  // start();

  startingScan();
  for(int r=0; r<MAT_ROWS; r++) {
    store8(GPIOB, 0xFF & ~(1<<r));
    byte in = read8(GPIOA);
    for(int c=0; c<MAT_COLS; c++) {
      int val = !(in & (1<<(c+1)));
      setState(r,c,val);
    }
  }
}
