/**
   Helloworld style, connect an ESP8266 to the IBM IoT Foundation

   Author: Ant Elder
   License: Apache License v2
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

//-------- Customise these values -----------
const char* ssid = "Trojan";
const char* password = "#98765*zYaW#.i.*.%@.";

#define ORG "1u1pnl"
#define DEVICE_TYPE "NodeMCU"
#define DEVICE_ID "T2"
#define TOKEN "velhacoman"
//-------- Customise the above values --------


char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

int counter = 0;

void loop() {

  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }

  //ler coordenadas do arduino
  byte bytesRecebidos[64];
  if (Serial.available() > 0) {
    Serial.readBytes(bytesRecebidos, 64);
  }
  String myString = String((char*)bytesRecebidos);
  String NodeCorredor = getValue(myString, '#', 1);
  String CoordenadasGPS = getValue(myString, '#', 2);
  //"{\"Sucesso\":\"" + temperature + "\"}"
  String payload = "{\"d\":{\"Corredor\":\"" + NodeCorredor + "\", GPS:\"" + CoordenadasGPS + "\""  ;

  //String payload = "{\"d\":{\"myName\":\"ESP8266.Test1\",\"counter\":";
  //payload += counter;
  payload += "}}";

  Serial.print("Sending payload: ");
  Serial.println(payload);

  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
  } else {
    Serial.println("Publish failed");
  }

  ++counter;
  delay(30000);
}




String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
