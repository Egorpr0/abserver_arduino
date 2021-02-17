#include "Dump.h"
#include "GyverStepper.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <ArduinoJson.h>

//#define ARDUINOJSON_DECODE_UNICODE 0 TODO - fix
#define FIRMWARE_VERSION "0.1.2"

// global varibles
float statusReportFrequency = 0.25; // in Hz

// steppers
GStepper<STEPPER2WIRE> stepperHa(200 * 8 * 2 * 64.77, 3, 2, 5);
GStepper<STEPPER2WIRE> stepperDec(200 * 8 * 2 * 64.77, 7, 6, 12);

// print/return avalile memory
int availableMemory() {
  int size = 2048;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL)
    ;
  free(buf);
  return size;
};

ISR(TIMER1_A) {
  DynamicJsonDocument data(128);
  data["type"] = "status";
  data["stHaDeg"] = stepperHa.getCurrentDeg();
  data["stDecDeg"] = stepperDec.getCurrentDeg();
  data["aMem"] = availableMemory();
  String serializedJson;
  serializeJson(data, serializedJson);
  Serial.println(serializedJson);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  StaticJsonDocument<20> initMessage;
  initMessage["type"] = "initialize";
  String initMessageString;
  serializeJson(initMessage, initMessageString);
  Serial.println(initMessageString);

  stepperHa.setRunMode(KEEP_SPEED);
  stepperDec.setRunMode(KEEP_SPEED);
  stepperHa.setMaxSpeedDeg(2);
  stepperDec.setMaxSpeedDeg(2);
  stepperHa.disable();
  stepperDec.disable();
  // TODO enable stepper testing.
  // stepperHa.setSpeedDeg(degPerMinute(-10));
  // stepperDec.setSpeedDeg(degPerMinute(-10));

  // timers setup
  // timer1 is for status reporting
  Timer1.setFrequencyFloat(statusReportFrequency);
  Timer1.enableISR();

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

// rotate [to] - rotate to some point in decimal degrees
// ping - returns pong to serial port
void doAction(DynamicJsonDocument action) {
  auto actionType = action["n"];
  for (size_t i = 0; i < actionType.size(); i++) {
    actionType[i] = tolower(actionType[i]);
  }

  if (actionType == "moveto") {
    float ha = action["p"]["ha"];
    float dec = action["p"]["dec"];

    stepperHa.setRunMode(FOLLOW_POS);
    stepperDec.setRunMode(FOLLOW_POS);

    stepperHa.setTargetDeg(ha);
    stepperDec.setTargetDeg(dec);

  } else if (actionType == "stop") {
    stepperHa.setSpeed(0);
    stepperDec.setSpeed(0);
    stepperHa.disable();
    stepperDec.disable();

  } else if (actionType == "track") {
    stepperHa.setRunMode(KEEP_SPEED);
    stepperHa.setSpeedDeg(degPerHour(15));

  } else if (actionType == "move") {
    if (action["p"]["haDiff"]) {
      stepperHa.setRunMode(FOLLOW_POS);
      stepperHa.setTargetDeg(action["p"]["haDiff"], RELATIVE);
    }
    if (action["p"]["decDiff"]) {
      stepperDec.setRunMode(FOLLOW_POS);
      stepperDec.setTargetDeg(action["p"]["decDiff"], RELATIVE);
    }
  } else if (actionType == "ping") {
    StaticJsonDocument<64> pingResponse;
    pingResponse["type"] = "pong";
    pingResponse["version"] = String(FIRMWARE_VERSION);
    String pingResponseSerialized;
    serializeJson(pingResponse, pingResponseSerialized);
    Serial.println(pingResponseSerialized);

  } else if (actionType == "test") {
    stepperHa.setRunMode(KEEP_SPEED);
    stepperDec.setRunMode(KEEP_SPEED);
    stepperHa.setSpeedDeg(1);
    stepperDec.setSpeedDeg(1);
  }
}

void loop() {
  if (Serial.available()) { // TODO change reading method
    String recievedString = Serial.readStringUntil('\n');
    StaticJsonDocument<128> recievedJson;
    deserializeJson(recievedJson, recievedString);
    doAction(recievedJson);
  }
  stepperHa.tick();
  stepperDec.tick();
  // Serial.println(stepper1.getCurrentDeg());
}