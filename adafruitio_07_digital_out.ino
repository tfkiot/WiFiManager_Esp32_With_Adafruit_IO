#include "config.h"

#define LED_PIN 2

const int flashButtonPin = 0;

AdafruitIO_Feed *feed1 = io.feed("feed1");

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(flashButtonPin, INPUT);
  WiFiManager wifiMn;

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

 if (!wifiMn.autoConnect("MyESP32AP")) {
    Serial.println("Failed to connect and hit timeout");
    esp_restart();
    delay(1000);
  }
  Serial.println("Connected to Wi-Fi!");

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  feed1->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  feed1->get();
}

void loop() {
  if (digitalRead(flashButtonPin) == LOW) {
    WiFiManager wifiMn;
    wifiMn.resetSettings();
    esp_restart();
    Serial.println("WiFi settings reset");
    delay(1000);
  }
  io.run();
}


void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  digitalWrite(LED_PIN, data->toPinLevel());
}
