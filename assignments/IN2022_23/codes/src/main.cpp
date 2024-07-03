//----------------------Skeleton Code--------------------//
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "1234"  // Replace with your network credentials
#define STAPSK  "12345678"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

void OTAsetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();
}

void OTAloop() {
  ArduinoOTA.handle();
}

void setup() {
  OTAsetup();

  //-------------------//
  // Custom setup code //
  //-------------------//
  pinMode(2, OUTPUT);   
  pinMode(4, OUTPUT);   
  pinMode(5, OUTPUT);   
}

void loop() {
  OTAloop();
  delay(10);  // Ensure to have a delay in loop

  //-------------------//
  // Custom loop code  //
  //-------------------//

  // NAND gate logic
  digitalWrite(2, HIGH);   // Input A
  digitalWrite(4, HIGH);   // Input B
  delay(100);              // Delay for stabilization
  digitalWrite(5, !(digitalRead(2) && digitalRead(4)));  // NAND output

  delay(100);              // Delay between operations

  digitalWrite(2, LOW);    // Input A
  digitalWrite(4, HIGH);   // Input B
  delay(100);              // Delay for stabilization
  digitalWrite(5, !(digitalRead(2) && digitalRead(4)));  // NAND output

  delay(100);              // Delay between operations

  digitalWrite(2, HIGH);   // Input A
  digitalWrite(4, LOW);    // Input B
  delay(100);              // Delay for stabilization
  digitalWrite(5, !(digitalRead(2) && digitalRead(4)));  // NAND output

  delay(100);              // Delay between operations

  digitalWrite(2, LOW);    // Input A
  digitalWrite(4, LOW);    // Input B
  delay(100);              // Delay for stabilization
  digitalWrite(5, !(digitalRead(2) && digitalRead(4)));  // NAND output

  delay(100);  // Additional delay as needed
}
