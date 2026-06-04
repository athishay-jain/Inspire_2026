#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Address (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int mq3Pin = A0;
const int buzzerPin = 9;

int sensorValue;
int alcoholPercent;

void setup()
{
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Alcohol Tester");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  delay(3000);

  lcd.clear();
}

void loop()
{
  sensorValue = analogRead(mq3Pin);

  // Convert sensor reading to percentage
  alcoholPercent = map(sensorValue, 0, 1023, 0, 100);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alcohol Level");

  lcd.setCursor(0, 1);
  lcd.print(alcoholPercent);
  lcd.print("%");

  if (alcoholPercent >60)
  {
    tone(buzzerPin, 1000);   // Buzzer ON

    lcd.setCursor(10, 1);
    lcd.print("HIGH");
  }
  else
  {
    noTone(buzzerPin);       // Buzzer OFF

    lcd.setCursor(10, 1);
    lcd.print("SAFE");
  }

  delay(500);
}
