

#include <I2C.h>
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.


#include <Servo.h> 


Servo servoRotate;
Servo servoScan;

int servoRotatePosition = 0;
int servoScanPosition = 0;

int servoRotateDirection = 1;
int servoScanDirection = 1;

int sensorEnablePin = 4;

int distance;

void setup() 
{ 
  servoRotate.attach(9);  // attaches the servo on pin 9 to the servo object 
  servoScan.attach(7);  // attaches the servo on pin 9 to the servo object 

  Serial.begin(9600); //Opens serial connection at 9600bps.     

  I2c.begin(); // Opens & joins the irc bus as master
  delay(100); // Waits to make sure everything is powered up before sending or receiving data  
  I2c.timeOut(50); // Sets a timeout to ensure no locking up of sketch if I2C communication fails
  
  pinMode(sensorEnablePin, OUTPUT);
  digitalWrite(sensorEnablePin, HIGH); // Set trigger LOW for continuous read

  
  
  servoRotate.write(180);
  servoScan.write(180);
  delay(1000);
  
  while (!Serial.available()) {}
  
  servoRotate.write(servoRotatePosition);
  servoScan.write(servoScanPosition);
  delay(1000);



} 
 
void loop() 
{ 

  if (servoScanPosition <= 180)
  {

    
  
    servoRotate.write(servoRotatePosition);
    servoScan.write(servoScanPosition);
    
    
    
    








  // Write 0x04 to register 0x00
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.write(LIDARLite_ADDRESS,RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling
  }

  byte distanceArray[2]; // array to store distance bytes from read function
  
  // Read 2byte distance from register 0x8f
  nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.read(LIDARLite_ADDRESS,RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    delay(1); // Wait 1 ms to prevent overpolling
  }
  distance = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int
  
  // Print Distance
  printOutput(servoRotatePosition, servoScanPosition, distance);
  
  
  
  
  
    
    
    if (servoRotateDirection == 1)
      servoRotatePosition++;
    else
      servoRotatePosition--;
  
    if (servoRotatePosition == 180 || servoRotatePosition == 0)
    {
      servoRotateDirection = -servoRotateDirection;
      servoScanPosition = (servoScanPosition + 1)%182;
    }  
  
      
    delay(20);

     
  }
 /*
  for (int i = 0; i <= 180; i++)
  {
    servoScanPosition = i;
    servoScan.write(servoScanPosition);
    delay(50);

    for (int j = 0; j <= 180; j++)
    {
      int servoRotatePosition = (i%2 == 0) ? j : 180-j;
      servoRotate.write(servoRotatePosition);
      

      delay(40);

      

  
  
    }

  } 


*/
} 

void printOutput(int angleRotate, int angleScan, int distance)
{
  //angleRotate += 180;
  Serial.print("T");
  if (angleRotate < 10)
    Serial.print("00");
  else if (angleRotate < 100)
    Serial.print("0");
  Serial.print(angleRotate);
  
  //beta = (beta + 270)%360;
  //angleScan += 180;
  Serial.print("P");
  if (angleScan < 10)
    Serial.print("00");
  else if (angleScan < 100)
    Serial.print("0");
  Serial.print(angleScan);
  
    //distance = 30;
  Serial.print("V");
  if (distance < 10)
    Serial.print("000");
  else if (distance < 100)
    Serial.print("00");
  else if (distance < 1000)
    Serial.print("0");
  Serial.println(distance);
  
}

