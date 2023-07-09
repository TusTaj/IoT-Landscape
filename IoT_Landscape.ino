#include <SPI.h>
#include <BH1750.h>
#include <math.h>
#include <Wire.h>
#include "SHT31.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
BlynkTimer timer;

#define BLYNK_TEMPLATE_ID " .....  "
#define BLYNK_DEVICE_NAME " ...  "
#define BLYNK_AUTH_TOKEN " ...  "
#define BLYNK_PRINT Serial

#define S1 32
#define S2 35
#define S3 34

#define R1 15
#define R2 2
#define R3 4

char auth[] = " ....  ";
char ssid[] = " ...  ";
char pass[] = " ...  ";

BH1750 lightMeter;
float humidity,temperature,light;
SHT31 sht;

BLYNK_CONNECTED(){
  Blynk.syncAll();
}

void myTimer() {
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V5, light);
}
BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  if (pinValue==0){
    digitalWrite(R1,HIGH);    
  }
  if (pinValue==1){
    digitalWrite(R1,LOW);    
  }  
}
BLYNK_WRITE(V3){
  int pinValue = param.asInt();
  if (pinValue==0){
    digitalWrite(R2,HIGH);    
  }
  if (pinValue==1){
    digitalWrite(R2,LOW);    
  }  
}
BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  if (pinValue==0){
    digitalWrite(R3,HIGH);    
  }
  if (pinValue==1){
    digitalWrite(R3,LOW);    
  }  
}
/*
void with_internet (){
  if(digitalRead(S1)==LOW){
    digitalWrite(R1,LOW);
    Blynk.virtualWrite(V2,0);
  }
  if(digitalRead(S1)==HIGH){
    digitalWrite(R1,HIGH);
    Blynk.virtualWrite(V2,1);
  }
  if(digitalRead(S2)==LOW){
    digitalWrite(R2,LOW);
    Blynk.virtualWrite(V3,0);
  }
  if(digitalRead(S2)==HIGH){
    digitalWrite(R2,HIGH);
    Blynk.virtualWrite(V3,1);
  }
  if(digitalRead(S3)==LOW){
    digitalWrite(R3,LOW);
    Blynk.virtualWrite(V4,0);
  }
  if(digitalRead(S3)==HIGH){
    digitalWrite(R3,HIGH);
    Blynk.virtualWrite(V4,1);
  }
}
void without_internet()
{
  digitalWrite(R1, digitalRead(S1));
  digitalWrite(R2, digitalRead(S2));
  digitalWrite(R3, digitalRead(S3));
}
*/
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  while (!Serial);
  Wire.begin();
  lightMeter.begin();
  sht.begin(0x44);    //Sensor I2C Address
  Wire.setClock(100000);
  uint16_t stat = sht.readStatus();  
  timer.setInterval(1000L, myTimer);  

  lcd.init();                      // initialize the lcd
  lcd.init();
    // Print a message to the LCD.
  lcd.backlight();

  pinMode(S1, INPUT);
  pinMode(R1, OUTPUT); 
  digitalWrite(R1,LOW); 

  pinMode(S2, INPUT); 
  pinMode(R2, OUTPUT); 
  digitalWrite(R2,HIGH);

  pinMode(S3, INPUT);
  pinMode(R3, OUTPUT); 
  digitalWrite(R3,HIGH);
}

void loop() {
  light = lightMeter.readLightLevel();
  sht.read();
  humidity=sht.getHumidity();
  temperature=sht.getTemperature();  
  myTimer(); 

  Serial.print(light);
  Serial.print("--");
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.setCursor(3, 0);
  lcd.print(light);

  Serial.print(temperature);
  Serial.print("--");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.setCursor(3, 1);
  lcd.print(temperature);

  Serial.print(humidity);
  Serial.print("--");
  lcd.setCursor(0, 2);
  lcd.print("H:");
  lcd.setCursor(3, 2);
  lcd.print(humidity);

  delay(2000);  
  Blynk.run();
  timer.run();
}