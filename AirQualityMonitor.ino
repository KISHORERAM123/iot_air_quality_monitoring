#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#include "MQ135.h"
#include "DHT.h"
int gas, co2lvl;
#define DHTPIN 4 
int LED1 = D1;
int LED2 = D5;
int LED3 = D6;

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "Abi's Nord";
const char* password = "Abi@7177";

WiFiClient client;
unsigned long myChannelNumber = 1784816;
const char* myWriteAPIKey = "EW1NEAPPSKGX9M8A"; 
void setup()
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);

  delay(10);

  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

  dht.begin();
}
 
 
void loop()
{
    MQ135 gasSensor = MQ135(A0);
    float air_quality = gasSensor.getPPM();

    gas = analogRead(A0);
    co2lvl = gas - 120;
    co2lvl = map(co2lvl, 0, 1024, 400, 5000);

    float h = dht.readHumidity();
 
    float t = dht.readTemperature();
   
    float f = dht.readTemperature(true);
  
   
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    


    
    Serial.print("Air Quality: ");  
    Serial.print(air_quality);
    Serial.println("  PPM"); 
    
    if (air_quality <= 50) {
      Serial.println("GOOD");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
    }
    else if (air_quality > 50 and air_quality <= 100) {
      Serial.println("SATISFACTORY");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
    }
    else if (air_quality > 101 and air_quality <= 150) {
      Serial.println("MODERATE");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
    }
    else if (air_quality > 151 and air_quality <= 200) {
      Serial.println("UNLEATHY");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
    }
    else if (air_quality > 201 and air_quality <= 300) {
      Serial.println("VERY UNHEALTHY");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
    }
    else if (air_quality > 301) {
      Serial.println("VERY UNHEALTHY");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
    }


/*
 * Atmospheric CO2 Level..............400ppm
 * Average indoor co2.............350-450ppm
 * Maxiumum acceptable co2...........1000ppm
 * Dangerous co2 levels.............>2000ppm
 */

 
    

    Serial.print("CO2 = ");
    Serial.print(co2lvl);  // prints the value read
    Serial.println(" PPM");
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F"));
    Serial.println(" ");
    Serial.println("................................................ ");

    ThingSpeak.writeField(myChannelNumber, 1, air_quality, myWriteAPIKey);
    delay(100);
    ThingSpeak.writeField(myChannelNumber, 2, co2lvl, myWriteAPIKey);
    delay(100);
    ThingSpeak.writeField(myChannelNumber, 3, h, myWriteAPIKey);
    delay(100);
    ThingSpeak.writeField(myChannelNumber, 4, t, myWriteAPIKey);

    delay(500);
}
