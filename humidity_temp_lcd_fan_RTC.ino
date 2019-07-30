

#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>    //  for temp/humidity sensor
#include <LiquidCrystal.h> //  for LCD
#include <Adafruit_NeoPixel.h>

RTC_DS1307 RTC;

const int RTC_POW_PIN = A3;
const int RTC_GND_PIN = A2;

#define DHT22_PIN 7   // pin 7 for DHT sensor
#define DHTTYPE DHT22 //  specify which DHT sensor is being used
DHT dht(DHT22_PIN, DHTTYPE);    // creates instance of DHT object; constructor for DHT class will be called
#define PIN 13
#define LED_OUTPUT 8

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    // initialize digital output pins for LCD
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);   // setup baud rate for serial monitor 
  pinMode(RTC_POW_PIN, OUTPUT);
  pinMode(RTC_GND_PIN, OUTPUT);
  digitalWrite(RTC_POW_PIN, HIGH);
  digitalWrite(RTC_GND_PIN, LOW);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    //RTC.adjust(DateTime(__DATE__, __TIME__));
   
  } 
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  strip.begin();  // start up LED ring
  strip.show();   // since nothing has been set it initializes LED ring to OFF
  
  Serial.println("DHTxx test!");
  dht.begin();    //    start up DHT
  lcd.begin(16, 2);   // dimensions of LCD screen
 pinMode(LED_OUTPUT, OUTPUT);
}
 

void PixelsON(){
  for(int x=1; x<=24; x++){
    strip.setPixelColor(x, 10,100, 200);
  }
  strip.show();
}
void PixelsOFF(){
  for(int x=1; x<=24; x++){
    strip.setPixelColor(x, 0,0,0);
  }
  strip.show();
}

void lightON(){     // function for writing 5V to light path
  digitalWrite(LED_OUTPUT,HIGH);
  PixelsON();
  //strip.show();
  Serial.println("Light On");
}
void lightOFF(){    //  function for writing 0V to light path
  digitalWrite(LED_OUTPUT,LOW);
  PixelsOFF();
  //strip.show();
  Serial.println("Light Off");
}


void loop(){
  DateTime datetime = RTC.now();
  Serial.print(datetime.hour());
  Serial.print(':');
  Serial.print(datetime.minute());
  Serial.print(':');
  Serial.println(datetime.second());
  if (datetime.hour() > 12L && datetime.hour() <= 24L){   // first 12 hours are lightON()
    lightOFF();
  }
  if (datetime.hour() >= 0L && datetime.hour() <= 12L){   //  if 12 hours have passed since setTime(0) then run lightOFF() then reset time to 0
    lightON();
  }
 
  
  
  delay(2000);
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F\t");

  lcd.setCursor(0,0); 
  lcd.print("Temp");
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(dht.readHumidity());
  lcd.print("%    ");
  
  if (digitalRead(8)==HIGH){
    lcd.setCursor(20,0);
    lcd.print("Light On:");
    lcd.print(datetime.hour());
    lcd.print(':');
    lcd.print(datetime.minute());
    lcd.print(':');
    lcd.print(datetime.second());
    
  }
  if(digitalRead(8)==LOW){
    lcd.setCursor(20,0);
    lcd.print("Light off:");
    lcd.print(datetime.hour());
    lcd.print(':');
    lcd.print(datetime.minute());
    lcd.print(':');
    lcd.print(datetime.second());
  }
  for (int positionCounter = 0; positionCounter < 20; positionCounter++)
  {
    lcd.scrollDisplayLeft();
    delay(150);
  }

}




