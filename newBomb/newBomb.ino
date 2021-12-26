/* Firmware for "The Bomb" (GRT - FRC 192)
 * Made in 2020-21
 * 
 * Binh-Minh Nguyen
 * binhminh292@gmail.com 
 * 
 * I'm commenting this like a year later sorry if it's unclear
 * 
 * Control board (PCB) supports 5 inputs + 1 master, code currently has 4 inputs + 1 master
 * 
 * pot = slider pin
 * en = enable pin
 * rev = reverse pin
 */

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// pattern for the disable character (circle with x)
byte disableChar[] = {
  0b00000,
  0b01110,
  0b10101,
  0b11011,
  0b10101,
  0b01110,
  0b00000,
  0x00
};


//Declare input pins
//Includes master input, master is stored as first value in each array
//I really should have created an inputModule class, but I'm lazy
#define numInputs 5

byte pot[] = {A6, A7, A0, A1, A2};
byte en[] = {11, 13, 6, 8, 9};
byte rev[] = {12, 5, 7, 10, 4};

int potData[numInputs];
int enData[numInputs];
int revData[numInputs];

//Positions for A,B,C, and D outputs on the LCD
int displayOffset[][2] = {{0,0},{8,0},{0,1},{8,1}};

//Scaled output for each channel
long percentOutput[numInputs];

void setup() {
  // Begin Serial connection w/ computer if connected
  Serial.begin(9600);

  //Initialize LCD
  int status;

  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }

  // Save disable character to slot 0
  lcd.createChar(0, disableChar);

  //initialize PCA9685 driver IC (I2C to PWM chip), see datasheet for what these registers are
  //Enable sleep mode to change PRE_SCALE (determines pwm frequency)
  writeRegister(0x60, 0x00, 0x11);
  //Change PRE_SCALE to 7Dh (20ms period/50hz frequency)
  writeRegister(0x60, 0xFE, 0x7B);
  //Disable sleep mode/turn the internal osciallator on
  writeRegister(0x60, 0x00, 0x01);

  // Initialize input pins
  for(int i = 0; i < numInputs; i++){
    pinMode(pot[i], INPUT);
    pinMode(en[i], INPUT);
    pinMode(rev[i], INPUT);
  }

  // Print the channel names to the LCD
  lcd.clear();
  lcd.setCursor(displayOffset[0][0], displayOffset[0][1]);
  lcd.write("A:");
  lcd.setCursor(displayOffset[1][0], displayOffset[1][1]);
  lcd.write("B:");
  lcd.setCursor(displayOffset[2][0], displayOffset[2][1]);
  lcd.write("C:");
  lcd.setCursor(displayOffset[3][0], displayOffset[3][1]);
  lcd.write("D:");
}

void loop() {
  //Updates input values (get slider and button positions)
  for(int i = 0; i < numInputs; i++){
    potData[i] = analogRead(pot[i]);
    enData[i] = digitalRead(en[i]);
    revData[i] = digitalRead(rev[i]);

    //Old Debugging prints
//    Serial.print(potData[i]);
//    Serial.print(" ");
//    Serial.print(enData[i]);
//    Serial.print(" ");
//    Serial.println(revData[i]);
  }

  
  for(byte i = 1; i < numInputs; i++){
    lcd.setCursor(displayOffset[i-1][0] + 2, displayOffset[i-1][1]);
    
    //EN active Low
    if(enData[i] == HIGH){
      //write disable symbol
      lcd.write(0);
    }else{
      //Clear disable symbol
      lcd.write(" ");
    }

    //If under master control, read master potentiometer, else read normal potentiometer
    if(enData[0] == LOW){
      //Deadband at 1021 (Assume 1021 or higher is full ouput)
      percentOutput[i] = (potData[0] > 1015) ? 100 : (long) potData[0] * 100 / 1023;

      revData[i] = revData[i] ^ revData[0];
      Serial.print("master: ");
    }else{
      //Deadband at 1021 (Assume 1021 or higher is full ouput)
      percentOutput[i] = (potData[i] > 1015) ? 100 : (long) potData[i] * 100 / 1023;
      Serial.print("Regular: ");
    }

    
    //REV active HIGH
    if(revData[i] == HIGH){
      lcd.write("-");
    }else{
      lcd.write(" ");
    }

    //Pad whitespace for 1 and 2 digit numbers
    if(percentOutput[i] < 100){
      lcd.print(" ");
      if(percentOutput[i] < 10){
        lcd.print(" ");
      }
    }

    //Update percentOutput based on enable signal and slider position
    if(enData[i] == LOW){
      setPower(i-1, (int) percentOutput[i] * (revData[i] ? -1 : 1));
      Serial.print(" ");
    }else{
      setPower(i-1, 0);
    }
    lcd.print(String((int) percentOutput[i]));
    Serial.println(String((int) percentOutput[i]));
  }

  Serial.println();
//  delay(500);
}

//Unused method, was only used to figure out communication with the PCA9685 chip
byte readRegister(byte devAddr, byte regAddr){  
  Wire.beginTransmission(devAddr);
  Wire.write(regAddr);              // sends one byte
  Wire.endTransmission();    // stop transmitting


  Wire.requestFrom(devAddr, 1);
  byte data = Wire.read();
  return data;
}

//Unused method, was only used to figure out communication with the PCA9685 chip
void printRegister(byte devAddr, byte regAddr){
  Serial.print(devAddr, HEX);
  Serial.print(" ");
  Serial.print(regAddr, HEX);
  Serial.print(": ");
  Serial.println(readRegister(devAddr, regAddr), HEX);
}

// Tells PCA9685 at I2C address devAddr to write data to its register at address regAddr (see datasheet)
byte writeRegister(byte devAddr, byte regAddr, byte data){
  Wire.beginTransmission(devAddr);
  Wire.write(regAddr);
  Wire.write(data);
  Wire.endTransmission();
}

// Tells PCA9685 to set a channel's output
// See schematic to see which channel (letter) corresponds to which channel (pin #) 
// Basically go read the datasheet to understand what this is doing
void setPower(byte channel, int percentOutput){
  // Scales percentOutput [-100 to 100] to PCA9685's counter
  int output = 0x133 + 0x67 * percentOutput/100;
  
  //Set on time to 0
  writeRegister(0x60, 0x06 + 4*channel, 0);
  writeRegister(0x60, 0x07 + 4*channel, 0);
  //Set off time to 133h (307d) 1.5 ms pulse width
  writeRegister(0x60, 0x08 + 4*channel, output & 0xff);
  writeRegister(0x60, 0x09 + 4*channel, (output >> 8) & 0xff);

  Serial.println(output & 0xff, HEX);
  Serial.println((output >> 8) & 0xff, HEX);
}
