/* Include das bibliotecas usadas no código */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Servo.h>
 
 /* Defines dos pinos usados */
#define ServoPort 5
#define LED  14

/* Variáveis usados na conexão WiFi e MQTT */
const char* ssid = "WIFI";
const char* password = "SENHA";
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "TOPICO";

/* Variável recebida pelo MQTT */
char msg[100];

/* Declarando os objetos usados */
Servo myservo;
WiFiClient client;
PubSubClient MQTT(client);

/* Iniciando conexão WiFi */
void setup_WiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando à rede (em caso de demora, verifique se o nome e a senha estão corretos!):\nTentando conectar à rede de nome");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

/* Realiza a conexão com o Broker do MQTT */
void reconectar() {
  while (!MQTT.connected()) {
    Serial.println("Conectando ao Broker MQTT.");
    if (MQTT.connect("ESP8266")) {
      Serial.println("Conectado ao Broker com sucesso!");
      MQTT.subscribe("quarto");  // Inscreva-se no tópico "quarto"
    } else {
      Serial.print("Falha ao Conectar, rc=");
      Serial.print(MQTT.state());
      Serial.println(" tentando se reconectar...");
      delay(1500);
    }
  }
}

/* Lógica de controle do servo */
void servo_control(String pos)  {
  uint8_t p = pos.toInt();
  if (pos=="gira")  {
    controlLED();
    for (p = 0; p <= 180; p += 1) { 
      myservo.write(p);            
      delay(15);                    
    }
    for (p = 180; p >= 0; p -= 1) { 
      myservo.write(p);             
      delay(15);                
    }
    controlLED();
  } else  {
    myservo.write(p);
  }
}

/* Liga/Desliga LED simples */
void controlLED() {
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
}

/* Recebe a mensagem do MQTT */
void callback(char* topic, byte* payload, unsigned int length) {
  length = min(length, sizeof(msg) - 1);
  strncpy(msg, (char*)payload, length);
  msg[length] = '\0';
  Serial.println(msg);
}

/* Inicialização */
void setup() {
  Serial.begin(115200);
  setup_WiFi();
  MQTT.setServer(mqtt_server, 1883);
}

void loop() { 
  String mens = msg;
  if (!MQTT.connected()) {
    reconectar();
  }
  MQTT.loop();
  MQTT.setCallback(callback);
  servo_control(mens);
}
