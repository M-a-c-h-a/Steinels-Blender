#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "Timer.h" 


class DataLogger {
public:

    DataLogger(uint32_t imuPeriod, uint32_t baroPeriod, uint32_t gpsPeriod);
    

    bool initialize();
    

    void logData();
    
private:

    bool createNewDataFile();
    

    String generateFileName();
    

    void logIMUData(File& dataFile);
    void logBarometerData(File& dataFile);
    void logGPSData(File& dataFile);
    

    uint8_t chipSelect; 
    String fileName; 
    File dataFile; 
    
    
    Timer imuTimer;
    Timer baroTimer;
    Timer gpsTimer;
};

#endif
