#include <Arduino.h>
#include <Vector.h>
#include "GyverTimers.h"

//global storage variables
uint32_t pingTimer;
//print/return avalile memory
int availableMemory()
{
  int size = 2048;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL)
    ;
  free(buf);
  Serial.println("RAM left: " + String(size));
  return size;
};

void setup()
{
  //timers setup
  //timer for outputting to serial and status reporting
  Timer2.setPeriod(16384);
  Timer2.enableISR();

  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

ISR(TIMER2_A)
{
  if (millis() - pingTimer >= 2000)
  {
    pingTimer = millis();
    Serial.println("ping " + String(millis()));
  }
}

Vector<String> divideString(String recievedString)
{
  String storage_array[10];
  Vector<String> vector(storage_array);
  vector.push_back("");
  for (int i = 0; i < recievedString.length(); i++)
  {
    if (recievedString[i] == ' ')
    {
      vector.push_back("");
    }
    else
    {
      vector.back() += recievedString[i];
    }
  }
  return vector;
}

void blinkLed(int numberOfBlinks)
{
  for (int i = 0; i < numberOfBlinks; i++)
  {
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
  }
}

//analyzing first letter of string because of switch limitations
//blink [number of blinks] - only for test purposes! Blink led on pin 13, made with delay, so unstable
//rotate [to] - rotate to some point in decimal degrees
//ping - returns pong to serial port
void analyzeString(Vector<String> data)
{
  char firstOption = data[0][0];
  switch (firstOption)
  {
  case 'b':
  {
    blinkLed(data[1].toInt());
  }
  case 'p':
  {
    Serial.println("pong");
  }

  break;

  default:
    break;
  }
}

void loop()
{
  if (Serial.available())
  {
    String s = Serial.readStringUntil('\n');
    String storage_array[10];
    Vector<String> recievedString(storage_array);
    recievedString.push_back("");
    for (int i = 0; i < s.length(); i++)
    {
      if (s[i] == ' ')
      {
        recievedString.push_back("");
      }
      else
      {
        recievedString.back() += s[i];
      }
    }
    analyzeString(recievedString);
  }
}