#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h> 

// definition of the RFID
RFID rfid(10,5);
bool ISLock = true;

byte rfidmipo[5] = {0xB0,0xCA,0xFA,0x71,0xF1}; 
byte rfidnum2[5] = {0x44,0xCB,0x94,0x5B,0x40}; 
byte serNum[5];
byte data[5];

//set LCD_I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// change this to fit the number of steps per revolution
const int stepsPerRevolution = 200;

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 8, 9, 6, 3);
Servo myservo; 

void setup(){
  Serial.begin(9600); // initialize serial communication
  SPI.begin(); // initialize SPI communication for RFID
  rfid.init(); // initialize the RFID
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.println("+                    RFID Doorlock (v1)                     +");
  Serial.println("+           Please choose from the following:               +");
  Serial.println("+           - Scan your card                                +");
  Serial.println("+           - Press the button to add new RFID tag#         +");
  Serial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  lcd.begin(16, 2);      // initial LCD，16 characters for one line * 2 ，default backlight on
  
  for(int i = 0; i < 3; i++) {
    lcd.backlight();     // turn on the backlight
    delay(250);
    lcd.noBacklight();   // turn off the backlight
    delay(250);
  }
  lcd.backlight();
  lcd.setCursor(0, 0); //  set cursor at 0,0
  lcd.print("Unlock With RFID");
  delay(1000);
  lcd.setCursor(0, 1); //  set cursor at 0,1
  lcd.print("ESLAB FINAL");
  delay(500);
  
  myservo.attach(9);
}
 
void loop(){

  // Create a variable for each user
  boolean rfidmipo_card = true; // my card
  
  if (rfid.isCard()){ // found valid card
    if (rfid.readCardSerial()){ // reads the card
      delay(1000);
      data[0] = rfid.serNum[0]; // stores the serial number
      data[1] = rfid.serNum[1];
      data[2] = rfid.serNum[2];
      data[3] = rfid.serNum[3];
      data[4] = rfid.serNum[4];
    }
  
  //rfid.halt(); // RFID to standby
 if(data[0] < 16){
    Serial.print("0");
  }
  Serial.print(data[0],HEX);
  
  if(data[1] < 16){
    Serial.print("0");
  }
  Serial.print(data[1],HEX);
  
  if(data[2] < 16){
    Serial.print("0");
  }
  Serial.print(data[2],HEX);
  
  if(data[3] < 16){
    Serial.print("0");
  }
  Serial.print(data[3],HEX);
  
  if(data[4] < 16){
    Serial.print("0");
  }
  Serial.print(data[4],HEX);
  for(int i=0; i<5; i++){
    if(data[i] != rfidmipo[i]) rfidmipo_card = false; // if not my card, poses as fake. Here you can check the other cards allowed
  }
  Serial.println();
  if (rfidmipo_card){ // A card found
    Serial.print("Weclome!! Have a nice day!."); //Serial.println("Weclome!! Have a nice day!."); // print message
    
    if(ISLock == true){
       Serial.println("clockwise");
       myservo.write(-15);
       delay(500);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Unlock Now");
       lcd.setCursor(0, 1);
       lcd.print("Welcome");
       ISLock = false;
       
       delay(1000);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Lock With RFID");
       lcd.setCursor(0, 1);
       lcd.print("ESLAB FINAL");
    }
    
    else{
       Serial.println("counterclockwise");
       myservo.write(+30);
       delay(250);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("On Lock Again");
       delay(250);
       lcd.setCursor(0, 1);
       lcd.print("Have a nice day!");
       ISLock = true;
       
       delay(1000);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Unlock With RFID");
       delay(250);
       lcd.setCursor(0, 1);
       lcd.print("ESLAB FINAL");
    }
  }
  else{ // if an unknown card is scanned
    Serial.print("Card not recognized. Access denied."); // Prints message
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access denied");
    lcd.setCursor(0, 1);
    lcd.print("Another Try!");
    
    delay(1000);
    lcd.setCursor(0, 0); 
    lcd.print("Unlock With RFID");
    lcd.setCursor(0, 1); 
    lcd.print("ESLAB FINAL");
  }

  Serial.println();
  delay(500);
  rfid.halt();
  }
}
  

