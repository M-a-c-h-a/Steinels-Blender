#include <Arduino.h>
#include "LoRa.h"
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <cmath>
#include <Adafruit_GPS.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME280 bme; 
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
// Adafruit_GPS gps(&GPSSerial);


int counter = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial);
  
    //LoRa Test
    Serial.println("LoRa Sender");  

    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    //GPS TEST

//    bool initializeGPS()
// 
//     if (!gps.begin(9600)) {
//         Serial.println("GPS not initialized");
//         return false;
//     }

//     gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
//     gps.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
//     gps.sendCommand(PGCMD_ANTENNA);

//     delay (1000);

//     return true;
// }

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
    

}


void loop() {

    //Grab Data
    float temperature = bme.readTemperature(); 
    float humidity = bme.readHumidity();
    // Adjust sea level as needed
    float altitude = bme.readAltitude(53);
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    //Round Data
    int roundedTemperature = round(temperature);
    int roundedHumidity = round(humidity);// lowkey fuck humidity 
    int roundedPitch = round(euler.x());
    int roundedRoll = round(euler.y());
    int roundedHeading = round(euler.z());
    int roundedAltitude = round(altitude);
  

    //Create Packet

    LoRa.beginPacket();
    LoRa.print("Temperature = ");
    LoRa.print(roundedTemperature);
    LoRa.println(" C*");

    LoRa.print("Humidity = "); //
    LoRa.print(humidity); //
    LoRa.println(" %"); //

    LoRa.print("Altitude = ");
    LoRa.print(roundedAltitude);
    LoRa.println(" m");

    LoRa.println("Orientation: ");
    LoRa.print("X: ");
    LoRa.println(euler.x()); // Pitch
    LoRa.print("Y: ");
    LoRa.println(euler.y()); // Roll
    LoRa.print("Z: "); 
    LoRa.println(euler.z()); // Heading
    
    Serial.println("Data packet sent!");
    LoRa.println();

  counter++;

  delay(5000);
}