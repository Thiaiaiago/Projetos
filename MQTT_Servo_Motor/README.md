#  Controle de Servo Motor por MQTT
Neste projeto pessoal fiz um simples controle de ângulo de um servo motor através de comandos de um dashboard do Node Red enviados por MQTT.
No arquivo servomotor está o código para o controle do servo para ESP8266 no Arduino IDE.
Já o arquivo servomotor_control é o JSON do flow utilizado no Node Red.
Para utilizar basta substituir o nome e senha do WiFi no código do ESP e o tópico, tanto no Node Red, quanto no ESP.
