#include <SPI.h>                  //library for SPI communication (standard protocol for comunication with SD loggers)
#include <SD.h>                   //library for SD manipulation (read/write)
#include <Wire.h>                 //library for SCL and SCI for melexis IR sensor
#include <Adafruit_MLX90614.h>    //library for melexis IR sensor

Adafruit_MLX90614 mlx_1 = Adafruit_MLX90614();  // first melexis IR sensor I2C address specification


//A0(14) and A1(15) are assigned to thermistor 1 and 2
int thermistor1=14, thermistor2=15;

// Set the pins used
#define chipSelect 10

File logfile;              //creating an object type file for the data logging



// blink out an error code
void error(uint8_t errno) {
  /*
  if (SD.errorCode()) {
   putstring("SD error: ");
   Serial.print(card.errorCode(), HEX);
   Serial.print(',');
   Serial.println(card.errorData(), HEX);
   }
   */
  while(1) {
    uint8_t i;
 
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

void setup() {


  Serial.begin(115200);
  Serial.println("\r\nNeslab temperature measurement program");
  


  // make sure that the default chip select pin is set to
  // output, even if it is not used:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {      
    Serial.println("Card init. failed!");
    error(2);
  }
  Serial.println("card initialized.");
  
  
  //creating a filename
  char filename[15];
  strcpy(filename, "templog00.txt");
  for (uint8_t i = 0; i < 100; i++) {                                    //this loop keeps trying names for the file. If it exists, it tries another name until one which doesn't exist is found
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  //creating the file itself and assigning it to logfile (file object)
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create datalog"); 
    error(3);
  }



  //melexis sensor is started
  mlx_1.begin(); 
  
  Serial.println("Ready!");
}



void loop() {

          Serial.println("Logging");
    
          //sending temperature reads to the SD card
          logfile.print(mlx_1.readObjectTempC());logfile.print('\t');    //melexis 1 reading
          logfile.print(analogRead(thermistor1));logfile.print('\t');    //thermistor 1 reading
          logfile.print(analogRead(thermistor2));logfile.print('\t');    //thermistor 2 reading
          
          //echo to serial port for debbuging
           Serial.print(mlx_1.readObjectTempC());Serial.print('\t');    //melexis 1 reading
          Serial.print(analogRead(thermistor1));Serial.print('\t');    //thermistor 1 reading
          Serial.print(analogRead(thermistor2));Serial.print('\t'); 

          logfile.println();
          Serial.println();
          logfile.flush();    //this forces a save of the data
          Serial.println("Logged");
          delay(1000);
          
}


/* End code */
