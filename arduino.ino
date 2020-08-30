// mengirim json data dari arduino uno ke node mcu
//arduino uno

#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "DHT.h"


#define DHTPIN 2
#define DHTTYPE DHT11

SoftwareSerial s(5,6); //rx tx

DHT dht(DHTPIN, DHTTYPE);

const int sensorRainPin = A0;
const int ledPin = 13;
const int sensorLDR = A1;


bool kondisiAtap = false;
int kondisiLangit = 0;

int nilaiSensorHujan = 0;
float nilaiSensorLDR = 0;
float h = 0;
float t = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  s.begin(9600);
  pinMode(ledPin,OUTPUT);
  pinMode(sensorRainPin,INPUT);
  pinMode(sensorLDR,INPUT);
  dht.begin();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  nilaiSensorHujan = analogRead(sensorRainPin);
  nilaiSensorLDR = analogRead(sensorLDR);
  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.print("Sensor Hujan: ");
  Serial.println(nilaiSensorHujan);
  Serial.print("Sensor LDR: ");
  Serial.println(nilaiSensorLDR);
  
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Kelembapan: ");//kelembaban
    Serial.print(h);
    Serial.print(" %t");
    Serial.print(" Suhu: "); 
    Serial.print(t);
    Serial.println(" *C");
  }
  Serial.println("");

  if (nilaiSensorHujan <= 800){
    digitalWrite (ledPin, HIGH);
    kondisiAtap = false;
    kondisiLangit = 1;
    Serial.println("SEDANG HUJAN !!!!");
    Serial.println(kondisiAtap);
    Serial.println("=====================");
    Serial.println("");
    kirimNodeMCU();
    delay (5000);
    }
  else if (nilaiSensorHujan >= 801 && nilaiSensorLDR > 200 ){
        if (t >= 28 ){
          digitalWrite (ledPin, LOW);
          kondisiAtap = true;
          kondisiLangit = 2;
          Serial.println("MENDUNG SUHU PANAS !!!!");
          Serial.println(kondisiAtap);
          Serial.println("=====================");
          Serial.println("");
          delay (5000);
          kirimNodeMCU();
          loop();
        }
        else {
          digitalWrite (ledPin, HIGH);
          kondisiAtap = false;
          kondisiLangit = 3;
          Serial.println("MENDUNG SUHU DINGIN!!!!");
          Serial.println(kondisiAtap);
          Serial.println("=====================");
          Serial.println("");
          delay (5000);
          kirimNodeMCU();
          loop();
        }
    }
  else {
    digitalWrite (ledPin, LOW);
    kondisiAtap = true;
    kondisiLangit = 4;
    Serial.println("TERANG BENERANG !!!!");
    Serial.println(kondisiAtap);
    Serial.println("=====================");
    Serial.println("");
    kirimNodeMCU();
    delay (5000);
    }
}

void kirimNodeMCU(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["hujan"] = nilaiSensorHujan;
  root["ldr"] = nilaiSensorLDR;
  root["suhu"] = t;
  root["kelembapan"] = h;
  root["atap"] = kondisiAtap;
  root["langit"] = kondisiLangit;

  if(s.available()>0){
    root.printTo(s);
  }
}
