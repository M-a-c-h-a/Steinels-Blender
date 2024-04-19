#include "DataLogger.h"


DataLogger::DataLogger(uint32_t imuPeriod, uint32_t baroPeriod, uint32_t gpsPeriod)
    : imuTimer(imuPeriod), baroTimer(baroPeriod), gpsTimer(gpsPeriod) {
}


bool DataLogger::initialize() {
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed.");
        return false;
    }

    if (!createNewDataFile()) {
        Serial.println("Failed to create a new data file.");
        return false;
    }

    Serial.println("SD card and data file initialized successfully.");
    return true;
}


bool DataLogger::createNewDataFile() {

    fileName = generateFileName();


    dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (!dataFile) {
        Serial.println("Failed to open data file for writing.");
        return false;
    }

    dataFile.close();
    return true;
}


String DataLogger::generateFileName() {

    String baseName = "telemetry_";
    int fileIndex = 0;


    while (SD.exists((baseName + fileIndex + ".csv").c_str())) {
        fileIndex++;
    }


    return baseName + fileIndex + ".csv";
}


void DataLogger::logData() {

    dataFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (!dataFile) {
        Serial.println("Failed to open data file for writing.");
        return;
    }


    unsigned long timestamp = millis();


    dataFile.print(timestamp);
    dataFile.print(",");


    if (imuTimer.check()) {
        imuTimer.reset();
        logIMUData(dataFile);
    }

    if (baroTimer.check()) {
        baroTimer.reset();
        logBarometerData(dataFile);
    }

    if (gpsTimer.check()) {
        gpsTimer.reset();
        logGPSData(dataFile);
    }

    dataFile.println();

    dataFile.close();
}

/*
void DataLogger::logIMUData(File& dataFile) {

    imu::Quaternion quat = getQuaternion();
    imu::Vector<3> gyro = getGyroscope();
    imu::Vector<3> accel = getAccelerometer();
    imu::Vector<3> grav = getGravity();


    dataFile.print(quat.w(), 4);
    dataFile.print(",");
    dataFile.print(quat.x(), 4);
    dataFile.print(",");
    dataFile.print(quat.y(), 4);
    dataFile.print(",");
    dataFile.print(quat.z(), 4);
    dataFile.print(",");
    dataFile.print(gyro.x());
    dataFile.print(",");
    dataFile.print(gyro.y());
    dataFile.print(",");
    dataFile.print(gyro.z());
    dataFile.print(",");
    dataFile.print(accel.x());
    dataFile.print(",");
    dataFile.print(accel.y());
    dataFile.print(",");
    dataFile.print(accel.z());
    dataFile.print(",");
    dataFile.print(grav.x());
    dataFile.print(",");
    dataFile.print(grav.y());
    dataFile.print(",");
    dataFile.print(grav.z());
    dataFile.print(",");
}
*/

void DataLogger::logBarometerData(File& dataFile) {

    float temperature = getTemperature();
    float pressure = getPressure();
    float altitude = getSeaLevelAlt();

    dataFile.print(temperature, 2);
    dataFile.print(",");
    dataFile.print(pressure, 2);
    dataFile.print(",");
    dataFile.print(altitude, 2);
    dataFile.print(",");
}

// Log data from the GPS to the data file
void DataLogger::logGPSData(File& dataFile) {
    // Collect GPS data (latitude, longitude, altitude, speed, heading)
    float latitude = getLatitude();
    float longitude = getLongitude();
    float altitude = getAltitude();
    float speed = getSpeed();
    float heading = getHeading();

    dataFile.print(latitude, 6);
    dataFile.print(",");
    dataFile.print(longitude, 6);
    dataFile.print(",");
    dataFile.print(altitude, 2);
    dataFile.print(",");
    dataFile.print(speed, 2);
    dataFile.print(",");
    dataFile.print(heading, 2);
    dataFile.print(",");
}
