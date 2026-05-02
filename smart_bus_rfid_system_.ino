#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// ---------------- PINS ----------------
#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

// ---------------- WIFI ----------------
const char* ssid = "WIFI NAME";
const char* password = "YOUR PASSWORD";

// ---------------- FIREBASE ----------------
String firebaseURL = "https://smart-bus-monitoring-sys-1542f-default-rtdb.asia-southeast1.firebasedatabase.app/logs.json";

// ---------------- TIME (IST) ----------------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

// ---------------- STATUS ----------------
bool annaya = false;
bool sev = false;
bool sneha = false;
bool arjun = false;
bool kavya = false;
bool person6 = false;
bool deekshith = false;
bool rahul = false;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  // Sync Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Syncing time...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nTime Synced!");
  Serial.println("Scan your card...");
}

// ---------------- LOOP ----------------
void loop() {

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // ---- READ UID ----
  String uidStr = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidStr += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.print("UID: ");
  Serial.println(uidStr);

  // ---- DEFAULT VALUES ----
  String name = "Unknown";
  String status = "";
  String studentId = "UNKNOWN";

  // ---- MAPPING ----
  if (uidStr == "3cb481c") {
    annaya = !annaya;
    name = "Annaya";
    status = annaya ? "Boarded" : "Exited";
    studentId = "STU01";
  } 
  else if (uidStr == "e33c8314") {
    sev = !sev;
    name = "Sev";
    status = sev ? "Boarded" : "Exited";
    studentId = "STU02";
  } 
  else if (uidStr == "bea8fc3") {
    sneha = !sneha;
    name = "Sneha";
    status = sneha ? "Boarded" : "Exited";
    studentId = "STU03";
  } 
  else if (uidStr == "a1cffc3") {
    arjun = !arjun;
    name = "Arjun";
    status = arjun ? "Boarded" : "Exited";
    studentId = "STU04";
  } 
  else if (uidStr == "3ee3c1c") {
    kavya = !kavya;
    name = "Kavya";
    status = kavya ? "Boarded" : "Exited";
    studentId = "STU05";
  } 
  else if (uidStr == "196604") {
    person6 = !person6;
    name = "Person6";
    status = person6 ? "Boarded" : "Exited";
    studentId = "STU06";
  } 
  else if (uidStr == "343bf88e") {
    deekshith = !deekshith;
    name = "Deekshith";
    status = deekshith ? "Boarded" : "Exited";
    studentId = "STU07";
  } 
  else if (uidStr == "be24fb3") {
    rahul = !rahul;
    name = "Rahul";
    status = rahul ? "Boarded" : "Exited";
    studentId = "STU08";
  } 
  else {
    Serial.println("Unknown Card");
    return;
  }

  // ---- GET REAL TIME ----
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeString[30];
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

  String timeStamp = String(timeString);

  // ---- PRINT OUTPUT ----
  Serial.println("Student: " + name);
  Serial.println("Status: " + status);
  Serial.println("Time: " + timeStamp);

  // ---- SEND TO FIREBASE ----
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(firebaseURL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"name\":\"" + name + "\",";
    jsonData += "\"status\":\"" + status + "\",";
    jsonData += "\"studentId\":\"" + studentId + "\",";
    jsonData += "\"time\":\"" + timeStamp + "\"";
    jsonData += "}";

    int httpResponseCode = http.POST(jsonData);

    Serial.print("Firebase Response: ");
    Serial.println(httpResponseCode);

    http.end();
  }

  Serial.println("----------------------");

  // ---- STOP MULTIPLE READS ----
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(3000);
}