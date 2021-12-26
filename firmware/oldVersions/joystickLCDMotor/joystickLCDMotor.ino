#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

int JoyStick_X = A2; // x
int JoyStick_Y = A3; // y
int JoyStick_Z = 2; // key

int x, y, z;

double xPower;
double yPower;
int cursorX = 10;
int cursorY = 10;

int talon1Pin = 9;
int talon2Pin = 10;

int xDelayTime;
int yDelayTime;

long lastMicros = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode (JoyStick_X, INPUT);
  pinMode (JoyStick_Y, INPUT);
  pinMode (JoyStick_Z, INPUT);

  pinMode(talon1Pin, OUTPUT);
  pinMode(talon2Pin, OUTPUT);

  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("X: ");
  lcd.setCursor(0, 1);
  lcd.print("Y: ");
}


void loop()
{
  x = analogRead (JoyStick_X);
  y = analogRead (JoyStick_Y);
  z = digitalRead (JoyStick_Z);

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

  

  xDelayTime = 1490 + xPower*500;
  yDelayTime = 1490 + yPower*500;
  
  digitalWrite(talon1Pin, HIGH);
  delayMicroseconds(xDelayTime);
  digitalWrite(talon1Pin, LOW);

  digitalWrite(talon2Pin, HIGH);
  delayMicroseconds(yDelayTime);
  digitalWrite(talon2Pin, LOW);
}
