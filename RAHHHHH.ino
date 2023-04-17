//Include needed libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>



//Define pins, constants, sensors, and the SD card. 
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_BME280 bme;
File myFile;
unsigned long delayTime;
const int chipSelect = 10;
//Setup function, only run once.
void setup(void) 
{
  //Open serial print on 1000000 baud channel and begin startup phase
  Serial.begin(1000000);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(.1);
    
  bno.setExtCrystalUse(true);

//Initialize BME280 sensor
Serial.begin(1000000);
  Serial.println(F("BME280 test"));

  bool status;
  
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  Serial.println("-- Default Test --");
  delayTime = .1;

  Serial.println();

//Initialize SD card
Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}
//Start Opening the file and writing data
void loop(void) 
{

  // open the file.
  myFile = SD.open("test.txt", FILE_WRITE);
  sensors_event_t event; 
  bno.getEvent(&event);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");


    //Get sensor events in preparation for writing
    sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

    // Write microseconds since powering on to SD card
    myFile.print(micros()/10000);
    myFile.print(",");
    //Write Temperature, Pressure, Altitude, and Humidity from the BME280 chip
    myFile.print(bme.readTemperature());
    myFile.print(",");
    myFile.print(bme.readPressure());
    myFile.print(",");
    myFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    myFile.print(",");
    myFile.print(bme.readHumidity());
    myFile.print(",");
    //print Orientation and Linear Acceleration, both in all 3 axes
    myFile.print(event.orientation.x, 4);
    myFile.print(",");
    myFile.print(event.orientation.y, 4);
    myFile.print(",");
    myFile.print(event.orientation.z, 4);
    //Close the file
    myFile.close();
    
    // Print everything from above, but onto serial monitor
    Serial.print(micros()/10000);
    Serial.print(",");
    Serial.print(bme.readTemperature());
    Serial.print(",");
    Serial.print(bme.readPressure());
    Serial.print(",");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(",");
    Serial.print(bme.readHumidity());
    Serial.print(",");
    Serial.print(event.orientation.x, 4);
    Serial.print(",");
    Serial.print(event.orientation.y, 4);
    Serial.print(",");
    Serial.print(event.orientation.z, 4);


  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }



  
  delay(.1);

  Serial.println();

}