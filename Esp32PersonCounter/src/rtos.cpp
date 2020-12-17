#include "rtos.h"
#include <ArduinoJson.h>
#include "freertos/queue.h"

#define SENSOR_R 22
#define SENSOR_L 23
#define SENSOR_ZONE "ZONE1"

QueueHandle_t xQueue;
TaskHandle_t xTaskMQTTHandle;
TaskHandle_t xTaskSensorHandle;

void vTaskSensor(void *pvParameters);
void vTaskMQTT(void *pvParameters);

typedef struct counter
{
    int left;
    int right;
} counter_t;

void rtosInit()
{
    xQueue = xQueueCreate(1, sizeof(counter_t));
    xTaskCreatePinnedToCore(vTaskSensor, "TaskSensor", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskSensorHandle, APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskMQTT, "TaskMQTT", configMINIMAL_STACK_SIZE + 2048, NULL, 2, &xTaskMQTTHandle, PRO_CPU_NUM);
}

void vTaskSensor(void *pvParameters)
{
    pinMode(SENSOR_R, INPUT_PULLUP);
    pinMode(SENSOR_L, INPUT_PULLUP);
    counter_t counter = {0, 0};
    boolean lastLeftState = false;
    boolean lastRightState = false;
    while (1)
    {
        lastLeftState = false;
        while (!digitalRead(SENSOR_R))
        {
            bool currentL = !digitalRead(SENSOR_L);
            if (currentL && lastLeftState != currentL)
            {
                counter.left++;
            }
            lastLeftState = currentL;
            vTaskDelay(10);
        }
        lastRightState = false;
        while (!digitalRead(SENSOR_L))
        {
            bool currentR = !digitalRead(SENSOR_R);
            if (currentR && lastRightState != currentR)
            {
                counter.right++;
            }
            lastRightState = currentR;
            vTaskDelay(10);
        }
        if (lastLeftState || lastRightState)
        {
            xQueueOverwrite(xQueue, &counter);
        }
        vTaskDelay(10);
    }
}

void vTaskMQTT(void *pvParameters)
{
    counter_t counter;
    while (1)
    {
        if (xQueueReceive(xQueue, &counter, portMAX_DELAY) == pdTRUE)
        {
            StaticJsonDocument<300> msg;
            msg["left"] = counter.left;
            msg["right"] = counter.right;
            msg["zone"] = SENSOR_ZONE;
            char buffer[256];
            serializeJson(msg, buffer);
            mqttIsConected();
            mqttSend(buffer, false);
        }
        vTaskDelay(10);
    }
}
