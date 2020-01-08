#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define talon1 6
#define talon2 9
#define talon3 10
#define talon4 11

#define dir1 0
#define dir2 2
#define dir3 4
#define dir4 7

#define ena1 1
#define ena2 3
#define ena3 5
#define ena4 8

#define pot1 A0
#define pot2 A1
#define pot3 A2
#define pot4 A3

#define dirMaster 12
#define enaMaster 13

int a;
int b;
int c;
int d;

int master;

double aPower;
double bPower;
double cPower;
double dPower;

double masterPower;

int aDelay;
int bDelay;
int cDelay;
int dDelay;

byte disableChar[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B10101,
  B01110,
  B00000,
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

unsigned long time1;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

  pinMode(talon1, OUTPUT);
  pinMode(talon2, OUTPUT);
  pinMode(talon3, OUTPUT);
  pinMode(talon4, OUTPUT);

  pinMode(dir1, INPUT);
  pinMode(dir2, INPUT);
  pinMode(dir3, INPUT);
  pinMode(dir4, INPUT);

  pinMode(ena1, INPUT);
  pinMode(ena2, INPUT);
  pinMode(ena3, INPUT);
  pinMode(ena4, INPUT);

  pinMode(dirMaster, INPUT);
  pinMode(enaMaster, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, disableChar);
  
  lcd.setCursor(0, 0);
  lcd.print("A: ");
  lcd.setCursor(8, 0);
  lcd.print("C: ");
  lcd.setCursor(0, 1);
  lcd.print("B: ");
  lcd.setCursor(8, 1);
  lcd.print("D: ");
}

void loop() {
  //Serial.println(digitalRead(enaMaster));
  
  if(digitalRead(enaMaster) == 0){
    //Makes value negative if dir pin triggered
    master = readPot(pot1) * -1 * (digitalRead(dirMaster)*2-1);

    aPower = toPower(master, digitalRead(dir1));
    bPower = toPower(master, digitalRead(dir2));
    cPower = toPower(master, digitalRead(dir3));
    dPower = toPower(master, digitalRead(dir4));
  }else{
    a = readPot(pot1);
    b = readPot(pot2);
    c = readPot(pot3);
    d = readPot(pot4);
  
    aPower = toPower(a, digitalRead(dir1));
    bPower = toPower(b, digitalRead(dir2));
    cPower = toPower(c, digitalRead(dir3));
    dPower = toPower(d, digitalRead(dir4));
  }
    
  printPower(3, 0, aPower, digitalRead(ena1));
  printPower(3, 1, bPower, digitalRead(ena2));
  printPower(11, 0, cPower, digitalRead(ena3));
  printPower(11, 1, dPower, digitalRead(ena4));

  aPower = (digitalRead(ena1) == 0) ? aPower : 0;
  bPower = (digitalRead(ena2) == 0) ? bPower : 0;
  cPower = (digitalRead(ena3) == 0) ? cPower : 0;
  dPower = (digitalRead(ena4) == 0) ? dPower : 0;

  aDelay = 1490 + aPower*500;
  bDelay = 1490 + bPower*500;
  cDelay = 1490 + cPower*500;
  dDelay = 1490 + dPower*500;

  
  pwmPulse(talon1, aDelay);
  pwmPulse(talon2, bDelay);
  pwmPulse(talon3, cDelay);
  pwmPulse(talon4, dDelay);
}

void printPower(int x, int y, double power, int ena){
  lcd.setCursor(x - 1,y);

  if(ena == 0){
    lcd.print(" ");
  }else{
    lcd.write(0);
  }
  
  if(power >= 0){
    lcd.print(" ");
  }
  lcd.print(power);
}

void pwmPulse(unsigned int pin, int time1){
  digitalWrite(pin, HIGH);
  delayMicroseconds(time1);
  digitalWrite(pin, LOW);
}

int readPot(int pin){
  analogRead(pin);
  return analogRead(pin);
}

double toPower(int sensorVal, int forward){
    return ((double)sensorVal) / 1023 * (forward*2-1) * -1.0;
}
