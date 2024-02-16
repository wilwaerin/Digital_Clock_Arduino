#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS1307RTC.h>
#include <TimeLib.h>

tmElements_t tm;

LiquidCrystal LCD(7, 6, 5, 4, 3, 2); 

void setalarms(int, int, int );

#define set_save 10
#define incrhour 9
#define incrminute 8
#define buzzer 11
#define redLED 12
#define resetButton 13

int hours = 5, minutes = 0;
void setup()
{
   // Set up button and pin configurations
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(set_save, INPUT_PULLUP);
  pinMode(incrhour, INPUT_PULLUP);
  pinMode(incrminute, INPUT_PULLUP);
  digitalWrite(set_save, HIGH);
  digitalWrite(incrhour, HIGH);
  digitalWrite(incrminute, HIGH);
  digitalWrite(redLED, LOW);
  // Initialize the LCD display
  LCD.begin(16,2);
  LCD.clear();
  delay(1000);
  LCD.setCursor(0,0);
  LCD.print("Digital Clock");
  delay(1000);
  LCD.setCursor(0,1);
  LCD.print("0420 Embedded");
  delay(2000);
}

void loop()
{
  // Update the current time and date
  time_date();
// Check if the reset button is pressed
  if (digitalRead(resetButton) == 0)
{
  hours = 5; 
  minutes = 0; 
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Digital Clock");
  LCD.setCursor(0,1);
  LCD.print("0420 Embedded");
  delay(1000);
}
  // Check if the set/save button is pressed
  if(digitalRead(set_save) == 0 )
  {
    delay(170);
    setalarms(tm.Hour, tm.Minute, tm.Second);
  }
// Check if it's time for the alarm
  else if(hours == tm.Hour && minutes == tm.Minute)
  {
    
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("    Wake UP !");
    LCD.setCursor(0,1);
    LCD.print("    Wake UP !");
    beep();
  }

  else
  {
    digitalWrite(redLED, LOW);
  }

}
// Function to display current time and date on the LCD
void time_date()
{
  if (RTC.read(tm))
  {
    LCD.setCursor(0,0);
    LCD.print("Time= " + (String)tm.Hour + ":" + (String)tm.Minute + ":" + (String)tm.Second + "   ");
    LCD.setCursor(0,1);
    LCD.print("Date= " + (String)tm.Day + "/" + (String)tm.Month + "/" + (String)tmYearToCalendar(tm.Year) + "   ");
  }
  delay(170);
}
// Function to set alarms
void setalarms(int HOURS, int MINUTES, int SECONDS)
{
 // Clear the LCD display and set the cursor to the starting position
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("   SET ALARM   ");
  LCD.setCursor(0,1);

// Infinite loop for setting the alarm time
  while (1)
  {
    // Check if the hour increment button is pressed
    if (digitalRead(incrhour) == 0 && digitalRead(incrminute) == 1 && digitalRead(set_save) == 1 )
    {
      HOURS = HOURS + 1;
      delay(170);
      if (HOURS > 23)
        HOURS = HOURS - 24;
    }
// Check if both hour and minute increment buttons are pressed, and set the alarm
    else if (digitalRead(incrhour) == 1 && digitalRead(incrminute) == 1 && digitalRead(set_save) == 0)
    {
      hours = HOURS;
      minutes = MINUTES;
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("ALARM TIME   ");
      LCD.setCursor(0,1);
      LCD.print("HAS BEEN SENT   ");
      delay(2000);
      break; // Exit the loop
    }
// Check if the minute increment button is pressed
    else if (digitalRead(incrhour) == 1 && digitalRead(incrminute) == 0 && digitalRead(set_save) == 1)
    {
      MINUTES = MINUTES + 1;
      delay(170);
      if (MINUTES > 60)
        MINUTES = MINUTES - 60;
    }
    // Display the current alarm time on the LCD
    LCD.setCursor(4,1);
    LCD.print((String)HOURS + ":" + (String)MINUTES + ":" + (String)SECONDS + "    ");
  }
}
// Function to produce a beep and flash the red LED
void beep()
{
 digitalWrite(buzzer, HIGH);
 delay(100);
 digitalWrite(buzzer, LOW);
 delay(100);
 digitalWrite(redLED, HIGH);
 delay(100);
 digitalWrite(redLED, LOW);
 delay(100);
}
