#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd1(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

int JoyStick_X = A2; // x
int JoyStick_Y = A3; // y
int JoyStick_Z = 2; // key

int x, y, z;

double xPower;
double yPower;
int cursorX = 10;
int cursorY = 10;

void setup()
{
  Serial.begin(9600);
  
  pinMode (JoyStick_X, INPUT);
  pinMode (JoyStick_Y, INPUT);
  pinMode (JoyStick_Z, INPUT);

  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("X: ");
  lcd.setCursor(0, 1);
  lcd.print("Y: ");

  lcd1.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd1.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print("Z: ");
}


void loop()
{
  x = analogRead (JoyStick_X);
  y = analogRead (JoyStick_Y);
  z = digitalRead (JoyStick_Z);

  
  
  lcd.setCursor(3, 0);
  lcd.print(x);
  lcd.print("   ");
  lcd.setCursor(3, 1);
  lcd.print(y);
  lcd.print("   ");
  lcd1.setCursor(3, 0);
  lcd1.print(z + "    ");

  //compute x power and y power %
  if(abs(x - 510) <= 10){
    xPower = 0.0;
  }else{
    xPower = (x-512)/512.0;
  }
  
  if(abs(y - 510) <= 10){
    yPower = 0.00;
  }else{
    yPower = (y-512)/512.0;
  }

  //Move number over to make sapce for negative sign
  if(xPower < 0){
    cursorX = 9;
  }else{
    cursorX = 10;
  }

  if(yPower < 0){
    cursorY = 9;
  }else{
    cursorY = 10;
  }

  lcd.setCursor(cursorX,0);
  lcd.print(xPower);
  lcd.print("  ");
  lcd.setCursor(cursorY, 1);
  lcd.print(yPower);
  lcd.print("  ");
  
  delay(250);
}
