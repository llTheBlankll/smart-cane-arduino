#include <Arduino.h>
#include <neotimer.h>

#define BUZZER_PIN 8
#define USONIC_TRIGGER_PIN 10
#define USONIC_ECHO_PIN 11

Neotimer timer = Neotimer(100);

// Define the number of readings to take for averaging
const int numberOfReadings = 100;
double readings[numberOfReadings]; // Readings from the analog input
int readIndex = 0;                 // the index of the current reading
double total = 0;                  // the running total
double average = 0;                // the average

long randNumber;

void setup()
{
  // put your setup code here, to run once:
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(USONIC_TRIGGER_PIN, OUTPUT);
  pinMode(USONIC_ECHO_PIN, INPUT);

  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void checkDistanceAndBeep(int distance)
{
  Serial.println("Distance: " + String(distance));

  int delayTime = map(distance, 0, 45, 50, 300);
  const int duration = map(distance, 5, 45, 0, 200);

  // Beep if distance is less than 10 cm, using Tone library
  if (distance <= 15)
  {
    tone(BUZZER_PIN, 2300, duration);
  }
  else if (distance <= 21)
  {
    tone(BUZZER_PIN, 2400, duration);
  }
  else if (distance <= 30)
  {
    tone(BUZZER_PIN, 2500, duration);
  }
  else if (distance <= 40)
  {
    tone(BUZZER_PIN, 2700, duration);
  }
  else if (distance <= 60)
  {
    tone(BUZZER_PIN, 3000, duration);
  } else {
    noTone(BUZZER_PIN);
  }

  if (delayTime > 0) {
    timer.set(delayTime);
  }
}

void loop()
{
  total -= readings[readIndex];

  digitalWrite(USONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(USONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(USONIC_TRIGGER_PIN, LOW);

  const auto duration = static_cast<double>(pulseIn(USONIC_ECHO_PIN, HIGH));

  readings[readIndex] = duration * 0.034 / 2;

  // Add the reading to the total
  total = total + readings[readIndex];

  // Advance to the next position in the array
  readIndex += 1;

  // If we're at the end of the array, wrap around to the beginning
  if (readIndex >= numberOfReadings)
  {
    readIndex = 0;
  }

  // Calculate the average
  average = total / numberOfReadings;

  if (timer.repeat())
  {
    checkDistanceAndBeep(average);
  }
}
