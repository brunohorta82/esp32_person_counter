#include "mqtt.h"

#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

void mqttReconect();

/* WIFI CONFIG*/
const char *ssid = "IOTBH";
const char *password = "IOT2017@";

/* MQTT CONFIG*/
const char *mqttServer = "192.168.187.203";
const int mqttPort = 1883;
const char *mqttUser = "homeassistant";
const char *mqttPassword = "moscasMoscas82";
const char *clientId = "esp32SensorA";
const char *topic = "personcounter/sensor/status";

void mqttInit()
{

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("WiFi Connected");

    client.setServer(mqttServer, mqttPort);
}

void mqttIsConected()
{
    if (!client.connected())
    {
        mqttReconect();
    }
}

void mqttSend(const char *payload, bool retained)
{

    client.publish(topic, payload, retained);
}

void mqttReconect()
{

    while (!client.connected())
    {

        if (client.connect(clientId, mqttUser, mqttPassword))
        {
            Serial.println("Connected to Broker!");
        }
        else
        {
            Serial.print("Broker connection Error. Reason: ");
            Serial.print(client.state());
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}