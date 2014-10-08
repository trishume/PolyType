/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Wire.h"
#include "USBKeyboardOutput.h"

#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01
#define GPPUA  0x0C  // GPIO pull-up resistor register
#define GPPUB  0x0D
#define GPIOA  0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB  0x13
#define OLATA  0x14  // output latch register
#define OLATB  0x15

#define KB_ROWS 6
#define KB_COLS 7
#define M_COLS KB_COLS*2
#define M_ROWS KB_ROWS
#define SLEEP_COUNT 5000

const char *header = "- PolyType -";
const byte rowPins[KB_ROWS] = {0,1,2,3,22,21};
const byte colPins[KB_COLS] = {4,5,6,7,8,9,10};
// Software Dvorak
const int matKeys[M_ROWS][M_COLS] = {
  {KEY_ESC         , KEY_ESC , KEY_2   , KEY_3   , KEY_4         , KEY_5        , KEY_TAB , KEY_TAB , KEY_6   , KEY_7          , KEY_8   , KEY_9     , KEY_MINUS   , KEY_RIGHT_BRACE},
  {KEY_RIGHT_BRACE , KEY_1   , KEY_W   , KEY_E   , KEY_R         , KEY_T        , KEY_TAB , KEY_TAB , KEY_Y   , KEY_U          , KEY_I   , KEY_O     , KEY_0       , KEY_BACKSLASH},
  {KEY_TAB         , KEY_Q   , KEY_S   , KEY_D   , KEY_F         , KEY_G        , KEY_TAB , KEY_TILDE,KEY_H   , KEY_J          , KEY_K   , KEY_L     , KEY_P       , KEY_LEFT_BRACE},
  {MODIFIERKEY_SHIFT,KEY_A   , KEY_X   , KEY_C   , KEY_V         , KEY_B        , KEY_TAB , KEY_TILDE,KEY_N   , KEY_M          , KEY_COMMA,KEY_PERIOD, KEY_SEMICOLON,KEY_QUOTE},
  {MODIFIERKEY_SHIFT,KEY_Z   , KEY_LEFT, KEY_RIGHT,MODIFIERKEY_GUI,KEY_BACKSPACE, KEY_ESC , KEY_ENTER,KEY_SPACE,MODIFIERKEY_CTRL, KEY_UP  , KEY_DOWN  , KEY_SLASH    ,MODIFIERKEY_SHIFT},
  {0, 0,MODIFIERKEY_SHIFT,0, MODIFIERKEY_ALT,KEY_MEDIA_PLAY_PAUSE, KEY_RIGHT_BRACE   , KEY_TAB , KEY_MINUS,KEY_EQUAL      , KEY_A   , 0         , MODIFIERKEY_GUI, 0},
};
// Hardware Dvorak
/*const int matKeys[M_ROWS][M_COLS] = {
  {KEY_ESC         , KEY_ESC , KEY_2   , KEY_3   , KEY_4         , KEY_5        , KEY_TAB , KEY_TAB , KEY_6   , KEY_7          , KEY_8   , KEY_9     , KEY_MINUS   , KEY_RIGHT_BRACE},
  {KEY_TILDE       , KEY_1   , KEY_W   , KEY_E   , KEY_R         , KEY_T        , KEY_TAB , KEY_TAB , KEY_Y   , KEY_U          , KEY_I   , KEY_O     , KEY_0       , KEY_QUOTE},
  {KEY_TAB         , KEY_QUOTE, KEY_O   , KEY_E   , KEY_U         , KEY_I        , KEY_TAB , KEY_TAB , KEY_D   , KEY_H          , KEY_T   , KEY_N     , KEY_S       , KEY_LEFT_BRACE},
  {MODIFIERKEY_SHIFT,KEY_A   , KEY_X   , KEY_C   , KEY_V         , KEY_B        , KEY_TAB , KEY_TAB , KEY_N   , KEY_M          , KEY_COMMA,KEY_PERIOD, KEY_SEMICOLON,KEY_BACKSLASH},
  {MODIFIERKEY_SHIFT,KEY_Z   , KEY_LEFT, KEY_RIGHT,MODIFIERKEY_GUI,KEY_BACKSPACE, KEY_ESC , KEY_ENTER,KEY_SPACE,MODIFIERKEY_CTRL, KEY_UP  , KEY_DOWN  , KEY_SLASH    ,MODIFIERKEY_SHIFT},
  {0, 0,MODIFIERKEY_SHIFT,0, MODIFIERKEY_ALT,KEY_MEDIA_PLAY_PAUSE, KEY_RIGHT_BRACE   , KEY_TAB , KEY_MINUS,KEY_EQUAL      , KEY_A   , 0         , MODIFIERKEY_GUI, 0},
};*/

