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
Adafruit_GPS GPS;


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

    GPS.begin(9600);

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
    Serial.print("Temperature = ");
    Serial.print(roundedTemperature);
    Serial.println(" C*");

    Serial.print("Humidity = "); //
    Serial.print(humidity); //
    Serial.println(" %"); //

    Serial.print("Altitude = ");
    Serial.print(roundedAltitude);
    Serial.println(" m");

    Serial.println("Orientation: ");
    Serial.print("X: ");
    Serial.println(euler.x()); // Pitch
    Serial.print("Y: ");
    Serial.println(euler.y()); // Roll
    Serial.print("Z: "); 
    Serial.println(euler.z()); // Heading
    
    Serial.println("Data packet sent!");
    Serial.println();

  counter++;

  delay(5000);
}