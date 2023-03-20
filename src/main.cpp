#include <Arduino.h>
#include <SPI.h>
#include "mqtt.hpp"
#include "secrets.h"

const int pinCapteurMouvement = 6;
const int pinTransistor = 7;
const int pinLED = 2;
const int pinCapteurTemperature = A5;

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

const char BROKER[] = BROKER_IP;
const int port = BROKER_PORT;

int valeurCapteurTemperature = 0;
int valeurCapteurMouvement = 0;
int intensiteLED = 0;
double temperature = 0;
int valeurRPC ;


void processReceivedMessage() {
    // Vider la chaîne de caractères avant de recevoir le message
  String valeurRPCString = "";

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // Réception d'un message, affichage du sujet et du contenu
    Serial.print("Message reçu sur le topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', longueur ");
    Serial.print(messageSize);
    Serial.println(" octets :");

    // Affichage du contenu du message
    while (mqttClient.available()) {
      // lire un caractère dans le message MQTT
      char c = mqttClient.read();
      //Serial.print(c);
      valeurRPCString += c;

      // afficher la valeur entière
    }
        // convertir la chaîne de caractères en entier
    int valeurRPC = valeurRPCString.toInt();
    Serial.print("VALEUR RPC APRES CONVERSION = ");
    Serial.println(valeurRPC);
    
      // traiter la valeur RPC
    if (valeurRPC == 0) {
      Serial.print("LED = ");
      Serial.println(valeurRPC);
      digitalWrite(pinTransistor, LOW);
    } else if (valeurRPC == 1) {
      Serial.print("LED = ");
      Serial.println(valeurRPC);
      digitalWrite(pinTransistor, HIGH);
    } else {   
      // effectuer des actions en conséquence
      intensiteLED = valeurRPC;
      Serial.print("Knob = ");
      Serial.println(intensiteLED);
    }
      Serial.print("RPC Value = ");
      Serial.println(valeurRPC);
  }
}

void activerTransistor(int valeurRPC) {
  if (valeurRPC > 0) {
    digitalWrite(pinTransistor, HIGH);  // mettre la broche en niveau haut
  } else {
    digitalWrite(pinTransistor, LOW);  // mettre la broche en niveau bas
  }
}

void setup() {
  // Initialisation de la communication série à 9600 bps :
  Serial.begin(9600);

  // Tentative de connexion au réseau Wi-Fi :
  Serial.print("Tentative de connexion au réseau Wi-Fi WPA : ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // Échec, réessayer
    Serial.print(".");
    delay(5000);
  }

  Serial.println("Connexion au réseau Wi-Fi réussie !");
  Serial.println();

  // Connexion au courtier MQTT :
  Serial.print("Tentative de connexion au courtier MQTT : ");
  Serial.println(BROKER);

  if (!mqttClient.connect(BROKER, port)) {
    Serial.print("Échec de la connexion au courtier MQTT ! Code d'erreur = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("Connexion au courtier MQTT réussie !");
  Serial.println();

  // Abonnement à un sujet :
  Serial.print("Abonnement au topic : ");
  Serial.println(commandTopic);
  Serial.println();

  mqttClient.subscribe(commandTopic);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinTransistor, OUTPUT);
  pinMode(pinCapteurMouvement, INPUT);
  pinMode(pinCapteurTemperature, INPUT);
  pinMode(pinLED, OUTPUT);
}

void loop() {
  processReceivedMessage();
  Serial.println("Dans le loop !");
  
  // Lecture de la valeur du capteur de mouvement
  valeurCapteurMouvement = digitalRead(pinCapteurMouvement);
  if (valeurCapteurMouvement == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(1000);

  // Activation et désactivation du transistor
  //activerTransistor(valeurRPC);

  // Lecture de la valeur du capteur de température
  valeurCapteurTemperature = analogRead(pinCapteurTemperature);
  temperature = (double)valeurCapteurTemperature * (5 / 10.24);

  // Changement de la valeur de la LED
  analogWrite(pinLED, intensiteLED);


  /* // print the results to the serial monitor:
    Serial.print("Potention = ");
    Serial.print(valLm35);
    Serial.print("\t Intensite LED = ");
    Serial.println(intensiteLED);
    delay(20);*/

  appendPayload("Temperature", temperature);
  appendPayload("Motion", valeurCapteurMouvement);
  sendPayload();
}

