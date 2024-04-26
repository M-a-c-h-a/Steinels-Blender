#include "SensorInterface.h"

static Adafruit_BME280 bme; // I2C initialization for BME
extern Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28); // I2C initialization for BNO
static Adafruit_GPS gps(&GPSSerial); // Serial initialization for GPS

// If you modify the variables here, the other copy of the variables located in main.cpp WILL NOT CHANGE ACCORDINGLY! Go replicate changes everywhere.

uint8_t systemCal, gyro, accel, mag = 0;

bool initializeBME()
{
    if (!bme.begin()) {
        Serial.println("BME280 not detected");
        return false;
    }
  return true;
}

bool initializeBNO()
{
    if (!bno.begin()) {
        Serial.println("BNO055 not detected");
        return false;
    }

  bno.setMode(OPERATION_MODE_NDOF);

  return true;
}

bool initializeGPS()
{
    if (!gps.begin(9600)) {
        Serial.println("GPS not initialized");
        return false;
    }

    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
    gps.sendCommand(PGCMD_ANTENNA);

    delay (1000);

    return true;
}

bool initializeSensorSuite() {
  if (!initializeGPS() || !initializeBNO() || !initializeBME()) {
    return false;
  }

  Serial.println("Sensors Initialized!");

  return true;
}

// SENSOR DATA GETTERS:

float getPressure() { return bme.readPressure() / 100.0F; }
float getSeaLevelAlt() {return bme.readAltitude(SEALEVELPRESSURE_HPA); }
float getTemperature() { return bme.readTemperature(); }


imu::Vector<3> getAccelermometer() { return bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER); }
imu::Vector<3> getGyroscpe() { return bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); }
imu::Vector<3> getGravity() { return bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY); }
// imu::Vector<3> getEuler() { return bno.getVector(Adafruit_BNO055::VECTOR_EULER); }
imu::Quaternion getQuaternion() { return bno.getQuat(); }

void updateCalibration() {bno.getCalibration(&systemCal, &gyro, &accel, &mag); }
int getCalibrationSys() { return systemCal; }
int getCalibrationGyro() { return gyro; }
int getCalibrationAccel() { return accel; }
int getCalibrationMag() { return mag; }


bool updateGPS() // Must call every loop or else GPS does not yield updated values
{

  gps.read();

  if (gps.newNMEAreceived())
  {
      gps.parse(gps.lastNMEA()); // No need to parse duplicate GPS data
      return true;
  }
  return false;
  
}

int* getGPSTime(int *timeArr) {
    // Format: millisecond, second, minute, hour.

    timeArr[0] = gps.milliseconds;
    timeArr[1] = gps.seconds;
    timeArr[2] = gps.minute;
    timeArr[3] = gps.hour;

    return timeArr;

}

int* getGPSDate(int *dateArr) {
    //Format: day, month, year.

    dateArr[0] = gps.day;
    dateArr[1] = gps.month;
    dateArr[2] = gps.year;

    return dateArr;

}

float getLatitude() // Different from old board, now instead of appended letter, it changes sign.
{
  float latitude_ddeg = round(gps.latitude / 100);
  latitude_ddeg += fmod(gps.latitude, 100) / 60;
  return ((gps.lat == 'N') ? latitude_ddeg : -latitude_ddeg);
}

float getLongitude()
{
  float longitude_ddeg = round(gps.longitude / 100);
  longitude_ddeg += fmod(gps.longitude, 100) / 60;
  return ((gps.lon == 'E') ? longitude_ddeg : -longitude_ddeg);
}

bool getGPSFix() { return gps.fix; }
int getGPSFixquality_3d() { return gps.fixquality_3d; }
float getSpeed() { return gps.speed; }
float getAngle() { return gps.angle; }
float getAltitude() { return gps.altitude; }
float getSatellites() { return gps.satellites; }


// FLIGHT SOFTWARE MEMORY

void setPosAlgo(float *result) {
  posAlgoArr[0] = result[0];
  posAlgoArr[1] = result[1];
}

float* getPosAlgo(float *arr) {
  arr[0] = posAlgoArr[0];
  arr[1] = posAlgoArr[1];

  return arr;
}

void setHeadingDes(float desHeading) {
  headingDes = desHeading;
}

float getHeadingDes() {
  return headingDes;
}