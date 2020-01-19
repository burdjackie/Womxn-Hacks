//Include Libraries
#include <Servo.h> 
#include <Stepper.h>

//Definitions + Pin Variables
#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
#define CLK 2
#define DT 3
#define SW 4
#define SOIL 5
#define WHLPWR 6
#define ARM 7
#define SNSR A0
#define CIRC 134
#define WHIGH 12
#define WLOW 13



//Declare Variables
int counter = 0;
int currentStateLK;
int lastStateCLK;
String currentDir = ""; //for testing
int soilValue = 0;
int soilThreshold = 700;
bool needWater = 0;
bool refill = 0;
bool dir = 0;
int tiltSteps = 2038/4;
int waitTime = 8000;
int plantSpacing = 200; //200 milimeters
int maxDist = 0;

//Create Objects
Servo servoArm;
Stepper stepper(STEPS, 8, 10, 9, 11); //steps,in1,in2,in3,in4

void setup() {
  refill = 1;
  delay(waitTime);
  refill = 0;

  //set pins
  pinMode(CLK, INPUT); //rotary coutner
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(SOIL, OUTPUT);
    digitalWrite(SOIL, LOW);
  pinMode(WHIGH, OUTPUT);
    digitalWrite(WHIGH, LOW);
  pinMode(WLOW, OUTPUT);
    digitalWrite(WLOW, LOW);

  //Servo Arm Initialization
  servoArm.attach(ARM);
  servoArm.write(0);

  Serial.begin(9600);

}

void loop() {
  //refill check
  if(refill){
    delay(waitTime);
    refill = 0;
    moveForwards(maxDist);
  }
  
  moveForwards(plantSpacing);
  lowerArm();
  needWater = getMoistureReading();
  raiseArm();
  
  if(needWater){
    tiltDown();
    delay(3000);
    tiltUp();
    refill = 1;
    moveBackwards(maxDist);
  }else{
    maxDist += plantSpacing;
  }
  
}

//Wheel Functions
void moveForwards(int d){ //set positive voltage  

  while(counter!=d){
    digitalWrite(WHIGH, HIGH); //5V
    digitalWrite(WLOW, LOW); //0V
  }
  digitalWrite(WHIGH, LOW); //0V

  
}
void moveBackwards(int d){ //set negative voltage 
  while(getDistancecounter)
  {
    digitalWrite(WHIGH, LOW);
    digitalWrite(WLOW, HIGH);
  }
  digitalWrite(WLOW, LOW);
}

void brake() //voltage = 0
{
    digitalWrite(WHIGH, LOW);
    digitalWrite(WLOW, LOW);
}

int getDistance(int rCount){
  return ((rCount/20)*134); //return in milimeters
}


//Servo Arm Functions
void raiseArm(){
  servoArm.write(0);//lower arm
  delay(2000);
}
void lowerArm(){
  servoArm.write(100);//lower arm
  delay(2000);
}

//Moisture Sensor Function
bool getMoistureReading(){
  digitalWrite(SOIL, HIGH);//turn D7 "On"
  delay(10);//wait 10 milliseconds
  soilValue = analogRead(SNSR);//Read the SIG value from sensor
  
  return (soilValue<soilThreshold);
}

//Cup Functions
void tiltDown(){
  stepper.setSpeed(1); // 1 rpm (slow)
  stepper.step(tiltSteps); 
}

void tiltUp(){
  stepper.setSpeed(6); // 6 rpm (faster)
  stepper.step(-tiltSteps);
}
