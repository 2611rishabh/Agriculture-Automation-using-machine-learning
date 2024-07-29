#define BLYNK_TEMPLATE_ID "TMPL3mRCtqAn1"
// #define BLYNK_TEMPLATE_NAME "Agriculture"
#define BLYNK_TEMPLATE_NAME "Agriculture"
#define BLYNK_AUTH_TOKEN "YCKDqOxnlvb2S3IUKRLz6mtgT-xdRpGb"

#define BLYNK_PRINT Serial  

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// BlynkTimer timer; 
char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Tuntuna";
char pass[] = "mainnhiderha";
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define waterLevelPin A0
#define waterPump D5
#define fan D6

int waterLevelThreshold = 500;  // Set your water level threshold here

BlynkTimer timer;
bool Relay1 = 0;
bool Relay2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(waterPump, HIGH);
  digitalWrite(fan, HIGH);
  dht.begin();
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, checkWaterLevel);
  timer.setInterval(2000L, sendDHTData);
}

BLYNK_WRITE(V2) {
  Relay1 = param.asInt();
  digitalWrite(waterPump, Relay1 ? LOW : HIGH);
}

BLYNK_WRITE(V3) {
  Relay2 = param.asInt();
  digitalWrite(fan, Relay2 ? LOW : HIGH);
}

void checkWaterLevel() {
  float waterLevel = analogRead(waterLevelPin);
  Serial.print(waterLevel);
  Serial.print("waterLevel");
   Blynk.virtualWrite(V4, waterLevel);
  if (waterLevel > waterLevelThreshold) {
    Blynk.virtualWrite(V2, HIGH);  // Turn on water pump if water level is low
  } else {
    Blynk.virtualWrite(V2, LOW);   // Turn off water pump if water level is normal
  }
}

void sendDHTData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else{
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.print(t);
    Serial.println("°C");
  }
  Blynk.virtualWrite(V0, t);  // Send temperature to V9 in Blynk
  Blynk.virtualWrite(V1, h); // Send humidity to V10 in Blynk
    if (t > 31) {
    Blynk.virtualWrite(V3, HIGH);  // Turn on water pump if water level is low
  } else {
    Blynk.virtualWrite(V3, LOW);   // Turn off water pump if water level is normal
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
