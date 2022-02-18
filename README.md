# ESP32 SENSOR FIRMWARE

Este repositório irá conter o código que o ESP32 usará para fazer as leituras dos sensores de temperatura e humidade, movimento e gás e fumaça, e também enviar os dados coletadas para um MQTT Broker.


## Configuração

Crie um arquivo chamado config.h, adicione os seguintes defines e um valor para cada um deles

 - SSID
 - PASSWORD
 - MQTT_SERVER
 - MQTT_PORT
 - MQTT_USER
 - MQTT_PASSWORD

#### Exemplo

``#define SSID "Minha redel"``