int matrix[M_ROWS][M_COLS];
int sleepCounter;
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 16, 15);
USBKeyboardOutput usbOut = USBKeyboardOutput();

void debug(const char *str) {
  Serial.print(str);
  display.print(str);
  display.display();
}

void printScreenHeader() {
  display.clearDisplay();
  display.println(header);
  display.println("Version 0.0.2");
  display.println("Codename: Tarp");
  display.display();
}

void kick() {
  if(sleepCounter == 0) {
    printScreenHeader();
  }
  sleepCounter = SLEEP_COUNT;
}

// the setup routine runs once when you press reset:
byte read8 (byte mcpregister) {  // read 8 bits from MCP register;
  byte controlbyte  = B0100000;  // note that bit 0 not used b/c 7-bit opcode for Wire library
  byte tempdata;

  Wire.beginTransmission(controlbyte);
  Wire.send(mcpregister);
  Wire.endTransmission();

  Wire.requestFrom(controlbyte,(byte)1);  // otherwise function gets one byte and one int & chokes
  while (Wire.available() < 1) { }
  tempdata = Wire.receive();      // get lower 8 bytes
  return tempdata;
}

void store8 (byte mcpregister, byte mcpdata)    // Store 8 bits in specified register of MCP23017
{
  byte controlbyte  = B0100000;  // note that bit 0 not used b/c 7-bit opcode for Wire library

  Wire.beginTransmission(controlbyte);
  Wire.send(mcpregister);
  Wire.send(mcpdata);
  Wire.endTransmission();
}

void setupMCP() {
  debug("I/O chip: ");
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
  debug("up\n");
}

void setupMatrix() {
  debug("Matrix: ");
  for(int i=0;i<KB_ROWS;i++)
    pinMode(rowPins[i],OUTPUT);
  for(int i=0;i<KB_COLS;i++)
    pinMode(colPins[i],INPUT_PULLUP);
  debug("up\n");
}

void setupDisplay() {
  Serial.println("Setting up display");
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
}

void updateMCP() {
  for(int r=0; r<KB_ROWS; r++) {
    store8(GPIOB, 0xFF & ~(1<<r));
    byte in = read8(GPIOA);
    for(int c=0; c<KB_COLS; c++) {
      int val = !(in & (1<<(c+1)));
      int matC = c + KB_COLS;
      if(matrix[r][matC] != val) {
        matrix[r][matC] = val;
        keyEvent(r, matC);
      }
    }
  }
}

void updateMatrix() {
  pinMode(10,INPUT_PULLUP);
  for(int r=0; r<KB_ROWS; r++) {
    for(int i=0;i<KB_ROWS;i++) {
      pinMode(rowPins[i],INPUT);
    }
    pinMode(rowPins[r],OUTPUT);
    digitalWrite(rowPins[r],LOW);
    delay(2);
    for(int c=0; c<KB_COLS; c++) {
      int val = !digitalRead(colPins[c]);
      int matC = c;
      if(matrix[r][matC] != val) {
        matrix[r][matC] = val;
        keyEvent(r, matC);
      }
    }
  }
}

void keyEvent(int r, int c) {
  int code = matKeys[r][c];
  KeyEventType type = matrix[r][c] ? KeyDown : KeyUp;
  KeyCodeEvent event(code, type);
  usbOut.push(event);
  // sendKeys();
  kick();
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  setupDisplay();
  printScreenHeader();
  Wire.begin(); // wake up I2C bus

  for(int i=0; i<M_ROWS; i++){
    for(int j=0; j<M_COLS; j++) {
      matrix[i][j] = 0;
    }
  }

  setupMCP();
  setupMatrix();

  sleepCounter = SLEEP_COUNT;
}

// the loop routine runs over and over again forever:
void loop() {
  //Serial.println("updating");
  updateMatrix();
  updateMCP();
  usbOut.commit();

  if(sleepCounter == 1) {
    Serial.println("sleep");
    display.clearDisplay();
    display.display();
  }
  if(sleepCounter > 0) sleepCounter--;

  delay(10);        // delay in between reads for stability
}
