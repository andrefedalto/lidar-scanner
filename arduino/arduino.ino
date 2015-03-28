#include <Servo.h> 
#include <I2C.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

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
  servoRotate.attach(9);  //Servo making the pan movement
  servoScan.attach(7);    //Servo making the tilt movement

  Serial.begin(9600); 

  //Joining the I2C bus
  I2c.begin();
  delay(100);
  I2c.timeOut(50);
  
  //Enabling the sensor
  pinMode(sensorEnablePin, OUTPUT);
  digitalWrite(sensorEnablePin, HIGH); // Set trigger LOW for continuous read

  //Moving the servo to create some nice moving effect when the scan starts
  servoRotate.write(180);
  servoScan.write(180);
  delay(1000);
  
  //Keep pooling the serial buffer until any command is received from the PC
  while (!Serial.available()) {}
  
  //Moving to initial position
  servoRotate.write(servoRotatePosition);
  servoScan.write(servoScanPosition);
  delay(1000);
} 
 
void loop() 
{ 

  //Limit to only one scan, for more the program (or the arduino) must be restarted
  if (servoScanPosition <= 180)
  {

    servoRotate.write(servoRotatePosition);
    servoScan.write(servoScanPosition);
    
    
    // Based on example code from https://github.com/PulsedLight3D/

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
  
    
    // Pooling the sensor from inside nested loops was giving bugs and freezing the loops
    // The following code will make both servos move using only the built-in arduino loop()
    
    //Making the pan servo move both ways
    if (servoRotateDirection == 1)
      servoRotatePosition++;
    else
      servoRotatePosition--;
  
  
    //Changing the direction in the edges
    //Moving the tilt servo only when the pan reaches each side
    if (servoRotatePosition == 180 || servoRotatePosition == 0)
    {
      servoRotateDirection = -servoRotateDirection;
      servoScanPosition = (servoScanPosition + 1)%182;
    }  
  
    //Delay to allow servos moving and sensor reading  
    delay(20);

  }
} 

//We need a string with fixed length, so zeroes are added to the distances
void printOutput(int angleRotate, int angleScan, int distance)
{
  Serial.print("T");
  if (angleRotate < 10)
    Serial.print("00");
  else if (angleRotate < 100)
    Serial.print("0");
  Serial.print(angleRotate);
  
  Serial.print("P");
  if (angleScan < 10)
    Serial.print("00");
  else if (angleScan < 100)
    Serial.print("0");
  Serial.print(angleScan);
  
  Serial.print("V");
  if (distance < 10)
    Serial.print("000");
  else if (distance < 100)
    Serial.print("00");
  else if (distance < 1000)
    Serial.print("0");
  Serial.println(distance);
}

