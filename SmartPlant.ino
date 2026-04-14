/*
 * Project: ESP32-S3 Smart Plant Monitor
 * Architecture: FreeRTOS Dual-Core, WebSocket Telemetry (Blynk IoT)
 * Hardware: ESP32-S3 (N16R8), DHT22, Capacitive Moisture, WS2812B, DFPlayer
 */

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <Adafruit_NeoPixel.h>

// --- PIN DEFINITIONS ---
#define MOISTURE_PIN 4
#define LDR_PIN 5
#define DHT_PIN 18
#define LED_PIN 21
#define DFPLAYER_RX 16
#define DFPLAYER_TX 17

// --- FreeRTOS TASK HANDLES ---
TaskHandle_t TaskNetworkHandle;
TaskHandle_t TaskHardwareHandle;

void setup() {
  Serial.begin(115200);
  
  // Create Hardware Task (Pinned to Core 1)
  xTaskCreatePinnedToCore(
    HardwareControlTask,   /* Task function. */
    "HardwareTask",        /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                  /* parameter of the task */
    1,                     /* priority of the task */
    &TaskHardwareHandle,   /* Task handle to keep track of created task */
    1);                    /* pin task to core 1 */

  // Create Network Task (Pinned to Core 0)
  xTaskCreatePinnedToCore(
    NetworkTelemetryTask,  /* Task function. */
    "NetworkTask",         /* name of task. */
    10000,                 /* Stack size of task */
    NULL,                  /* parameter of the task */
    1,                     /* priority of the task */
    &TaskNetworkHandle,    /* Task handle to keep track of created task */
    0);                    /* pin task to core 0 */
}

void loop() {
  // Main loop is left empty intentionally. 
  // FreeRTOS manages the Core 0 and Core 1 tasks asynchronously.
}

// --- CORE 1: TIME-SENSITIVE HARDWARE OPERATIONS ---
void HardwareControlTask( void * pvParameters ){
  for(;;){
    // TODO: Poll DHT22 strictly
    // TODO: Read Moisture & LDR ADC
    // TODO: Drive WS2812B via RMT
    // TODO: UART Send to DFPlayer
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

// --- CORE 0: WIFI & IOT TELEMETRY ---
void NetworkTelemetryTask( void * pvParameters ){
  for(;;){
    // TODO: Maintain Blynk WebSocket Connection
    // TODO: Push Telemetry Data
    Blynk.run();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
