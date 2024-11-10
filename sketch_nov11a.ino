#include <LiquidCrystal.h>            // Includes the LiquidCrystal library for controlling the LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // Initializes the LCD object with the pins it's connected to
int starttime;                        // Variable to store the initial time when the program starts
int activetime;                       // Variable to store the current active time of the system in seconds
int prevoustime = 0;                  // Keeps track of the previous time value for time incrementing
int hours = 0;                        // Stores current hours in the clock
int mins = 0;                         // Stores current minutes in the clock
int ahours = 0;                       // Stores alarm hours
int amins = 0;                        // Stores alarm minutes

void setup() {
  lcd.begin(16, 2);  // Sets up the LCD with a 16x2 character display
  lcd.clear();       // Clears any existing data on the LCD

  Serial.begin(9600);  // Starts serial communication at 9600 baud rate

  pinMode(13, INPUT);      // Configures pin 13 as input for setting the time
  digitalWrite(13, HIGH);  // Enables internal pull-up resistor on pin 13
  pinMode(11, INPUT);      // Configures pin 11 as input for minute adjustment
  digitalWrite(11, HIGH);  // Enables internal pull-up resistor on pin 11
  pinMode(10, INPUT);      // Configures pin 10 as input for hour adjustment
  digitalWrite(10, HIGH);  // Enables internal pull-up resistor on pin 10

  pinMode(8, INPUT);       // Configures pin 8 as input to enter alarm setting mode
  digitalWrite(8, HIGH);   // Enables internal pull-up resistor on pin 8
  pinMode(A0, OUTPUT);     // Configures pin A0 as output, potentially to power an external component
  digitalWrite(A0, HIGH);  // Sets A0 to HIGH

  pinMode(9, OUTPUT);  // Configures pin 9 as output for alarm tone

  starttime = millis() / 1000;  // Records the start time in seconds since the program started
}

void loop()
{
  // Check if alarm mode button (pin 8) is pressed
  while (digitalRead(8) == LOW) {
    lcd.setCursor(6, 1);  // Positions cursor in the second row of the LCD
    lcd.print("Alarm");   // Displays "Alarm" to indicate alarm setting mode
    lcd.setCursor(6, 0);  // Positions cursor in the first row

    // Increment alarm minutes when button at pin 11 is pressed
    if (digitalRead(11) == LOW) {
      amins++;
    }
    // Increment alarm hours when button at pin 10 is pressed
    else if (digitalRead(10) == LOW) {
      ahours++;
    }

    // Display formatted alarm hours and minutes on the LCD
    lcd.setCursor(6, 0);
    if (ahours < 10) { lcd.print("0"); }  // Leading zero for single-digit hours
    lcd.print(ahours);
    lcd.print(":");
    if (amins < 10) { lcd.print("0"); }  // Leading zero for single-digit minutes
    lcd.print(amins);

    // Reset minutes and hours for correct formatting
    if (amins > 59) {
      ahours++;
      amins = 0;
    }
    if (ahours > 23) { ahours = 0; }

    delay(500);   // Adds delay for debouncing button input
    lcd.clear();  // Clears the display for the next update
  }

  // Check if time setting button (pin 13) is pressed
  if (digitalRead(13) == LOW) {
    lcd.setCursor(5, 1);
    lcd.print("Set Time");  // Displays "Set Time" to indicate time setting mode
    lcd.setCursor(6, 0);
    if (digitalRead(11) == LOW) { mins++; }  // Increment minutes
    else if (digitalRead(10) == LOW) {
      hours++;
    }  // Increment hours
  }

  // Calculate active time and update minutes every 60 seconds
  activetime = (millis() / 1000) - starttime;
  if (prevoustime < (activetime - 59)) {
    mins++;
    prevoustime = activetime;
  }

  // Reset minutes and hours for correct time display
  if (mins > 59) {
    hours++;
    mins = 0;
  }
  if (hours > 23) { hours = 0; }

  // Display current time on LCD
  lcd.setCursor(6, 0);
  if (hours < 10) { lcd.print("0"); }  // Leading zero for single-digit hours
  lcd.print(hours);
  lcd.print(":");
  if (mins < 10) { lcd.print("0"); }  // Leading zero for single-digit minutes
  lcd.print(mins);

  // Activate alarm tone if current time matches alarm time
  if (ahours == hours && amins == mins && amins != 0) {
    tone(9, 1000, 200);  // Plays alarm tone at 1000 Hz for 200 ms on pin 9
    delay(200);          // Wait 200 ms
    noTone(9);           // Stops the tone
    delay(200);          // Wait another 200 ms
  } else {
    delay(300);  // Delay if alarm is not active
  }

  lcd.clear();  // Clears the LCD for the next update

  // Print debug information to the serial monitor
  Serial.println(mins);
  Serial.println(hours);
  Serial.println("");
  Serial.println(amins);
  Serial.println(ahours);
  Serial.println("");
  Serial.println(activetime);
  Serial.println(prevoustime);
  Serial.println(starttime);
  Serial.println("");
}
