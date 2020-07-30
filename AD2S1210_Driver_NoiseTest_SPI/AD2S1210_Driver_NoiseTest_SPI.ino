
#include <SPI.h>

int pinCS = 10;
int pinClock = 13;
int pinMOSI = 11;
int pinMISO = 12;
int pinSample = 9;

int clockFrequency = 5000000;
byte dataReturn0 = 0;
byte dataReturn1 = 0;
byte dataReturn2 = 0;
byte dataReturn3 = 0;
int addrPositionHigh = 0x80;
int addrPositionLow = 0x81;
int addrVelocityHigh = 0x82;
int addrVelocityLow = 0x83;
int addrFault = 0xff;
int addrFrequency = 0x91;
int garbage = 0;

long angle = 0;
long velocity = 0;

unsigned long timeEnd = 0;
unsigned long timeStart = 0;

int angleLow = 0;
int angleHigh = 0;
int velocityLow = 0;
int velocityHigh = 0;
int faults = 0;


int clockSpeed = 1; // must be even so that clockSpeed/2 can be evaluated. measured in microseconds. 2 clockspeeds per clock cycle, so a value of 2 means 4uS per cycle, or 250kHz

void setup() {
  delay(100);
  pinMode(pinCS,OUTPUT);
  pinMode(pinClock,OUTPUT);
  pinMode(pinMOSI,OUTPUT);
  pinMode(pinMISO,INPUT);
  pinMode(pinSample, OUTPUT);
  digitalWrite(pinSample, LOW);
  digitalWrite(pinCS, HIGH);
  digitalWrite(pinMOSI, LOW);
  digitalWrite(pinClock, LOW);
  delay(100);
  Serial.begin(115200);
  delay(100);
  SPI.begin();
  delay(100);
  SPI.beginTransaction(SPISettings(clockFrequency, MSBFIRST, SPI_MODE1));

  delay(100);

  //dataReturn0 = bitBangSPITransfer(addrFrequency);
  //dataReturn1 = bitBangSPITransfer(32); // 8 khz
}

void loop() {

timeStart = micros();
  for (int loopCounter = 0; loopCounter < 500; loopCounter ++){
    digitalWrite(pinSample, HIGH);
    shortDelay();
    digitalWrite(pinSample, LOW);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    garbage = SPI.transfer(addrPositionHigh);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    angleHigh = SPI.transfer(addrPositionLow);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    angleLow = SPI.transfer(addrVelocityHigh);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    velocityHigh = SPI.transfer(addrVelocityLow);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    velocityLow = SPI.transfer(addrFault);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
    digitalWrite(pinCS, LOW);
    shortDelay();
    faults = SPI.transfer(addrFault);
    shortDelay();
    digitalWrite(pinCS, HIGH);
    shortDelay();
  
    angle = angleLow + (angleHigh << 8);
    velocity = velocityLow + (velocityHigh << 8);
  }
  timeEnd = micros();
  
  //Serial.println(timeEnd-timeStart);
  Serial.println(angle);
}

void shortDelay(){
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
}
