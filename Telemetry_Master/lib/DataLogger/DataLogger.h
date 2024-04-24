#ifndef DataLogger_h
#define DataLogger_h

#include <Arduino.h>
#include <Timer.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_GPS.h>
#include <SensorInterface.h>

const int chipSelect = 7;
// uint8_t systemCal, gyro, accel, mag = 0;
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

    uint8_t getSatellites;

private:
    Timer *timerBNO;
    Timer *timerBME;
    Timer *timerGPS;
    File dataFile;
    String fileName;
};

#endif