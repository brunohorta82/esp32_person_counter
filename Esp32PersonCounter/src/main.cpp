
#include "main.h"

void setup()
{
  Serial.begin(115200);
  
  mqttInit();
  rtosInit();
}

void loop()
{
  vTaskDelete(NULL);
}