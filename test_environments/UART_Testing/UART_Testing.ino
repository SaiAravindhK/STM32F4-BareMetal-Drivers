const int LED_PIN = 13;
bool is_first_request = true;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Wait until the STM32 has sent 3 bytes
  if (Serial.available() >= 3) {
    digitalWrite(LED_PIN, HIGH); 
    
    // Clear the 3 bytes from the buffer
    Serial.read();
    Serial.read();
    Serial.read();
    
    if (is_first_request) {
      // Respond to Polling Test
      byte response_1[3] = {0x99, 0x88, 0x77};
      Serial.write(response_1, 3);
      is_first_request = false;
    } else {
      // Respond to Interrupt Test
      byte response_2[3] = {0x44, 0x55, 0x66};
      Serial.write(response_2, 3);
      
      // Reset state
      is_first_request = true;
    }
    
    digitalWrite(LED_PIN, LOW); 
  }
}