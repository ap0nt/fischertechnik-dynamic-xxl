#include <Arduino.h>
#include <Servo.h>

#include <WiFi.h>
#include <WebSocketsClient.h>
#include <MQTTPubSubClient.h>

#include "arduino_secrets.h"

const char ssid[] = SECRET_WIFI_SSID;
const char pass[] = SECRET_WIFI_PASSWORD;

int h1 = 12;

int m1 = 4;
int m2 = 5;
int m3_1 = 6;
int m3_2 = 7;
int m4_1 = 8;
int m4_2 = 9;

int led1 = 10;
int led2 = 11;

int tip1 = A0;
int tip2 = A1;
int tip3 = A2;
int tip4 = A3;

Servo Servo1;
int iz1 = 2;

char loopingDirection = 'r';
char twoLineDirection = 'l';
char blockDirection = 'r';

const int speakerPin = 15;  // CE / P15 am 8-poligen Anschluss

// MQTT-WebSocket-Server
const char MQTT_HOST[] = "greater-tribal-catalyst-interaction.trycloudflare.com";
const uint16_t MQTT_PORT = 443;
const char MQTT_PATH[] = "";

// MQTT-Einstellungen
const char MQTT_CLIENT_ID[] = "arduino";
const char MQTT_PUBLISH_TOPIC[] = "marbles";
const char MQTT_SUBSCRIBE_TOPIC[] = "marbles";

WebSocketsClient webSocketClient;
MQTTPubSubClient mqttClient;

// Zeitsteuerung
unsigned long letzteNachricht = 0;
const unsigned long sendeIntervall = 5000;

void verbindeWLAN()
{
  Serial.print("Verbinde mit WLAN: ");
  Serial.println(SECRET_WIFI_SSID);

  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);

  unsigned long startzeit = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);

    if (millis() - startzeit > 20000)
    {
      Serial.println();
      Serial.println("WLAN-Verbindung fehlgeschlagen.");
      return;
    }
  }

  Serial.println();
  Serial.println("WLAN verbunden.");

  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void verbindeMQTT()
{
  if (mqttClient.isConnected())
  {
    return;
  }

  Serial.println("Verbinde MQTT ueber WebSocket ...");

  bool verbunden = mqttClient.connect(
    MQTT_CLIENT_ID,
    SECRET_MQTT_USER,
    SECRET_MQTT_PASSWORD
  );

  if (!verbunden)
  {
    Serial.println("MQTT-Verbindung fehlgeschlagen.");
    return;
  }

  Serial.println("MQTT-Verbindung hergestellt.");

  mqttClient.subscribe(
    MQTT_SUBSCRIBE_TOPIC,
    [](const String& payload, const size_t size)
    {
      Serial.print("Nachricht auf giga/befehl: ");
      Serial.println(payload);

      // Beispiel:
      // Wenn "TON" empfangen wird, kann hier der Speaker
      // eingeschaltet werden.
      if (payload == "TON")
      {
        tone(15, 1000, 500);
      }
    }
  );

  Serial.print("Topic abonniert: ");
  Serial.println(MQTT_SUBSCRIBE_TOPIC);
}

void playSound()
{
  pinMode(speakerPin, OUTPUT);

  tone(speakerPin, 500);   // Ton mit 1000 Hz
  delay(200);
  noTone(speakerPin);
  delay(200);
  tone(speakerPin, 500);   // Ton mit 1000 Hz
  delay(200); 
  tone(speakerPin, 1000);   // Ton mit 1000 Hz
  delay(500);             // eine Sekunde abspielen

  noTone(speakerPin);       // Ton ausschalten
}

void loopingSwitch()
{
  if (loopingDirection == 'r')
  {
    digitalWrite(m3_2, HIGH);
    delay(750);
    digitalWrite(m3_2, LOW);
    loopingDirection = 'l';
  }

  else if (loopingDirection == 'l')
  {
    digitalWrite(m3_1, HIGH);
    delay(750);
    digitalWrite(m3_1, LOW);
    loopingDirection = 'r';
  }
}

void twoLineSwitch()
{
  if (twoLineDirection == 'r')
  {
    digitalWrite(m4_2, HIGH);
    delay(750);
    digitalWrite(m4_2, LOW);
    twoLineDirection = 'l';
  }

  else if (twoLineDirection == 'l')
  {
    digitalWrite(m4_1, HIGH);
    delay(750);
    digitalWrite(m4_1, LOW);
    twoLineDirection = 'r';
  }
}

void blockSwitch()
{
  if (blockDirection == 'r')
  {
    Servo1.write(45);
    blockDirection = 'l';
  }

  else if (blockDirection == 'l')
  {
    Servo1.write(0);
    blockDirection = 'r';
  }

  delay(750);
}

void setup()
{
  //WLAN Verbingung VON HIER
  Serial.begin(115200);

  // Maximal 10 Sekunden auf den seriellen Monitor warten
  unsigned long startzeit = millis();

  while (!Serial && millis() - startzeit < 10000)
  {
    // warten
  }

  Serial.println();
  Serial.println("Arduino GIGA R1 WiFi gestartet.");

  // Prüfen, ob das WLAN-Modul erkannt wird
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Fehler: WLAN-Modul wurde nicht erkannt.");

    while (true)
    {
      delay(1000);
    }
  }

  Serial.print("Verbinde mit WLAN: ");
  Serial.println(ssid);

  int status = WL_IDLE_STATUS;

  // Mehrfach versuchen, das WLAN zu verbinden
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);

    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("WLAN-Verbindung hergestellt!");

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  Serial.print("Signalstaerke: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  //WLAN Verbingung BIS HIER
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3_1, OUTPUT);
  pinMode(m3_2, OUTPUT);

  pinMode(h1, INPUT);

  pinMode(tip1, INPUT);
  pinMode(tip2, INPUT);
  pinMode(tip3, INPUT);
  pinMode(tip4, INPUT);

  pinMode(iz1, OUTPUT);
  Servo1.attach(iz1);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  analogWrite(m2, 45);

  // set loopingSwitch to 'r'
  digitalWrite(m3_1, HIGH);
  delay(750);
  digitalWrite(m3_1, LOW);

  loopingSwitch();

  // set twoLineDirection to 'l'
  digitalWrite(m4_1, HIGH);
  delay(750);
  digitalWrite(m4_1, LOW);

  twoLineSwitch();

  Servo1.write(0);
  delay(750);
  blockSwitch();

  playSound();
}

void loop()
{
  const int tip1State = digitalRead(tip1);
  const int tip2State = digitalRead(tip2);
  const int tip3State = digitalRead(tip3);
  const int tip4State = digitalRead(tip4);

  digitalWrite(m1, tip1State);

  if (tip2State == LOW)
    blockSwitch();
  if (tip3State == LOW)
    twoLineSwitch();
  if (tip4State == LOW)
    loopingSwitch();


  //Serial.println(digitalRead(h1));
}
