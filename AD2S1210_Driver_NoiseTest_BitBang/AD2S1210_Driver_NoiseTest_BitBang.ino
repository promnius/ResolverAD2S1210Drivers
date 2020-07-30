
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
int addrFrequency = 0x91;
int garbage = 0;
long angle = 0;

unsigned long timeEnd = 0;
unsigned long timeStart = 0;


int clockSpeed = 1; // measured in microseconds. 2 clockspeeds per clock cycle, so a value of 2 means 4uS per cycle, or 250kHz

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
  //SPI.begin();
  delay(100);
  //SPI.beginTransaction(SPISettings(clockFrequency, MSBFIRST, SPI_MODE1));

  delay(100);

  dataReturn0 = bitBangSPITransfer(addrFrequency);
  dataReturn1 = bitBangSPITransfer(32); // 8 khz
}

void loop() {


  // TO USE THIS!!!: MAKE sure spi begin and spi begintransaction are disabled!!
  timeStart = micros();
  for (int loopCounter = 0; loopCounter < 100; loopCounter ++){
    digitalWrite(pinSample, HIGH);
    delayMicroseconds(1);
    digitalWrite(pinSample, LOW);
    delayMicroseconds(1);
    garbage = bitBangSPITransfer(addrPositionHigh);
    dataReturn1 = bitBangSPITransfer(addrPositionLow);
    dataReturn0 = bitBangSPITransfer(addrPositionHigh);
  
    angle = dataReturn0 + (dataReturn1 << 8);
  }
  timeEnd = micros();
  
  //Serial.println(timeEnd-timeStart);
  Serial.println(angle);
}

byte bitBangSPITransfer(byte transferData){
  byte returnData = 0;

  digitalWrite(pinCS, LOW);
  delayMicroseconds(clockSpeed);

  for (int bitCounter = 0; bitCounter < 8; bitCounter ++){
    digitalWrite(pinClock, HIGH);
    digitalWrite(pinMOSI, transferData & (0b10000000 >> bitCounter));
    delayMicroseconds(clockSpeed);
    digitalWrite(pinClock, LOW);
    returnData = returnData | ((digitalRead(pinMISO)<<7)>>bitCounter);
    if (bitCounter != 7){ delayMicroseconds(clockSpeed);}
  }
  delayMicroseconds(clockSpeed);
  digitalWrite(pinCS, HIGH);

  return returnData;
}
