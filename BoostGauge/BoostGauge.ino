#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define sensorPin A0

int OLED_RESET = 4;
Adafruit_SSD1306 display(OLED_RESET); //driver for the screen

float rawval = 0; // Setup raw sensor value
// peak
int boostPeakReset = 10000; // time in milis to reset peak value
float boostPeak = 0.00;
float boostMax = 0.00;
unsigned long boostPeakTimer = 0;

// log
float peakParsed = 0;
byte count;
byte sensorArray[128];
byte drawHeight;
boolean filled = 0; //decide either filled, or dot-display. 0==dot display.

void setup(){
  Serial.begin(9600); // start monitoring raw voltage for calibration
  display.begin(SSD1306_SWITCHCAPVCC); // 3.3V power supply
  display.clearDisplay(); // Clear the display and ram
  display.display();
  delay(2000); // display Adafruit logo for 2 seconds
  for (count = 0; count < 128; count++){ //zero all elements
    sensorArray[count] = 0;
  }
}

void loop(){
  //PEAK RIGHT UPPER CORNER
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("PEAK");
  
  int boostmbar = map(analogRead(sensorPin), 21, 961, 100, 2600);
  rawval = analogRead(0); // Read MAP sensor raw value on analog port 0
  
  if (boostPeak < boostmbar && boostmbar > 0.50) {
    boostPeak = boostmbar;
    boostPeakTimer = millis();
    if (boostMax < boostPeak) {
      boostMax = boostPeak;
    }
  }
  else if (boostPeak > boostmbar && (millis() - boostPeakTimer) > boostPeakReset) {
   boostPeak = 0.00;
  }

  // log 
  drawHeight = map(analogRead(A0), 0, 1023, 0, 25 );  
  sensorArray[0] = drawHeight;
  for(count = 55; count < 128; count++){
    if(filled == false){
      display.drawPixel(count, 71 - sensorArray[count - 55], WHITE);
    }
    else
      display.drawLine(count, 1, count, 71 - sensorArray[count - 55], WHITE); 
  }
  for (count = 80; count >= 2; count--){ // count down from 160 to 2
    sensorArray[count - 1] = sensorArray[count - 2];
  }

  peakParsed = ((boostPeak * 0.001) - 0.865)*14;
  display.setTextSize(1); //Display peak boost
  if(peakParsed < 0){
      display.setCursor(90, 0);
      display.println(peakParsed); // 0.97 = 970mbar atmospheric pressure correction
  }
  else{
    display.setCursor(95, 0);
    display.println(peakParsed); // 0.97 = 970mbar atmospheric pressure correction
  }

  if((((boostmbar * 0.001) - 0.865)*14) > 0){
    display.setTextSize(0.3);
    display.setCursor(109, 25);
    display.println("PSI");
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(18, 12);
    display.println(((boostmbar * 0.001) - 0.865)*14);    // calibrated for a 2.5 bar sensor in Denver (+/- 1psi)
  }
  else if((((boostmbar  * 0.001) - 0.865)*14) < 0){
    display.setTextSize(0.3);
    display.setCursor(109, 25);
    display.println("VAC");
    display.setCursor(0, 12);
    display.setTextSize(3);
    display.println(((boostmbar * 0.001) - 0.865)*63.2);
  }

  delay(1);
  display.display();
  display.clearDisplay();

  Serial.print("Input from A0 is ");
  Serial.println(rawval);    // print the raw value from the sensor (0-100%)
  Serial.println(boostmbar);
  delay(10); // delay half second between numbers
}
