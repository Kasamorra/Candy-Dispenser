//Candy Dispenser with three seperate containers and button for candy selection
//Step 2: Adding the LEDs for different modes and testing to switch between these
//Pins Servos: 9, 10, 13
//Pins LEDs: 8, 7, 6
//Pin Pushbutton: 4
//Pins Ultrasonic Sensor: 11, 12

//All Servos
#include <Servo.h> //Servo motor library
const int servoOpen = 90; //Angle of Servo when dispenser open
const int servoClosed = 0; //Angle of Servo when dispenser closed

//Servo One
const int pinS1 = 13; //Pin connected to first servo motor
Servo servo1; //creates an object of type Servo

//Servo Two
const int pinS2 = 10; //Pin connected to second servo motor
Servo servo2; //creates an object of type Servo

//Servo Three
const int pinS3 = 9; //Pin connected to third servo motor
Servo servo3; //creates an object of type Servo

//LEDs
const int pinLED1 = 8;
const int pinLED2 = 7;
const int pinLED3 = 6;

//Pushbutton
const int pinButton = 4;
int buttonStateOld = 1; // 0 = pressed, 1 = not pressed
int buttonStateNew;

//Ultrasonic Sensor
const int trigPin = 11;
const int echoPin = 12;
int pingTime; //time the Ping needs to travel

//General
int candyMode = 0; // 0 - 1 - 2
const int candyLED[] = {pinLED1, pinLED2, pinLED3};
const int candyOff1[] = {pinLED2, pinLED3, pinLED1};
const int candyOff2[] = {pinLED3, pinLED1, pinLED2};
const int openTime = 2000;
const int delayTime = 250;
const int microDelay = 10;

void setup() {
  Serial.begin(9600); //for Troubleshooting

  //Servos
  servo1.attach(pinS1);
  servo2.attach(pinS2);
  servo3.attach(pinS3);

  //LEDs
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinLED3, OUTPUT);

  //Pushbotton
  pinMode(pinButton, INPUT);

  //Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  //reset
  servo1.write(servoClosed);
  servo2.write(servoClosed);
  servo3.write(servoClosed);

  //checking the candy mode
  buttonStateNew = digitalRead(pinButton);

  //switching candy mode
  if (buttonStateOld == 0 && buttonStateNew == 1)
  { //checks if button was pressed -> switches combo when not pressed anymore
    candyMode = (candyMode + 1) % 3;
  }
  buttonStateOld = buttonStateNew;

  //controlling LEDs
  digitalWrite(candyLED[candyMode], HIGH);
  digitalWrite(candyOff1[candyMode], LOW);
  digitalWrite(candyOff2[candyMode], LOW);

  //checking the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(microDelay);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(microDelay);
  digitalWrite(trigPin, LOW);
  pingTime = pulseIn(echoPin, HIGH);

  Serial.println(pingTime);

  //checking if we should dispense candy
  if (pingTime < 250)
  {
    switch (candyMode)
    {
      case 0: //First Mode
        servo1.write(servoOpen);
        delay(openTime);
        servo1.write(servoClosed);
        break;

      case 1: //Second Mode
        servo2.write(servoOpen);
        delay(openTime);
        servo2.write(servoClosed);
        break;

      case 2: //Third Mode
        servo3.write(servoOpen);
        delay(openTime);
        servo3.write(servoClosed);
        break;
    }
  }

  delay(delayTime);
}
