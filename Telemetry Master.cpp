#include <Arduino.h>
#include "LoRa.h"

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  // Sending telemetry data packet
  sendTelemetry();

  // Waiting for a brief interval before sending the next packet
  delay(5000);
}

void sendTelemetry() {
  // Telemetry data to be sent
  String telemetryData = "Temperature: 25C, Humidity: 50%";

  Serial.print("Sending telemetry packet: ");
  Serial.println(telemetryData);

  // Sending telemetry packet
  LoRa.beginPacket();
  LoRa.print(telemetryData);
  LoRa.endPacket();
}
