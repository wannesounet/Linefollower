// Bluetooth
#include <SoftwareSerial.h>
char BluetoothData;             // Variabele om data van BT-module op te slaan
SoftwareSerial WhaddaBT(0, 1);  // RX | TX

// LineSensor
#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// Movement
int motorLinks = 11;
int motorRechts = 10;
int speedR = 45;
int speedL = 45;

int ledPin = 13;
bool setupDone = false;
bool Movement = false;

// PID-parameters
float Kp = 0.35;
float Ki = 0.0;
float Kd = 0.1;

// PID-variabelen
float lastError = 0;
float integral = 0;

void setup() {
    pinMode(ledPin, OUTPUT);
    WhaddaBT.begin(9600);

    // LineSensor configuratie
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){ 3, 4, 5, 6, 7, 8, 9, 12 }, SensorCount);
    qtr.setEmitterPin(2);

    pinMode(motorRechts, OUTPUT);
    pinMode(motorLinks, OUTPUT);
}

void loop() {
    uint16_t position = qtr.readLineBlack(sensorValues);

    if (WhaddaBT.available()) {
        BluetoothData = WhaddaBT.read();
        if (BluetoothData == 'c') {
            WhaddaBT.println("Calibrating...");
            Calibrate();
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
        if (BluetoothData == '1') {
            if (!Movement) {
                digitalWrite(ledPin, HIGH);
                WhaddaBT.println("LineFollowerBot Start");
                Movement = true;
            }
            WhaddaBT.println(position);

            int correction = pid_control(position);

            int leftSpeed = speedL - correction;
            int rightSpeed = speedR + correction;

            leftSpeed = constrain(leftSpeed, 0, 255);
            rightSpeed = constrain(rightSpeed, 0, 255);

            analogWrite(motorLinks, leftSpeed);
            analogWrite(motorRechts, rightSpeed);
        } else if (BluetoothData == '0' && Movement) {
            digitalWrite(ledPin, LOW);
            WhaddaBT.println("LineFollowerBot Stop");
            analogWrite(motorLinks, 0);
            analogWrite(motorRechts, 0);
            Movement = false;
        }
    }
}

int pid_control(uint16_t position) {
    float error = ((float)position - 3500) / 3500.0;
    integral += error;
    float derivative = error - lastError;
    float correction = Kp * error + Ki * integral + Kd * derivative;
    lastError = error;
    return (int)(correction * 255);
}

void Calibrate() {
    digitalWrite(ledPin, HIGH);
    for (uint16_t i = 0; i < 100; i++) {
        qtr.calibrate();
    }
    WhaddaBT.println("Calibration minimum:");
    for (uint8_t i = 0; i < SensorCount; i++) {
        WhaddaBT.print(qtr.calibrationOn.minimum[i]);
        WhaddaBT.print('|');
    }
    WhaddaBT.println();
    WhaddaBT.println("Calibration maximum:");
    for (uint8_t i = 0; i < SensorCount; i++) {
        WhaddaBT.print(qtr.calibrationOn.maximum[i]);
        WhaddaBT.print('|');
    }
    WhaddaBT.println();
    WhaddaBT.println();
    delay(1000);
}
