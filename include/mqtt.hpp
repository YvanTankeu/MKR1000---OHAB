#include <WiFi101.h>
#include <ArduinoMqttClient.h>

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
String Payload = "{"; // Chaine de caractère qui contiendra le message envoyer de l'objet vers thingsboard
bool first = false;

String RPCData, PrevData;

const char stateTopic[]  = "MKR1000/Capteur/telemetries";
const char commandTopic[]  = "MKR1000/Actuateur";
const char KnobTopic[]  = "MKR1000/Actuateur/pwm";
/* 
  Fonctionnalité qui permet de créer la chaine de données a envoyé au broker MQTT
  Pour le moment, cette chaine ne doit pas dépassé un maximum de 100 caractère
*/

void appendPayload(String Name, float Val)
{

  if( Payload != "{")
  {
    Payload += ",";  
  }
  
  Payload += "\"";
  Payload += Name;
  Payload += "\": ";
  Payload += Val;
   
}

/*
  Fonctionnalité qui permet l'envoie de la chaine de caractères sous la forme de paires
  Nom associé a la donnée, Valeur de al donnée
*/ 

void sendPayload()
{
  char attributes[200];
  Payload += "}";
  Payload.toCharArray(attributes, 200);
  mqttClient.beginMessage(stateTopic);
  mqttClient.print(attributes);
  mqttClient.endMessage();

  
  Serial.print("Payload -> ");
  Serial.println(Payload);
  Serial.print("Payload length -> ");
  Serial.println(Payload.length());

  Payload="{";
  
}


// Affiche la payload qui est envoyé ainsi que sa longeur

/*
  Fonctionnalité qui permet de créer la chaine de données a envoyé au broker MQTT
  Pour le moment, cette chaine ne doit pas dépassé un maximum de 100 caractère
*/
// {"ts":1666778909000,"values":{
/*void appendTimestamps(float value)
{
  first = false;
  Payload = "{\"ts\":";
  Payload += value;
  Payload += ",\"values\":{";
}

//"Temperature":23.169,"Humidite":59.411}}
void appendPayload(String Name, float Val)
{
  if (first)
  {
    Payload += ",";
  }
  Payload += "\"";
  Payload += Name;
  Payload += "\": ";
  Payload += Val;
  first = true;
}


void sendPayload()
{
  char attributes[200];
  Payload += "}}";
  Payload.toCharArray(attributes, 200);

  mqttClient.beginMessage(stateTopic);
  mqttClient.print(100);
  mqttClient.endMessage();

  Serial.print("Payload -> ");
  Serial.println(Payload);
  Serial.print("Payload length -> ");
  Serial.println(Payload.length());


}

void sendTempPayload()
{
  char attributes[200];
  Payload.toCharArray(attributes, 200);

  mqttClient.beginMessage(stateTopic);
  mqttClient.print(attributes);
  mqttClient.endMessage();

  Serial.print("Payload -> ");
  Serial.println(Payload);
  Serial.print("Payload length -> ");
  Serial.println(Payload.length());
}*/
