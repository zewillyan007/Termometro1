#include <Arduino.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>


const char *ssid = "ESP32Will";
const char *password = "esp123456789";

WiFiClient client;
IPAddress ip (192, 168, 19, 2);
IPAddress netmask (255, 255, 255, 0);
const int port = 5210;
WiFiServer server(port);

int Conect; //contador para verificar conexão


//Sensor de temperatura usando o LM35
 
const int LM35 = 34; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida
float cal = 0.4;
float tempC;
float Temp;

float Temperature() {
  temperatura = (float(analogRead(LM35))*3.3/(1023))/0.01;
  tempC = cal*temperatura;

  return tempC;
}
 
//Função que será executada uma vez quando ligar ou resetar o Arduino
void setup() {
Serial.begin(9600); // inicializa a comunicação serial

WiFi.mode(WIFI_AP);
WiFi.softAPConfig(ip, ip, netmask);
WiFi.softAP(ssid, password);
Serial.println("Wifi Details:");
Serial.println(ssid);
Serial.println(password);
Serial.println(WiFi.localIP());
server.begin();
}
 
//Função que será executada continuamente
void loop() {

if (!client.connected()) {
    Serial.println();
    Serial.println("Não há aparelho conectado!");
    
    Temp = Temperature();

    if (Temp > 0) {
      Serial.print(Temp);
      Serial.println("°C");
      delay(1000);
    }

    Conect = 0;

    client = server.available(); //reconexão
    return;
  }

  if (client.available() >= 0) {

    if (Conect == 0) {
      Serial.println();
      Serial.println("Conectado com Sucesso!");
    }

    Temp = Temperature();

    if (Temp > 0) {
      Serial.print(Temp);
      Serial.println("°C");
      client.print(Temp);
      client.println("°C");
      delay(1000);
    }

    Conect ++;
  }
}