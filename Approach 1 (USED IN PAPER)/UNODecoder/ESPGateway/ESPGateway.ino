#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <FirebaseJson.h>


#define WIFI_SSID       "Gami"
#define WIFI_PWD        "Gami@2511"
#define FIREBASE_HOST   "lifiaccesscontrol-default-rtdb.firebaseio.com"
#define FIREBASE_SECRET "rppkxQUkf2eLrWWBisVOLailEubGWBAQkjDUfuJE"

FirebaseData fb;
FirebaseJson json;

void setup() {
  Serial.begin(9600);          
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status()!=WL_CONNECTED) {
    delay(250); Serial.print('.');
  }
  Serial.println("\nWi-Fi connected");
  Firebase.begin(FIREBASE_HOST, FIREBASE_SECRET);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase ready");
}

void loop() {
  if (!Serial.available()) return;
  String line = Serial.readStringUntil('\n');
  line.trim();
  int comma = line.indexOf(',');
  if (comma < 0) return;

  String deviceId = line.substring(0, comma);
  String capBits   = line.substring(comma+1);

  // Build json payload
  json.clear();
  json.add("deviceId", deviceId);
  json.add("capBits",  capBits);
  json.add("timestamp", Firebase.getTimestamp(fb));

  // Push under the /accessRequests/{deviceId}
  String path = "/accessRequests/" + deviceId;
  if (Firebase.pushJSON(fb, path, json)) {
    Serial.println("✔️ Pushed to " + path);
  } else {
    Serial.println("❌ Push failed: " + fb.errorReason());
  }
}
