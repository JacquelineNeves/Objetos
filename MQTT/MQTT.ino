#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
 
#define WIFISSID "PANDORA"      // Coloque seu WifiSSID aqui
#define PASSWORD "87654321" // Coloque sua senha wifi aqui
#define TOKEN "BBFF-Z3kUMfo9dNEPEzkKLvuW8qjEF2vIrO" // Coloque o TOKEN do seu Ubidots
#define MQTT_CLIENT_NAME "jacNeves"               // Nome do cliente MQTT, insira sua própria sequência ASCII de 8-12 caracteres alfanuméricos;
                                                    // deve ser uma string ascii aleatória e única e diferente de todos os outros dispositivos
 
/****************************************
 * Definir constantes
 ****************************************/
#define VARIABLE_LABEL "sensor" // Atribuir o rótulo da variável
#define DEVICE_LABEL "esp32"    // Atribuir o rótulo do dispositivo
 
#define SENSOR A0 // Defina o A0 como SENSOR
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Espaço para armazenar valores para enviar
char str_sensor[10];
 
/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
 
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = '\0';
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect() {
  // Loop até estarmos reconectados
  while (!client.connected()) {
    Serial.println("Tentando conexão MQTT...");
    
    //Tentativa de conexão
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
 
/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  // Atribua o pino como INPUT
  pinMode(SENSOR, INPUT);
 
  Serial.println();
  Serial.print("Esperando por WiFi ...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço de IP:");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adiciona o rótulo da variável
  
  float sensor = analogRead(SENSOR); 
  
  /4 é a largura mínima, 2 é a precisão; valor flutuante é copiado para str_sensor/
  dtostrf(sensor, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publicação de dados na nuvem Ubidots");
  client.publish(topic, payload);
  client.loop();
  delay(500);
}
