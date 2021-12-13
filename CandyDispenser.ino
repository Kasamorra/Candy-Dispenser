//Candy Dispenser with three seperate containers and button for candy selection
//Components: 3x Servo Motor, 3x LED, 3x 330 Ohm Resistor, Pushbutton, Ultrasonic Sensor
//Pins Servos: 9, 10, 13
//Pins LEDs: 8, 7, 6
//Pin Pushbutton: 2
//Pins Ultrasonic Sensor: 11, 12

//All Servos
#include <Servo.h> //Servo motor library
const int servoOpen = 90; //Angle of Servo when dispenser open
const int servoClosed = 0; //Angle of Servo when dispenser closed

//Servo One
const int pinS1 = 13;
Servo servo1; 

//Servo Two
const int pinS2 = 10;
Servo servo2; 

//Servo Three
const int pinS3 = 9; 
Servo servo3; 

//LEDs
const int pinLED1 = 8;
const int pinLED2 = 7;
const int pinLED3 = 6;

//Pushbutton
const int pinButton = 2;
volatile int buttonStateOld = 1; // 0 = pressed, 1 = not pressed
volatile int buttonStateNew;

//Ultrasonic Sensor
const int trigPin = 11; //sends Ping
const int echoPin = 12; //receives Ping
int pingTime; //time the Ping needs to travel
const int microDelay = 10; 

//General
volatile int candyMode = 0; // 0 - 1 - 2, used to choose between different candy containers
const int candyLED[] = {pinLED1, pinLED2, pinLED3}; //LED of chosen container
const int candyOff1[] = {pinLED2, pinLED3, pinLED1}; //LED of one of the non-chosen containers
const int candyOff2[] = {pinLED3, pinLED1, pinLED2}; //LED of the other non-chosen container 
const int openTime = 2000; //time the dispenser stays open
const int delayTime = 150; //increase for troubleshooting
const int pingTimeLimit = 280; //time it should take the ping to travel if dispenser activated

//Interrupt -> gets called every time button gets pressed and released
void buttonISR()
{
  candyMode = (candyMode + 1) % 3;
}


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

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(pinButton), buttonISR, RISING);
}


void loop() {

  //reset
  servo1.write(servoClosed);
  servo2.write(servoClosed);
  servo3.write(servoClosed);

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

  //checking if candy is to be dispensed
  if (pingTime < pingTimeLimit)
  {
    switch (candyMode)
    {
      case 0: //Opens first Candy Container
        servo1.write(servoOpen);
        delay(openTime);
        servo1.write(servoClosed); //closes container again
        break;

      case 1: //Opens second Candy Container
        servo2.write(servoOpen);
        delay(openTime);
        servo2.write(servoClosed); //closes container again
        break;

      case 2: //Opens third Candy Container
        servo3.write(servoOpen);
        delay(openTime); 
        servo3.write(servoClosed); //closes container again
        break;
    }
  }

  delay(delayTime); //increase for troubleshooting
}
