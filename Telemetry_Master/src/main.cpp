#include <Arduino.h>
#include "LoRa.h"
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <cmath>
#include <Adafruit_GPS.h>
#include "DataLogger.h"
#include "SensorInterface.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define GPSSerial Serial1

DataLogger dataLogger(50, 50, 50);

void setup() {
    Serial.begin(115200);
    delay(5000);

    //LoRa Test
    Serial.println("LoRa Sender");  

    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    if (!initializeSensorSuite()) {
        Serial.println("Failed to initialize sensors :(");
        while (1);
    }

    // Initialize the DataLogger
    if (!dataLogger.initialize()) {
        Serial.println("Failed to initialize DataLogger");
        while (1);
    }
}

void loop() {


    // Log data to the SD card
    dataLogger.logDataCSV();


// Proper calls to record data.

    initializeBME();
    initializeBNO();



    //Grab Data
    float temperature = getTemperature(); 
    // Adjust sea level as needed
    float altitude = getAltitude();

    //float euler = getEuler();

    // Round Data
    int roundedTemperature = round(temperature); 
    // int roundedPitch = round(getEuler().x());
    // int roundedRoll = round(getEuler().y());
    // int roundedHeading = round(getEuler().z());
    int roundedAltitude = round(altitude);

    // Create Packet
    LoRa.beginPacket();
    LoRa.print("Temperature = ");
    LoRa.print(roundedTemperature);
    LoRa.println(" C*");

    LoRa.print("Altitude = ");
    LoRa.print(roundedAltitude);
    LoRa.println(" m");

    LoRa.println("Orientation: ");
    LoRa.print("X: ");LoRa.println(roundedPitch); // Pitch
    LoRa.print("Y: ");
    LoRa.println(roundedRoll); // Roll
    LoRa.print("Z: "); 
    LoRa.println(roundedHeading); // Heading

    Serial.println("Data packet sent!");
    LoRa.println();
    LoRa.endPacket();

    // counter++;

    delay(50);
}