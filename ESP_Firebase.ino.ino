#include <FirebaseESP32.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define FIREBASE_HOST "https://dht11-eea80-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ZYEvLOSJVgN43HDUyD7yUnQKn5H4Omi2HgIeKJ2s"
#define WIFI_SSID "RIO HOUSE Lau 1" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "RioHouseLau1"

#define DHTPIN 32
byte LED_X = 14; 
byte LED_D = 26;
int buzzer = 27;
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
FirebaseData fbdo;
LiquidCrystal_I2C lcd (0x27,16,2);

void setup() {
  pinMode(LED_X, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
   delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  dht.begin();
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void bandau(){
  lcd.clear();
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print(F("Do Am: "));
  lcd.setCursor(0, 1);
  lcd.print(F("Nhiet Do: "));
  lcd.setCursor(7, 0);
  lcd.print(h);
  delay(200);
  lcd.setCursor(12, 0);
  lcd.print("%");
  lcd.setCursor(10, 1);
  lcd.print(t);
  delay(200);
  lcd.setCursor(15, 1);
  lcd.print("C");
  Firebase.setFloat( fbdo,"Nhiet do", t);
  Firebase.setFloat ( fbdo,"Do am", h);

  delay(200);
}
void loop() {
  delay(2000);
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  bandau();
  //Serial.print(F("C"));
 if(h>70){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Do Am Cao");
   digitalWrite(LED_X,HIGH);
   digitalWrite(buzzer,LOW);
   delay(1000);
   digitalWrite(buzzer,LOW);
 }else if(h<70){
   digitalWrite(LED_X,LOW);
   bandau();
    }
 if(t>34){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Nhiet Do Cao");
   digitalWrite(LED_D,HIGH);
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);   
 }else if(t<33){
   digitalWrite(LED_D,LOW);
   bandau();
    }

}
