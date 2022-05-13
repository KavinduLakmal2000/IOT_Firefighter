#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);

#define MQ2pin (0)
const int PULSE_SENSOR_PIN = 1; 

float sensorValue; 
int Signal;              
int Threshold = 550;
const int smokePin = 5;
const int aroundTemp = 6;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  pinMode(smokePin,OUTPUT);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("FireFighter");
  display.setCursor(10,28);
  display.println("DashBoard");
  display.display();
  delay(2000);
  
  display.clearDisplay();


}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("DashBoard");
  display.display();
 

  sensorValue = analogRead(MQ2pin); 

   if(sensorValue > 300)
  {
    digitalWrite(smokePin,HIGH);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println("Smoke - detected!");
  display.display();
  display.clearDisplay();
  }
  else{
    digitalWrite(smokePin,LOW);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println("Smoke - Normal");
  display.display();
  display.clearDisplay();
  }

  Signal = analogRead(PULSE_SENSOR_PIN);
  Serial.write(Signal);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("BPM");
  display.setCursor(10,30);
  display.println(Signal);
  display.display();

  if (digitalRead(aroundTemp)==HIGH){
  digitalWrite(smokePin,LOW);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15,0);
  display.println("Temp over 50");
  display.display();
  display.clearDisplay();
  }
  else{
  digitalWrite(smokePin,LOW);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15,0);
  display.println("Temp under 50");
  display.display();
  display.clearDisplay();
  }
  
 delay(2000);
}
