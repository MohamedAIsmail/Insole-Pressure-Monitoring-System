#define sensorpin A0 // analog pin0 , the sensor output is connected here.
#include <LiquidCrystal_I2C.h>
#include "QuickStats.h"
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
int counter;
const int numReadings=50;
float forceVals[numReadings];
unsigned int i;
float v_meas; //measured voltage on the sensor
float force_gram; //measured force in gram
float r_meas; //measured resistance 
int analogReading;
int max_force;
float r0; //resistance used in series with the FSR in Kohms.
int iterations;
QuickStats stats; //initialize an instance of the statistics class


void setup() {
  //Initializing the LCD
  lcd.init();
  lcd.clear();         
  lcd.backlight();      
  //Moving the cursor to the desired position and printing a welcome message.
  lcd.setCursor(0, 0);
  lcd.print("FORCE DETECTOR");
  lcd.setCursor(0, 1);
  lcd.print("SBME24 Team 13");
  delay(3000);
  lcd.clear();
  r0=20; //we're using a 10 Kohms resistor, you might need to change this resistor to fit your application.
  i=0; //set the counter for the array of stored values.
}

void loop() {
  //get the sensor reading and subtract it from 1023 to have it correlate with the force directly.
  analogReading= 1023 - analogRead(sensorpin); //this is used for debug purposes only.

  //calculating the force on the sensor in grams (then kg).
  v_meas=(analogRead(sensorpin)/1023.0)*5.0; //convert this reading into voltage (using a 5v source.)
  r_meas=v_meas*r0/(5-v_meas);//calculate the resistance of the FSR (using voltage divider.)
  force_gram=6193.22-755.52*r_meas; //calculate the force on the sensor in grams, div by 10^3 to get value in kg.
  
  //this block prints out the current force on the first line.
  lcd.setCursor(0,0);
  lcd.print("Force:");
  lcd.setCursor(9,0);
  if(force_gram>0){
      lcd.print((int)force_gram);
  }else{
      lcd.print(0);
  }
//  lcd.print(v_meas);

//debug messages.
  Serial.print(r_meas);
  Serial.println(v_meas);

  //add the reading to the array.
  forceVals[i] = force_gram;
  i++;
  if (i>=numReadings)
  {
    i=0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
  }else{
    //this function just gets the max value from the array, if you find a better solution you can get rid of 
    //the Quickstats.h library.
    max_force=stats.maximum(forceVals,i);
  }
  
  //print the max force on the bottom line.
  lcd.setCursor(0,1);
  lcd.print("MaxForce:");
  lcd.setCursor(12,1);
  if(max_force>0){
      lcd.print((int)max_force);
  }else{
      lcd.print(0);
  }

  delay(500);
  lcd.clear();
}
