#include <LiquidCrystal.h>

// Standard 16x2 LCD connections from your original code
// RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 13, 4, 3, 2);

// Four potentiometers simulate crowd density in four zones
#define ENTRANCE A0
#define WAITING  A1
#define STAIRCASE A2
#define BOARDING A3

// LED and buzzer connections
#define GREEN_LED 6
#define RED_LED 5
#define YELLOW_LED 10
#define BUZZER 7

// Platform capacity for demonstration
const int PLATFORM_CAPACITY = 500;

// Crowd density thresholds (demonstration only)
const int MODERATE_LIMIT = 40;
const int HIGH_LIMIT = 65;
const int CRITICAL_LIMIT = 85;

// Zone crowd densities
int entranceDensity;
int waitingDensity;
int staircaseDensity;
int boardingDensity;

int maxDensity;
int averageDensity;

// Simulated passenger flow rates
int arrivalRate;
int departureRate;

// LCD screen control
int screenNumber = 0;
unsigned long lastScreenChange = 0;
const unsigned long SCREEN_INTERVAL = 2500;

// Serial Monitor control
unsigned long lastSerialUpdate = 0;
const unsigned long SERIAL_INTERVAL = 2000;

void setup() {
  Serial.begin(9600);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("RAILWAY CROWD");
  lcd.setCursor(0, 1);
  lcd.print("MANAGEMENT");
  delay(2000);

  lcd.clear();

  Serial.println("Railway Platform Crowd Management");
  Serial.println("---------------------------------");
}

void loop() {
  // Read the four zone inputs and convert them to percentages
  entranceDensity = map(analogRead(ENTRANCE), 0, 1023, 0, 100);
  waitingDensity = map(analogRead(WAITING), 0, 1023, 0, 100);
  staircaseDensity = map(analogRead(STAIRCASE), 0, 1023, 0, 100);
  boardingDensity = map(analogRead(BOARDING), 0, 1023, 0, 100);

  // Find the highest crowd density
  maxDensity = max(
    max(entranceDensity, waitingDensity),
    max(staircaseDensity, boardingDensity)
  );

  // Calculate average zone density
  averageDensity = (
    entranceDensity +
    waitingDensity +
    staircaseDensity +
    boardingDensity
  ) / 4;

  // Simulated flow estimates, not actual passenger counts
  arrivalRate = map(entranceDensity, 0, 100, 0, 60);
  departureRate = map(boardingDensity, 0, 100, 0, 60);

  // Update crowd warning LEDs and buzzer
  if (maxDensity < MODERATE_LIMIT) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  }
  else if (maxDensity < HIGH_LIMIT) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  }
  else if (maxDensity < CRITICAL_LIMIT) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    noTone(BUZZER);
  }
  else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 1000);
  }

  // Rotate LCD information screens
  unsigned long currentTime = millis();

  if (currentTime - lastScreenChange >= SCREEN_INTERVAL) {
    lastScreenChange = currentTime;
    lcd.clear();

    switch (screenNumber) {

      case 0:
        lcd.setCursor(0, 0);
        lcd.print("Arrival Rate:");
        lcd.setCursor(0, 1);
        lcd.print(arrivalRate);
        lcd.print(" pass/min");
        break;

      case 1:
        lcd.setCursor(0, 0);
        lcd.print("Departure Rate:");
        lcd.setCursor(0, 1);
        lcd.print(departureRate);
        lcd.print(" pass/min");
        break;

      case 2:
        lcd.setCursor(0, 0);
        lcd.print("Train: Express");
        lcd.setCursor(0, 1);
        lcd.print("Arrival: 10:30AM");
        break;

      case 3:
        lcd.setCursor(0, 0);
        lcd.print("Platform: 1");
        lcd.setCursor(0, 1);
        lcd.print("Capacity:");
        lcd.print(averageDensity);
        lcd.print("%");
        break;

      case 4:
        lcd.setCursor(0, 0);
        lcd.print("Max Crowd:");
        lcd.print(maxDensity);
        lcd.print("%");

        lcd.setCursor(0, 1);

        if (maxDensity < MODERATE_LIMIT) {
          lcd.print("Status: LOW");
        }
        else if (maxDensity < HIGH_LIMIT) {
          lcd.print("Status: MODERATE");
        }
        else if (maxDensity < CRITICAL_LIMIT) {
          lcd.print("Status: HIGH");
        }
        else {
          lcd.print("Status: CRITICAL");
        }
        break;

      case 5:
        lcd.setCursor(0, 0);
        lcd.print("Entrance:");
        lcd.print(entranceDensity);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("Waiting:");
        lcd.print(waitingDensity);
        lcd.print("%");
        break;

      case 6:
        lcd.setCursor(0, 0);
        lcd.print("Stairs:");
        lcd.print(staircaseDensity);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("Boarding:");
        lcd.print(boardingDensity);
        lcd.print("%");
        break;
    }

    screenNumber++;

    if (screenNumber > 6) {
      screenNumber = 0;
    }
  }

  // Print crowd report to Serial Monitor
  if (currentTime - lastSerialUpdate >= SERIAL_INTERVAL) {
    lastSerialUpdate = currentTime;

    Serial.println("\n--- CROWD REPORT ---");

    Serial.print("Entrance: ");
    Serial.print(entranceDensity);
    Serial.println("%");

    Serial.print("Waiting Area: ");
    Serial.print(waitingDensity);
    Serial.println("%");

    Serial.print("Staircase: ");
    Serial.print(staircaseDensity);
    Serial.println("%");

    Serial.print("Boarding Area: ");
    Serial.print(boardingDensity);
    Serial.println("%");

    Serial.print("Estimated arrival rate: ");
    Serial.print(arrivalRate);
    Serial.println(" passengers/min");

    Serial.print("Estimated departure rate: ");
    Serial.print(departureRate);
    Serial.println(" passengers/min");

    Serial.print("Average zone density: ");
    Serial.print(averageDensity);
    Serial.println("%");

    Serial.print("Highest zone density: ");
    Serial.print(maxDensity);
    Serial.println("%");

    Serial.println("--------------------");
  }

  delay(50);
}

