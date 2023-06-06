#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4OLED display;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;

const uint8_t sensorLevel = 1;
const uint16_t turnSpeedMax = 500;
const uint16_t turnSpeedMin = 100;
const uint16_t deceleration = 10;
const uint16_t acceleration = 10;

#define LEFT 0
#define RIGHT 1

bool senseDir = RIGHT;
bool turningLeft = false;
bool turningRight = false;
uint16_t turnSpeed = turnSpeedMax;
uint16_t lastTimeObjectSeen = 0;

void setup() {

  //wachten op button A.
 

  //de drie proximity sensoren activeren.
  proxSensors.initThreeSensors();

  Serial.begin(9600);
  delay(1000);

  buttonA.waitForButton();
}

void turnRight()
{
  motors.setSpeeds(turnSpeed, -turnSpeed);
  turningLeft = false;
  turningRight = true;
}

void turnLeft()
{
  motors.setSpeeds(-turnSpeed, turnSpeed);
  turningLeft = true;
  turningRight = false;
}

void stop()
{
  motors.setSpeeds(0, 0);
  turningLeft = false;
  turningRight = false;
}


void loop() {
  //Zorgen dat er waardes worden gelezen met de sensor.
  proxSensors.read();
  //int waardes van sensoren toekennen.
  int left_sensor = proxSensors.countsFrontWithLeftLeds();
  int right_sensor = proxSensors.countsFrontWithRightLeds();

  bool objectSeen = left_sensor >= sensorLevel || right_sensor >= sensorLevel;

  if (objectSeen)
  { 
    //Om oscillatie te voorkomen, remmen we de motoren af.
    turnSpeed -= deceleration;

  }
  else
  {
    //Object is niet zichtbaar dus we versnellen de motor.
    turnSpeed += acceleration;
  }

  //Gemiddelde van turnspeed.
  turnSpeed = constrain(turnSpeed, turnSpeedMin, turnSpeedMax);

  if (objectSeen)
  {
    //Object is zichtbaar.
    ledYellow(1);
    motors.setSpeeds(800, 800);
    lastTimeObjectSeen = millis();

    if ((left_sensor < right_sensor) && (left_sensor-right_sensor > 1))
    {
      //Rechter sensor value is groter, dus stuur naar rechts.
      turnRight();
      senseDir = RIGHT;
    }
    else if ((left_sensor > right_sensor) && (left_sensor-right_sensor > 1))
    {
      //Linker sensor value is groter, dus stuur naar links.
      turnLeft();
      senseDir = LEFT;
    }
    else
    {
      //sensor zijn gelijk, dus rij richting.
      motors.setSpeeds(800, 800);
    }
  }
  else
  {
    //Geen object is zichtbaar, dus draai richting het laatst gevonden object.
    if (senseDir == RIGHT)
    {
      turnRight();
    }
    else
    {
      turnLeft();
    }
  }
}
