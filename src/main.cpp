#include "Dump.h"
#include "GyverStepper.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Vector.h>

#define FIRMWARE_VERSION "0.1.1"

// global varibles
uint16_t statusReportFrequency = 1; // in Hz

// steppers
GStepper<STEPPER2WIRE> stepper1(200 * 8 * 2 * 64.77, 4, 5);
GStepper<STEPPER2WIRE> stepper2(200, 5, 6);

// global storage variables
uint32_t pingTimer;

// print/return avalile memory
int availableMemory() {
  int size = 2048;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL)
    ;
  free(buf);
  Serial.println("RAM left: " + String(size));
  return size;
};

ISR(TIMER1_A) {
  DynamicJsonDocument data(10);
  data["st1Deg"] = stepper1.getCurrentDeg();
  data["st2Deg"] = stepper2.getCurrentDeg();
  serializeJson(data, Serial);
}

void setup() {
  Serial.begin(115200);
  // timers setup
  // timer1 is for status reporting
  Timer1.setFrequency(statusReportFrequency);
  Timer1.enableISR();

  stepper1.setRunMode(KEEP_SPEED);
  stepper1.setSpeedDeg(1);
  pinMode(13, OUTPUT);
}

// blink led number of times passed as arameter
void blinkLed(int numberOfBlinks) {
  for (int i = 0; i < numberOfBlinks; i++) {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
  }
}

// blink [number of blinks] - only for test purposes! Blink led on pin 13, made with delay, so unstable
// rotate [to] - rotate to some point in decimal degrees
// ping - returns pong to serial port
void doAction(String data[]) {
  if (data[0] == "moteTo") {
    Serial.println("moveTo");

  } else if (data[0] == "stop") {
    stepper1.setSpeed(0);
    stepper2.setSpeed(0);

  } else if (data[0] == "ping") {
    Serial.println("pong");
    Serial.println("version: " + String(FIRMWARE_VERSION));

  } else if (data[0] == "blink") {
    blinkLed(data[1].toInt());
  }
}

void loop() {
  if (Serial.available()) {

    // recieveing string and parsing into strings array
    String recievedString = Serial.readStringUntil('\n');
    uint16_t recievedStringLength = recievedString.length();
    char convertedString[recievedStringLength];
    recievedString.toCharArray(convertedString, recievedStringLength + 1);
    char *dividedString = strtok(convertedString, " ");

    String parsedString[recievedStringLength];
    uint8_t index = 0;
    while (dividedString != NULL) {
      Serial.println(dividedString);
      parsedString[index] = dividedString;
      index++;
      dividedString = strtok(NULL, " ");
    }
    doAction(parsedString);
  }
  stepper1.tick();
  stepper2.tick();
  // Serial.println(stepper1.getCurrentDeg());
}