// Bluetooth
#include <SoftwareSerial.h>
char BluetoothData;             // variable to store data from BT module
SoftwareSerial WhaddaBT(0, 1);  // RX | TX

// LineSensor
#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// Movement
int motorLinks = 11;   // LED connected to digital pin 10
int motorRechts = 10;  // LED connected to digital pin 10
int speedR = 100;
int speedL = 0.85 * speedR;

int ledPin = 13;         // set ledpin to pin 13
bool setupDone = false;  // Flag to indicate setup has been completed
bool Movement = false;

void setup() {
  pinMode(ledPin, OUTPUT);  // Set led pin as OUTPUT

  //Bluetooth
  WhaddaBT.begin(9600);  // Initialize the Serial connection with the BT module at 9600 baud

  // LineSensor
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){ 3, 4, 5, 6, 7, 8, 9, 12 }, SensorCount);
  qtr.setEmitterPin(2);

  // Movement
  pinMode(motorRechts, OUTPUT);  // sets the pin as output
  pinMode(motorLinks, OUTPUT);   // sets the pin as output
}

void loop() {
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);
  // When new data is available from the BT module:
  if (WhaddaBT.available()) {
    BluetoothData = WhaddaBT.read();  // Read incoming data
    if (BluetoothData == 'c') {
      WhaddaBT.println("Calibrating...");
      Calibrate();
      //Controle van de calibratie
      for (uint8_t i = 0; i < SensorCount; i++) {
        if ((qtr.calibrationOn.minimum[i] > 100) || (qtr.calibrationOn.maximum[i] < 1000)) {
          setupDone = false;
          WhaddaBT.println("To recalibrate enter 'c'");
          break;
        } else {
          setupDone = true;
        }
      }
    }
  }
  if (setupDone) {
    // WhaddaBT.println("Ready to GO");

    // If incoming data is 1
    if (BluetoothData == '1') {
      if (Movement == false) {
        digitalWrite(ledPin, HIGH);                 // Turn on LED
        WhaddaBT.println("LineFollowerBot Start");  // Send "LED turned ON" message to BT module
        Movement = true;
      }
      WhaddaBT.println(position);
      if ((position > 2500) && (position < 4500)) {
        analogWrite(motorLinks, 0.75 * speedL);   // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0.75 * speedR);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }

      if ((position >= 4500) && (position < 5500)) {
        analogWrite(motorLinks, 0.25 * speedL);   // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0.75 * speedR);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
      if ((position <= 2500) && (position > 1500)) {
        analogWrite(motorLinks, 0.75 * speedL);   // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0.25 * speedR);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
      if ((position >= 5500) && (position < 6500)) {
        analogWrite(motorLinks, 0 * speedL);      // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0.75 * speedR);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
      if ((position <= 1500) && (position > 500)) {
        analogWrite(motorLinks, 0.75 * speedL);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0 * speedR);    // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
      if (position >= 6500) {
        analogWrite(motorLinks, 0 * speedL);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, speedR);     // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
      if (position <= 500) {
        analogWrite(motorLinks, speedL);       // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
        analogWrite(motorRechts, 0 * speedR);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }

    } else if ((BluetoothData == '0') && (Movement == true)) {
      digitalWrite(ledPin, LOW);                 // Turn off LED
      WhaddaBT.println("LineFollowerBot Stop");  // Send "LED turned OFF" message to BT module
      analogWrite(motorLinks, 0);                // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      analogWrite(motorRechts, 0);               // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      Movement = false;
    }
  }
}
void Calibrate() {
  digitalWrite(ledPin, HIGH);  // Turn on LED
  // Calibrate the sensor
  for (uint16_t i = 0; i < 100; i++) {
    qtr.calibrate();
  }
  WhaddaBT.println("Calibration minimum:");
  // Print the calibration minimum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++) {
    WhaddaBT.print(qtr.calibrationOn.minimum[i]);
    WhaddaBT.print('|');
  }
  WhaddaBT.println();
  WhaddaBT.println("Calibration maximum:");
  // Print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++) {
    WhaddaBT.print(qtr.calibrationOn.maximum[i]);
    WhaddaBT.print('|');
  }
  WhaddaBT.println();
  WhaddaBT.println();
  delay(1000);
}
