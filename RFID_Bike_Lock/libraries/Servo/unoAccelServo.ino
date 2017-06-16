/*
This Sketch Written By Mike Hall
Feel Free to distribute and alter this code!

Using Memsic 2125 Accelerometer, Standard Hobby Servos, and Arduino Uno.
Matches tilt from accelerometer to servos. Video at http://www.youtube.com/watch?v=i_1c97m_p0I
*/
#include <Servo.h> //Include Servo Library

Servo xServo;  //Declares servos
Servo yServo;
int xsPin = 11;  //Pin for Servos
int ysPin = 10;
int xPin = 2;  //Pins for Accelerometer
int yPin = 3;
int sensitivity = 10;  //If > 2, will require a larger movement before activating servos
boolean setServo = false;  //Flag for whether to change servo position or not
int temp;  //temporary calculations value
int accelerationX, accelerationY = 90;  //Servo positions, initializes to middle of field

void setup()
{
  xServo.attach(xsPin);  //Initializes Servos
  yServo.attach(ysPin);
}

void loop()
{
  int pulseX, pulseY;  //Resets and declares input variables
  setServo = false;  //Resets flags
  digitalWrite(13, LOW);  //Turns Uno LED off

  pulseX = (((pulseIn(xPin, HIGH) - 3700) * 18.0) / 250);  //reading from accelerometer and converting value to 0-180
  pulseY = (((pulseIn(yPin, HIGH) - 3700) * 18.0) / 250);
  
  temp = (accelerationX - pulseX);  //Determines difference between new readings and current position
  temp = abs(temp);
  if (temp > sensitivity)  //Checks difference to sensitivity value set above
  {
    accelerationX = pulseX;
    setServo = true;
  }
  if (accelerationX > 180)  //Sets max value
  {
    accelerationX = 180;
    digitalWrite(13, HIGH);
  }
  if (accelerationX < 0)  //Sets min value
  {
    accelerationX = 0;
    digitalWrite(13, HIGH);
  }
  
  temp = (accelerationY - pulseY);  //Determines difference between new readings and current position
  temp = abs(temp);
  if (temp > sensitivity)  //Checks difference to sensitivity value set above
  {
    accelerationY = pulseY;
    setServo = true;
  }
  if (accelerationY > 180)  //Sets max value
  {
    accelerationY = 180;
    digitalWrite(13, HIGH);
  }
  if (accelerationY < 0)  //Sets min value
  {
    accelerationY = 0;
    digitalWrite(13, HIGH);
  }
  
  if (setServo)  //Adjusts Servo Positions
  {
    xServo.write(accelerationX);
    yServo.write(accelerationY);
  }
  
  delay(1);  //Tiny delay to reduce electrical noise.
      
}
