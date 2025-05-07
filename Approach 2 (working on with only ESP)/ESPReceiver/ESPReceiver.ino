// Reads the LDR and decodes 6 CAP bits and then pushes { deviceId, capBits, timestamp } to Firebase

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <FirebaseJson.h>         


#define WIFI_SSID       "Gami"
#define WIFI_PWD        "Gami@2511"


#define FIREBASE_HOST   "lifiaccesscontrol-default-rtdb.firebaseio.com"
#define FIREBASE_SECRET "rppkxQUkf2eLrWWBisVOLailEubGWBAQkjDUfuJE"

// unique ID for reader
#define DEVICE_ID       "device_001"


FirebaseData    fb;
FirebaseJson    json;

// LDR and FSM parameters
const int  LDR_PIN       = A0;
const int  OOK_THRESH    = 600;
const int  CAP_THRESH_H  = 700;
const int  CAP_THRESH_M  = 500;
const unsigned long SAMPLE_INTERVAL = 100;

enum State { FIND_PREAMBLE, READ_CAP };
State state = FIND_PREAMBLE;
int  preIdx  = 0;
char capBits[7];
int  bitIdx  = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  // for Firebase 
  Firebase.begin(FIREBASE_HOST, FIREBASE_SECRET);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase ready");
}

void loop() {
  delay(SAMPLE_INTERVAL);
  int val = analogRead(LDR_PIN);

  if (state == FIND_PREAMBLE) {
    bool bit = val > OOK_THRESH;
    bool expect = (preIdx==0||preIdx==2||preIdx==3);  // pattern 1,0,1,1
    if (bit == expect) {
      preIdx++;
      if (preIdx == 4) {
        state = READ_CAP;
        bitIdx = 0;
      }
    } else {
      preIdx = 0;
    }
  }
  else if (state == READ_CAP) {
    // mapping brightness with two bits
    char b1,b2;
    if      (val > CAP_THRESH_H) { b1='0'; b2='1'; }  // 255→01
    else if (val > CAP_THRESH_M) { b1='1'; b2='1'; }  // 210→11
    else                         { b1='1'; b2='0'; }  // 170→10

    capBits[bitIdx++] = b1;
    capBits[bitIdx++] = b2;

    if (bitIdx >= 6) {
      capBits[6] = '\0';
      Serial.printf("Decoded CAP bits: %s\n", capBits);

      // craeate json 
      json.clear();
      json.add("deviceId", DEVICE_ID);
      json.add("capBits",  capBits);
      // server timestamp:
      json.add("timestamp", Firebase.getTimestamp(fb));

      // Push json to db
      String path = String("/accessRequests/") + DEVICE_ID;
      if (Firebase.pushJSON(fb, path, json)) {
        Serial.println("✔️  Pushed JSON to " + path);
      } else {
        Serial.printf("❌  Push failed: %s\n", fb.errorReason().c_str());
      }

      // reset FSM
      state = FIND_PREAMBLE;
      preIdx = 0;
    }
  }
}
