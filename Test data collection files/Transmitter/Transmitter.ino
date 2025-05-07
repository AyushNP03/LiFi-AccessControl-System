
// Sends 4-bit OOK preamble and 3 CAP pulses 6 bits payload

const int LED_PIN = 5;
const unsigned long BIT_DELAY = 100;    // ms per on/off

//this is fixed 6-bit test payload
const char PAYLOAD[]   = "101011";

// 4-bit OOK pattern
const bool PREAMBLE[4] = { 1, 0, 1, 1 };

// Brightness levels for the CAP encoding
const int LOW_BRIGHT  = 170;  // "10"
const int MED_BRIGHT  = 210;  // "11"
const int HIGH_BRIGHT = 255;  // "01"

void sendOOK(bool bit) {
  digitalWrite(LED_PIN, bit ? HIGH : LOW);
  delay(BIT_DELAY);
  digitalWrite(LED_PIN, LOW);
  delay(BIT_DELAY);
}

void sendCAP(int brightness) {
  analogWrite(LED_PIN, brightness);
  delay(BIT_DELAY);
  digitalWrite(LED_PIN, LOW);
  delay(BIT_DELAY);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
}

void loop() {
  // Log the send event in serial monitor
  unsigned long txTime = millis();
  Serial.print("SEND,");
  Serial.print(txTime);
  Serial.print(",");
  Serial.println(PAYLOAD);

  // this transmit OOK preamble
  for (int i = 0; i < 4; i++) {
    sendOOK(PREAMBLE[i]);
  }
  // Transmit 3 CAP pulses (each carries 2 bits)
  for (int i = 0; i < 6; i += 2) {
    char a = PAYLOAD[i], b = PAYLOAD[i+1];
    if      (a=='1' && b=='0') sendCAP(LOW_BRIGHT);
    else if (a=='1' && b=='1') sendCAP(MED_BRIGHT);
    else if (a=='0' && b=='1') sendCAP(HIGH_BRIGHT);
    else                        sendCAP(0);  // “00” or invalid
  }

  delay(150);  // stop for bit before next trial
}
