#include "Dump.h"
#include "GyverStepper.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <Vector.h>

#define FIRMWARE_VERSION "0.1.1"

// steppers
GStepper<STEPPER2WIRE> stepper1(200, 4, 5);
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

void setup() {
  // testSteppers(); //TODO
  // timers setup
  // timer for outputting to serial and status reporting
  // Timer2.setPeriod(16384);
  // Timer2.enableISR();

  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

ISR(TIMER2_A) {
  // if (millis() - pingTimer >= 2000)
  //{
  //  pingTimer = millis();
  //  Serial.println("ping " + String(millis()));
  //}
}

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
    while (dividedString != NULL) {
      Serial.println(dividedString);
      dividedString = strtok(NULL, " ");
    }

    doAction(parsedString);
  }
  stepper1.tick();
  stepper2.tick();
}