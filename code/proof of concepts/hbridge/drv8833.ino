int motorLinks = 11;      // LED connected to digital pin 10
int motorRechts = 10;      // LED connected to digital pin 10
int speedR = 100;
int speedL = 85;


void setup() {
  pinMode(motorRechts, OUTPUT);  // sets the pin as output
  pinMode(motorLinks, OUTPUT);  // sets the pin as output
}

void loop() {

  analogWrite(motorLinks, speedL); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(motorRechts, speedR); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  delay(500);
  analogWrite(motorLinks, 0.75*speedL); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(motorRechts, 0.75*speedR); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255 
  delay(500);
  analogWrite(motorLinks, 0.50*speedL); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(motorRechts, 0.50*speedR); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255 
  delay(500);
  analogWrite(motorLinks, 0.25*speedL); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(motorRechts, 0.25*speedR); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255 
  delay(500);
  analogWrite(motorLinks, 0); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(motorRechts, 0); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255  
  

}