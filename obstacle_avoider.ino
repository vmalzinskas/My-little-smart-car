#include <AFMotor.h>

AF_DCMotor motorRearLeft(1, MOTOR12_1KHZ);
AF_DCMotor motorRearRight(2, MOTOR12_1KHZ);
AF_DCMotor motorFrontRight(3, MOTOR34_1KHZ);
AF_DCMotor motorFrontLeft(4, MOTOR34_1KHZ);

const int pingPin = A2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = A3; // Echo Pin of Ultrasonic Sensor

//prototypes
long microsecondsToCentimeters(long microseconds);
void singleDrive(int cartSpeed);
int check();
void stopCart();
void turnLeft(int imer);
void turnRight(int timer);
void reverse(int cartSpeed);

void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready.>");
  delay(1000);
  singleDrive(200);
  }

void loop() {
  long impact_distance;  
  int  clearDirection;
  impact_distance = distanceScan();
  
  if(impact_distance>35){
    singleDrive(200);
    }
  else if(impact_distance<35 && impact_distance>20){
    singleDrive(100);
    }
  else if(impact_distance<20 && impact_distance>112){
    singleDrive(50);
  }
  else if(impact_distance<12){
      clearDirection = check();
        switch(clearDirection){
          case(-100):
          turnLeft(400);
          break;
          case(0):
          reverse(150);
          turnLeft(1000);
          break;
          case(100):
          turnRight(400);
          break;
        }
  }
}




////////////////////////////////
//functions

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}   
void singleDrive(int cartSpeed){ // This function stops the car from what it is doing and drives all wheels at the same speed.
    stopCart();
    motorRearRight.setSpeed(cartSpeed);
    motorRearLeft.setSpeed(cartSpeed);
    motorFrontRight.setSpeed(cartSpeed);
    motorFrontLeft.setSpeed(cartSpeed);
    motorRearRight.run(FORWARD);
    motorRearLeft.run(FORWARD);
    motorFrontRight.run(FORWARD);
    motorFrontLeft.run(FORWARD);
  }

int check(){ // This function checks for obstacles when not driving
  long cmLeft, cmRight, cmForward;
  int delayDirection;
    
    stopCart();
    cmForward = distanceScan(); //Look Forward
    turnLeft(400); // Look left
    cmLeft = distanceScan();
    turnRight(800); // Look right
    cmRight = distanceScan();
    turnLeft(400); // This should return to original heading.
    stopCart();

    if(cmForward>cmLeft && cmForward>cmRight)
      return 0;
    else if(cmLeft>cmForward && cmLeft>cmRight)
      return -100;
    else if(cmRight>cmLeft && cmRight>cmForward)
      return 100;
   
  }

long distanceScan(){ //Scans distance to object
   long duration, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);
return cm;
  }
  
void stopCart(){
    motorRearRight.run(RELEASE);
    motorRearLeft.run(RELEASE);
    motorFrontRight.run(RELEASE);
    motorFrontLeft.run(RELEASE);
  }

void turnLeft(int timer){
    stopCart();
    motorRearRight.setSpeed(200);
    motorRearLeft.setSpeed(200);
    motorFrontRight.setSpeed(200);
    motorFrontLeft.setSpeed(200);
    motorRearRight.run(FORWARD);
    motorRearLeft.run(BACKWARD);
    motorFrontRight.run(FORWARD);
    motorFrontLeft.run(BACKWARD);
    delay(timer);
    stopCart();
    delay(200);
  }

void turnRight(int timer){
    stopCart();
    motorRearRight.setSpeed(200);
    motorRearLeft.setSpeed(200);
    motorFrontRight.setSpeed(200);
    motorFrontLeft.setSpeed(200);
    motorRearRight.run(BACKWARD);
    motorRearLeft.run(FORWARD);
    motorFrontRight.run(BACKWARD);
    motorFrontLeft.run(FORWARD);
    delay(timer); 
    stopCart();
    delay(200); 
  }

void reverse(int cartSpeed){
    stopCart();
    motorRearRight.setSpeed(cartSpeed);
    motorRearLeft.setSpeed(cartSpeed);
    motorFrontRight.setSpeed(cartSpeed);
    motorFrontLeft.setSpeed(cartSpeed);
    motorRearRight.run(BACKWARD);
    motorRearLeft.run(BACKWARD);
    motorFrontRight.run(BACKWARD);
    motorFrontLeft.run(BACKWARD);
}
