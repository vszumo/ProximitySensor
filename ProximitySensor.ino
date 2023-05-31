#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;

int speedAchteruit = -100;

void setup() {

  //wachten op button A
  buttonA.waitForButton();

  //de drie proximity sensoren activeren
  proxSensors.initThreeSensors();

  Serial.begin(9600);
  delay(1000);

}

void loop() {

  //Zorgen dat de leds uit staan
  ledRed(0);
  ledGreen(0);
  ledYellow(0);

  //Zorgen dat er waardes worden gelezen met de sensor
  proxSensors.read();

  //int waardes van sensoren toekennen
  int left_sensor = proxSensors.countsFrontWithLeftLeds();
  int right_sensor = proxSensors.countsFrontWithRightLeds();
  int left_center = proxSensors.countsLeftWithLeftLeds();
  int right_center = proxSensors.countsRightWithRightLeds();

  //Motors
  motors.setLeftSpeed(150);
  motors.setRightSpeed(150);

  //Als proximity sensor een count heeft van 2, krijgt de motor speed een waarde van 0, waardoor de zumo stil staat
 if (proxSensors.countsFrontWithLeftLeds() == 2 || proxSensors.countsFrontWithRightLeds() == 2) {
     motors.setLeftSpeed(0);
    motors.setRightSpeed(0);


 }
    //Als proximity sensor een count heeft van minder dan 2,  krijgt de motor speed een waarde van speedAchteruit,
    //waardoor de zumo stil naar achteren rijdt in increments van 50 op basis van de "counts" van de proxSensors.
if (proxSensors.countsFrontWithLeftLeds() > 2 || proxSensors.countsFrontWithRightLeds() > 2 ) {
    ledRed(1);
    motors.setLeftSpeed(speedAchteruit - 50 );
    motors.setRightSpeed(speedAchteruit - 50 );
}
if (proxSensors.countsFrontWithLeftLeds() > 3 || proxSensors.countsFrontWithRightLeds() > 3 ) {
    ledRed(1);
    ledGreen(1);
    motors.setLeftSpeed(speedAchteruit - 100 );
    motors.setRightSpeed(speedAchteruit - 100 );

}
if (proxSensors.countsFrontWithLeftLeds() > 5 || proxSensors.countsFrontWithRightLeds() > 5 ) {
    ledRed(1);
    ledGreen(1);
    ledYellow(1);
    motors.setLeftSpeed(speedAchteruit - 150 );
    motors.setRightSpeed(speedAchteruit - 150 );
}

  //Serial monitor waardes afdrukken
  Serial.print("       Links: ");
  Serial.println(left_sensor);
  Serial.print("       Rechts: ");
  Serial.println(right_sensor);

}