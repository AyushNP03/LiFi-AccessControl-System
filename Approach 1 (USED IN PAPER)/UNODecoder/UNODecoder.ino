
#include <SoftwareSerial.h>       // for UNO and ESP8266 link

const int LDR_PIN = A0;            
const int RX_PIN  = 2;             
const int TX_PIN  = 3;             
SoftwareSerial espSerial(RX_PIN, TX_PIN);

const int OOK_THRESH   = 600;    
const int CAP_THRESH_H = 700;
const int CAP_THRESH_M = 500;
const unsigned long SAMPLE_INTERVAL = 100;  // ms

enum State { FIND_PREAMBLE, READ_CAP };
State state = FIND_PREAMBLE;
int preIdx = 0, bitIdx = 0;
char capBits[7];

// Unique ID for 
const char* DEVICE_ID = "device_001";

void setup() {
  Serial.begin(9600);                      
  espSerial.begin(9600);                   
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  delay(SAMPLE_INTERVAL);
  int val = analogRead(LDR_PIN);

  // ook Preamble (1,0,1,1)
  if (state == FIND_PREAMBLE) {
    bool bit = val > OOK_THRESH;
    bool expect = (preIdx==0||preIdx==2||preIdx==3);
    if (bit==expect) {
      preIdx++;
      if (preIdx>=4) {
        state = READ_CAP;
        bitIdx = 0;
      }
    } else {
      preIdx = 0;
    }
  }
  // Read 3 CAP 
  else if (state == READ_CAP) {
    char b1,b2;
    if      (val > CAP_THRESH_H) { b1='0'; b2='1'; }  // full bright
    else if (val > CAP_THRESH_M) { b1='1'; b2='1'; }  // medium
    else                         { b1='1'; b2='0'; }  // low

    capBits[bitIdx++] = b1;
    capBits[bitIdx++] = b2;

    if (bitIdx >= 6) {
      capBits[6] = '\0';
      Serial.print("Decoded CAP: "); Serial.println(capBits);
      // Send to ESP 
      espSerial.print(DEVICE_ID);
      espSerial.print(',');
      espSerial.println(capBits);
      // reset
      state = FIND_PREAMBLE;
      preIdx = 0;
    }
  }
}
