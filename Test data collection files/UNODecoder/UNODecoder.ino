// Reads the LDR, detects the 4-bit OOK preamble, decodes 6 CAP bits,
// logs "RECV" and at the end of 50 runs it prints a summury to collect data.

const int LDR_PIN       = A0;
const int OOK_THRESH    = 600;
const int CAP_THRESH_H  = 700;
const int CAP_THRESH_M  = 500;
const unsigned long SAMPLE_INTERVAL = 100; // ms between samples

enum State { FIND_PREAMBLE, READ_CAP };
State state = FIND_PREAMBLE;

// preamble to match 1,0,1,1
const bool PREAMBLE[4] = { 1, 0, 1, 1 };
int preIdx = 0;

// for 6 decoded bits + null terminator
char capBits[7];
int bitIdx = 0;

// Timing for sampling
unsigned long lastSample = 0;

// Test data counters to store resulrts
int trials = 0;
int TP = 0;
int FP = 0;
const char EXPECTED[] = "101011";

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  unsigned long now = millis();
  if (now - lastSample < SAMPLE_INTERVAL) return;
  lastSample = now;

  int val = analogRead(LDR_PIN);
  bool high = val > OOK_THRESH;

  if (state == FIND_PREAMBLE) {
    // detecting the 4-bit OOK sync
    if (high == PREAMBLE[preIdx]) {
      preIdx++;
      if (preIdx >= 4) {
        state  = READ_CAP;
        bitIdx = 0;
      }
    } else {
      preIdx = 0;
    }
  }
  else if (state == READ_CAP) {
    // Mapping the one CAP sample with resapective two bits for that brightness level
    char b1, b2;
    if      (val > CAP_THRESH_H) { b1 = '0'; b2 = '1'; }  // full bright
    else if (val > CAP_THRESH_M) { b1 = '1'; b2 = '1'; }  // medium
    else                         { b1 = '1'; b2 = '0'; }  // low

    capBits[bitIdx++] = b1;
    capBits[bitIdx++] = b2;

    if (bitIdx >= 6) {
      capBits[6] = '\0';

      // Log receive event
      unsigned long rxTime = now;
      Serial.print("RECV,");
      Serial.print(rxTime);
      Serial.print(",");
      Serial.println(capBits);

      // Check match and increment the counters
      trials++;
      if (strcmp(capBits, EXPECTED) == 0) {
        TP++;
        Serial.println("pattern matched");
      } else {
        FP++;
        Serial.println("no pattern match");
      }

      // After 50 trials, print summary and halt
      if (trials >= 50) {
        Serial.print("SUMMARY,Trials=");
        Serial.print(trials);
        Serial.print(",TP=");
        Serial.print(TP);
        Serial.print(",FP=");
        Serial.println(FP);
        while (true);  // stop
      }

      // reset FSM
      state  = FIND_PREAMBLE;
      preIdx = 0;
    }
  }
}
