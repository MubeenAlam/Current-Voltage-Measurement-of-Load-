#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ACS712.h>
#include <String.h>

String message = ""; 
// string Var to use as a container for output strings 
float voltage = 0;  // Var for Voltage value
float current = 0;  // Var for Current Value
float power = 0;    // Var for Power
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Set the LCD address to 0x27 for a 16 chars and 2 line display

ACS712  ACS(A0, 5.0, 1023, 200);
// Set the Current sensor to pin A0 for output voltage 5V and 
// ADC max value 1023 in 10 bit ADC 200mV / A for our IC of 5A. 
// IC ACS712ELCTR-05B-T

void setup()
{
	// initialize the LCD
	lcd.init();
	// Turn on the blacklight and print a message.
	lcd.backlight();
  // going to home position 0,0
  lcd.home();
  // clear the screen
  lcd.clear();

  // Screen Print.
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F (x positions)
  // I = x x x x x x x V = x x x x x
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F (x positions)
  // P = x x x x x x x x x x x x x x
  
  // there are only 2 y positions 0 and 1
  // 'x' in above shows the avaiable positions. 
  message = "I=0A";
  lcd.print(message);
  lcd.setCursor(9, 0);
  message = "V=0V";
  lcd.print(message);
  lcd.setCursor(0, 1);
  message = "P=0W";
  lcd.print(message);

  // Screen Print.
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F (x positions)
  // I = 0 A           V = 0 V      
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F (x positions)
  // P = 0 W

  // Setting the mid point of Current sennsor.
  // normally mid point is 512.
  // we used trial and error to get more accurate value 528.
  ACS.setMidPoint(512);
}
// char buffer to store the conversion for float to string.
char buffer[5];
void loop()
{
  current = ACS.mA_DC();      // outputs current in mA.
  voltage = analogRead(A1);   // Reading the ADC value from pin A1
  voltage = map(voltage,0,1022,0,12); // converting from ADC level to Voltage
  power = voltage * current/1000; // calculating Power
  lcd.home();
  lcd.setCursor(2, 0);    // Sets the coursor on (x,y) position.
  lcd.print("      ");    // clearing the space to write new data.
  lcd.setCursor(2, 0);

// checking if we need to print in mA or A.
  if(current > 999)
  {
    dtostrf(current/1000, 5, 2, buffer);
    message = String(buffer);
    message = message + "A";
  }
  else if(current <= 999)
  {
    dtostrf(current, 3, 0, buffer);
    message = String(buffer);
    message = message + "mA";
  }

  lcd.print(message);
  lcd.setCursor(11, 0);     // Sets the coursor on (x,y) position.
  lcd.print("      ");      // clearing the space to write new data.
  lcd.setCursor(11, 0);     // Sets the coursor on (x,y) position.
  dtostrf(voltage, 3, 1, buffer);
  message = String(buffer);
  message = message + "V";
  lcd.print(message);
  lcd.setCursor(2, 1);    // Sets the coursor on (x,y) position.
  lcd.print("              ");  // clearing the space to write new data.
  lcd.setCursor(2, 1);    // Sets the coursor on (x,y) position.
  message = String(power) + 'W';
  lcd.print(message);
  delay(1000);
}

