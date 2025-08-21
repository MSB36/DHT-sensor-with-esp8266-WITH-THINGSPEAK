#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

#define DHTPIN 2          // GPIO2 = D4 on NodeMCU
#define DHTTYPE DHT11     // Use DHT11 or DHT22

const char* ssid = "WIFI NAME";
const char* password = "PASSWORD";

unsigned long myChannelNumber = CHANNEL ID ;  
const char * myWriteAPIKey = "API KEY (WRITE)";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: "); Serial.println(t);
  Serial.print("Humidity: "); Serial.println(h);

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // ThingSpeak needs minimum 15-sec delay
}
