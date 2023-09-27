#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SECRET_CH_ID 2109045   // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "2EWLZT3O25G6D5PM" 
#define WIFI_SSID "RIO HOUSE Lau 1" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "RioHouseLau1"
WiFiClient  client;
float A1_value, A2_value;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
#define DHTPIN 32
byte LED_X = 14; 
byte LED_D = 26;
int buzzer = 27;
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
unsigned long dataMillis = 0;
LiquidCrystal_I2C lcd (0x27,16,2);

void setup() {
  pinMode(LED_X, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
   delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  dht.begin();
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  ThingSpeak.begin(client); 
  
}

void bandau(){
  lcd.clear();
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  //////////////////////////////
  int D1_value = digitalRead(LED_D);
  int D2_value = digitalRead(LED_X);
  int D3_value = digitalRead(buzzer);
  float A2_value = dht.readHumidity();
  float A1_value = dht.readTemperature();
  ThingSpeak.setField(1, A1_value);
  ThingSpeak.setField(2, A2_value);
  ThingSpeak.setField(3, D1_value);
  ThingSpeak.setField(4, D2_value);
  ThingSpeak.setField(5, !D3_value);
  if (millis() - dataMillis > 20000)
  {
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }  
   }
  lcd.setCursor(0, 0);
  lcd.print(F("Do Am: "));
  lcd.setCursor(0, 1);
  lcd.print(F("Nhiet Do: "));
  lcd.setCursor(7, 0);
  lcd.print(h);
  lcd.setCursor(12, 0);
  lcd.print("%");
  lcd.setCursor(10, 1);
  lcd.print(t);
  lcd.setCursor(15, 1);
  lcd.print("C");

  

  delay(200);
}
void loop() {
  delay(2000);
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  bandau();
  //Serial.print(F("C"));
 if(h>50){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Do Am Cao");
   digitalWrite(LED_X,HIGH);
   digitalWrite(buzzer,LOW);
   delay(1000);
   digitalWrite(buzzer,LOW);
 }else if(h<50){
   digitalWrite(LED_X,LOW);
   bandau();
    }
 if(t>35){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Nhiet Do Cao");
   digitalWrite(LED_D,HIGH);
   digitalWrite(buzzer,HIGH);
   delay(1000);
   digitalWrite(buzzer,LOW);   
 }else if(t<35){
   digitalWrite(LED_D,LOW);
   bandau();
    }

}