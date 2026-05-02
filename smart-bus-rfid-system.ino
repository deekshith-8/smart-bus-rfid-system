#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "RAMITHzx";
const char* password = "12345678";

String firebaseURL = "https://smart-bus-monitoring-sys-1542f-default-rtdb.asia-southeast1.firebasedatabase.app/";

const char* ntpServer = "time.google.com";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  SPI.begin();
  rfid.PCD_Init();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(3000);

  time_t now = time(nullptr);
  while (now < 100000) {
    Serial.println("Retrying time...");
    delay(2000);
    now = time(nullptr);
  }

  Serial.println("Time synced");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.println(uid);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    String url = firebaseURL + "rfid/" + uid + ".json";
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    String data = "{\"status\":\"present\"}";

    int httpResponseCode = http.PUT(data);

    Serial.print("Firebase Response: ");
    Serial.println(httpResponseCode);

    http.end();
  }

  delay(2000);
}