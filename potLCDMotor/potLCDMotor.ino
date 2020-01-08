#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

int pot1 = A0;
int pot2 = A1;

int button1 = 4;
int button2 = 2;

int x, y;

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

  pinMode(talon1Pin, OUTPUT);
  pinMode(talon2Pin, OUTPUT);

  pinMode(13, OUTPUT);

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
  //compute x power and y power %
  //Reads 2x to get rid of noise
  x = analogRead(pot1);
  x = analogRead(pot1);
  y = analogRead(pot2);
  y = analogRead(pot2);
  xPower = toPower(x, digitalRead(button1));
  yPower = linearPower(y, digitalRead(button2));

  Serial.print(x);
  Serial.print("   ");
  Serial.println(y);

  lcd.setCursor(9,0);
  if(!(xPower < 0)){
    lcd.print(" ");
  }
  lcd.print(xPower);
  lcd.print("  ");
  
  lcd.setCursor(9, 1);
  if(!(yPower < 0)){
    lcd.print(" ");
  }
  lcd.print(yPower);
  lcd.print("  ");

  if(digitalRead(button2) == 0){
    yPower = 0;
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }


  xDelayTime = 1490 + xPower*500;
  yDelayTime = 1490 + yPower*500;
  
  digitalWrite(talon1Pin, HIGH);
  delayMicroseconds(xDelayTime);
  digitalWrite(talon1Pin, LOW);

  digitalWrite(talon2Pin, HIGH);
  delayMicroseconds(yDelayTime);
  digitalWrite(talon2Pin, LOW);
}

double toPower(int sensorVal, int forward){
  return ((double)sensorVal) / 1023 * (forward*2-1);
}

double linearPower(int sensorVal, int brake){
//  if(brake == 0){
//    return 0.0;
//  }else{
    return ((double)sensorVal - 512.5)/512.5;
//  }
}
