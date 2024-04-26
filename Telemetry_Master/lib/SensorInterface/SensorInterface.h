#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GPS.h>

#define SEALEVELPRESSURE_HPA (1013.25)  // Change to match actual pressure before drop test

#define GPSSerial Serial1

extern uint8_t systemCal, gyro, accel, mag; // Calibration data

// extern Adafruit_BME280 bme; 
// extern Adafruit_BNO055 bno;
// extern Adafruit_GPS GPS();

bool initializeBNO();
bool initializeBME();
bool initializeGPS();
bool initializeSensorSuite();

// BME / IMU

float getPressure();
float getSeaLevelAlt();
float getTemperature();

// BNO / Barometer

imu::Vector<3> getAccelermometer();
imu::Vector<3> getGyroscpe();
imu::Vector<3> getGravity();
// imu::Vector<3> getEuler();
imu::Quaternion getQuaternion();

void updateCalibration(); // Must call every loop or else calibration level will stale.
int getCalibrationSys();
int getCalibrationGyro();
int getCalibrationAccel();
int getCalibrationMag();

// GPS

bool updateGPS();  // Returns true if new NMEA recieved, false if not
int* getGPSTime(int *arr); // Array: millisecond, second, minute, hour. UTC
int* getGPSDate(int *arr); // Integer array: day, month, year. UTC
bool getGPSFix();
int getGPSFixquality_3d();
float getLatitude();
float getLongitude();
float getSpeed();
float getAngle();
float getAltitude();
float getSatellites();

// FLIGHT SOFTWARE MEMORY

static float posAlgoArr[2];
static float headingDes;

void setPosAlgo(float *result);
float* getPosAlgo(float *arr);
void setHeadingDes(float headingDes);
float getHeadingDes();

#endif