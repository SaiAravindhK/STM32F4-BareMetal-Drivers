#include <Wire.h>

#define SLAVE_ADDR 0x68
volatile bool dataRequested = false;

void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
  Serial.println("Arduino I2C Slave Ready.");
}

void loop() {
  if(dataRequested) {
    Serial.println("STM32 requested data. Sent: BB 66");
    dataRequested = false;
  }
  delay(10);
}

void receiveEvent(int bytesReceived) {
  Serial.print("STM32 Sent: ");
  while (Wire.available()) {
    byte c = Wire.read();
    if (c < 0x10) Serial.print("0");
    Serial.print(c, HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void requestEvent() {
  // Sending 2 bytes back
  byte response[2] = {0xBB, 0x66};
  Wire.write(response, 2);
  dataRequested = true; 
}