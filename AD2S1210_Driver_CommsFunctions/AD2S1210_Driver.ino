
#include <SPI.h>

int pinCS = 10;
int pinClock = 13;
int pinMOSI = 11;
int pinMISO = 12;

int clockFrequency = 5000000;
byte dataReturn0 = 0;
byte dataReturn1 = 0;
byte dataReturn2 = 0;
byte dataReturn3 = 0;
int addrPosition = 0;
int addrFrequency = 0x91;
int clockSpeed = 2; // must be even so that clockSpeed/2 can be evaluated. measured in microseconds. 2 clockspeeds per clock cycle, so a value of 2 means 4uS per cycle, or 250kHz

void setup() {
  delay(100);
  pinMode(pinCS,OUTPUT);
  pinMode(pinClock,OUTPUT);
  pinMode(pinMOSI,OUTPUT);
  pinMode(pinMISO,INPUT);
  digitalWrite(pinCS, HIGH);
  digitalWrite(pinMOSI, LOW);
  digitalWrite(pinClock, LOW);
  delay(100);
  Serial.begin(9600);
  delay(100);
  SPI.begin();
  delay(100);
  SPI.beginTransaction(SPISettings(clockFrequency, MSBFIRST, SPI_MODE1));

  delay(100);
}

void loop() {
  /*
  dataReturn0 = bitBangSPITransfer(addrFrequency);
  delayMicroseconds(10);
  dataReturn1 = bitBangSPITransfer(57);
  Serial.print(dataReturn0); Serial.print(","); Serial.println(dataReturn1);
  delay(1000);*/


// TO USE THIS!!!: MAKE sure spi begin and spi begintransaction are disabled!!
/*
 * // sweep frequency settings, bitbang
  for (int dataCounter = 0; dataCounter < 128; dataCounter ++){
    dataReturn0 = bitBangSPITransfer(addrFrequency);
    delayMicroseconds(5);
    dataReturn1 = bitBangSPITransfer(dataCounter);
    Serial.print(dataReturn0); Serial.print(","); Serial.println(dataReturn1);
    delay(5000);
  }
  */
  


  for (int dataCounter = 0; dataCounter < 128; dataCounter ++){
    digitalWrite(pinCS, LOW);
    delayMicroseconds(5);
    dataReturn0 = SPI.transfer(addrFrequency);
    delayMicroseconds(5);
    digitalWrite(pinCS, HIGH);
    
    delayMicroseconds(5);
  
    digitalWrite(pinCS, LOW);
    delayMicroseconds(5);
    dataReturn1 = SPI.transfer(dataCounter);
    delayMicroseconds(5);
    digitalWrite(pinCS, HIGH);

    Serial.print(dataReturn0); Serial.print(","); Serial.println(dataReturn1);
    delay(500);
  }
  
  
  /*
}
  for (int dataCounter = 0; dataCounter < 128; dataCounter ++){
    for (int cycleCounter = 0; cycleCounter < 20; cycleCounter++){
        digitalWrite(pinCS, LOW);
        delayMicroseconds(1);
        dataReturn0 = SPI.transfer(addrFrequency);
        delayMicroseconds(1);
        digitalWrite(pinCS, HIGH);
        
        delayMicroseconds(10);
      
        digitalWrite(pinCS, LOW);
        delayMicroseconds(1);
        dataReturn1 = SPI.transfer(dataCounter);
        delayMicroseconds(1);
        digitalWrite(pinCS, HIGH);

        delayMicroseconds(10);

        Serial.print("Test "); Serial.print(dataCounter); Serial.print(": "); Serial.print(dataReturn0); Serial.print(","); Serial.print(dataReturn1);
        
        for (int readCounter = 0; readCounter < 20; readCounter ++){
          digitalWrite(pinCS, LOW);
          delayMicroseconds(1);
          dataReturn2 = SPI.transfer(addrFrequency);
          delayMicroseconds(1);
          digitalWrite(pinCS, HIGH);

          delayMicroseconds(10);
          Serial.print(","); Serial.print(dataReturn2);
        }

        Serial.println();
         //Serial.print(","); Serial.println(dataReturn3);
        delay(500);
    }
  }
  */
}

byte bitBangSPITransfer(byte transferData){
  byte returnData = 0;
  /*
  for(int clockCounter = 0; clockCounter < 10; clockCounter++){
    delayMicroseconds(clockSpeed);
    digitalWrite(pinClock, HIGH);
    delayMicroseconds(clockSpeed);
    digitalWrite(pinClock, LOW);
  }
  */
  
  delayMicroseconds(clockSpeed/2);
  digitalWrite(pinCS, LOW);
  delayMicroseconds(clockSpeed/2);
  delayMicroseconds(5);

  for (int bitCounter = 0; bitCounter < 8; bitCounter ++){
    digitalWrite(pinClock, HIGH);
    digitalWrite(pinMOSI, transferData & (0b10000000 >> bitCounter));
    delayMicroseconds(clockSpeed);
    digitalWrite(pinClock, LOW);
    returnData = returnData | ((digitalRead(pinMISO)<<7)>>bitCounter);
    if (bitCounter != 7){ delayMicroseconds(clockSpeed);}
  }
  delayMicroseconds(5);
  delayMicroseconds(clockSpeed/2);
  digitalWrite(pinCS, HIGH);
  delayMicroseconds(clockSpeed/2);

  /*
  for(int clockCounter = 0; clockCounter < 10; clockCounter++){
    digitalWrite(pinClock, HIGH);
    delayMicroseconds(clockSpeed);
    digitalWrite(pinClock, LOW);
    delayMicroseconds(clockSpeed);
  }*/
  return returnData;
}
