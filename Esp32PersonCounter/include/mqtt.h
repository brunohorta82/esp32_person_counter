#ifndef _MQTT_H_
#define _MQTT_H_

void mqttInit();
void mqttIsConected();
void mqttSend(const char *payload, bool retained);

#endif