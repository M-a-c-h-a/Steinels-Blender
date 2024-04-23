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

#define GPSSerial Serial2

 Adafruit_BME280 bme; 
 Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
 Adafruit_GPS GPS(&Serial1);

DataLogger dataLogger(50, 50, 50);

void setup() {
    Serial.begin(115200);
    delay(5000);
    GPS.begin(9600);

    //LoRa Test
    Serial.println("LoRa Sender");  

    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    //GPS 
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);

    //BNO055 Initialization

    Serial.println(F("BNO055 test"));   
    if (!bno.begin()) {
        Serial.println("BNO055 failed to initialize");
        while (1);
      }

      Serial.println("BNO055 initialized");

    //BME280 Initialziation
    Serial.println(F("BME280 Test"));

    if (!bme.begin()) {
        Serial.println("BME280 failed to initialize");
        while (1);
    }

    // Initialize the DataLogger
    if (!dataLogger.initialize()) {
        Serial.println("Failed to initialize DataLogger");
        while (1);
    }
}

void loop() {
    GPS.read();

    if (GPS.newNMEAreceived())
    {
        Serial.print("new data recieved");
        GPS.parse(GPS.lastNMEA());
        Serial.print("Time: ");
        Serial.print(GPS.hour, DEC);
        Serial.print(":");
        Serial.print(GPS.minute, DEC);
        Serial.print(":");
        Serial.print(GPS.seconds, DEC);

        // Update the DataLogger with new GPS data
        updateGPS();
    }

    // Log data to the SD card
    dataLogger.logDataCSV();

    Serial.print("Location: ");
    Serial.print(GPS.latitude, 3);
    Serial.print(", ");
    Serial.println(GPS.longitude, 3);
    Serial.println(GPS.satellites);

    //Grab Data
    float temperature = bme.readTemperature(); 
    // Adjust sea level as needed
    float altitude = bme.readAltitude(53);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    // Round Data
    int roundedTemperature = round(temperature); 
    int roundedPitch = round(euler.x());
    int roundedRoll = round(euler.y());
    int roundedHeading = round(euler.z());
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