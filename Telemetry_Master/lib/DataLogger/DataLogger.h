#ifndef DataLogger_h
#define DataLogger_h

#include <Timer.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>

class DataLogger
{
public:
    DataLogger(uint32_t bnoPeriod, uint32_t bmePeriod, uint32_t gpsPeriod);
    bool initialize();
    bool newDataFile();
    void addDataHeaders(File dataFile);
    String getFileName();
    void logDataCSV();
    void logBME(File dataFile);
    void logBMESpacer(File dataFile);
    void logBNO(File dataFile);
    void logBNOSpacer(File dataFile);
    void logAlgo(File dataFile);
    void logAlgoSpacer(File dataFile);
    void logGPS(File dataFile);
    void logGPSSpacer(File dataFile);

    float getPressure();
    float getTemperature();
    float getSeaLevelAlt();
    imu::Quaternion getQuaternion();
    imu::Vector<3> getGyroscpe();
    imu::Vector<3> getAccelermometer();
    imu::Vector<3> getGravity();
    void updateCalibration();
    float *getPosAlgo(float posTemp[2]);
    int *getGPSTime(int timeArr[4]);
    int *getGPSDate(int dateArr[3]);
    bool updateGPS();
    uint8_t getGPSFix();
    uint8_t getGPSFixquality_3d();
    float getLongitude();
    float getLatitude();
    float getAltitude();
    float getSpeed();
    float getAngle();
    uint8_t getSatellites;

private:
    Timer *timerBNO;
    Timer *timerBME;
    Timer *timerGPS;
    File dataFile;
    String fileName;
};

#endif