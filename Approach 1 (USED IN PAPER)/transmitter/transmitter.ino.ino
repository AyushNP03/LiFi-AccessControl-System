// Transmit 4 OOK bits and 3 CAP pulses 

const int LED_PIN       = 5;
const unsigned long BIT_TIME = 100;  // ms per on/off

bool ookPattern[4] = { 1, 0, 1, 1 };

uint8_t capLevels[3] = { 170, 210, 255 };

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Send 4 OOK bits
  for (int i = 0; i < 4; i++) {
    if (ookPattern[i]) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
    delay(BIT_TIME);
    digitalWrite(LED_PIN, LOW);
    delay(BIT_TIME);
  }

  // Send 3 CAP 
  for (int i = 0; i < 3; i++) {
    analogWrite(LED_PIN, capLevels[i]);
    delay(BIT_TIME);
    digitalWrite(LED_PIN, LOW);
    delay(BIT_TIME);
  }

  // pause before repeating
  delay(150);
}
