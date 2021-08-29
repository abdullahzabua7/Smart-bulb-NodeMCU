#include<Adafruit_MQTT_Client.h>
#include<Adafruit_MQTT.h>
#include<ESP8266WiFi.h>

#define wifi_name "YOUR SSID NAME"
#define pswd "SSID PASSWORD"
#define server "io.adafruit.com"
#define port 1883
#define username "YOUR USERNAME"
#define aiokey "YOUR AIO KEY"

int trig = 0;
int led = D7;
WiFiClient me;

Adafruit_MQTT_Client mqtt (&me, server, port, username, aiokey);

Adafruit_MQTT_Subscribe feed = Adafruit_MQTT_Subscribe(&mqtt, username"/feeds/IFTTT");
void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  delay (10);
  Serial.println("Connecting to ");
  Serial.print(wifi_name);
  WiFi.begin(wifi_name, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("IP Address is :");
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&feed);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}


void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(mqtt.readSubscription(500));
  //Serial.print("HElloo");
   MQTT_connect();
  if (mqtt.readSubscription(5000)) {
    Serial.print("Got: ");
    Serial.print((char *)feed.lastread);
    int trig = atoi((char *)feed.lastread);
    Serial.print("trig: ");
    Serial.println(trig);
    if (trig == 1) {
      digitalWrite(D7, LOW);
      Serial.println("OFF");
    }
    else if (trig == 0) {
      digitalWrite(D7, HIGH);
      Serial.println("ON");
    }
    //delay(2000);
  }
}
