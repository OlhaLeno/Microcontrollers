#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "web.h"

ESP8266WebServer server(80);

void setup() {
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_ATTEMPTS) {
        if (millis() - lastTime >= WIFI_RETRY_INTERVAL_MS) {
            Serial.print(".");
            attempts++;
            lastTime = millis();
        }
        Serial.print("Connecting to WiFi...");
        Serial.println(WiFi.status());
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi Failed to Connect");
    }

    server.on("/", HTTP_GET, []() {
        server.send_P(200, "text/html", index_html);
    });
    server.on("/increase", HTTP_GET, []() { handleButtonPressWeb(server); });
    server.begin();
}

void loop() {
    int buttonState = digitalRead(BUTTON);

    if (buttonState == LOW && lastButtonState == HIGH) {
        buttonPressStartTime = millis();
        intervalDecreased = false;
    }

    if (buttonState == LOW && millis() - buttonPressStartTime >= BUTTON_PRESS_TIMEOUT && !intervalDecreased) {
        interval = (interval <= MIN_INTERVAL) ? DEFAULT_INTERVAL : interval - DECREASE_INTERVAL;
        intervalDecreased = true;
        Serial.println("Interval: " + String(interval));
    }

    if (millis() - lastTime >= interval) {
        lastTime = millis();
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);

        switch (currentLED) {
            case LED1_INDEX: 
                digitalWrite(LED_PINS[LED1_INDEX], HIGH); 
                currentLED = LED2_INDEX; 
                break;
            case LED2_INDEX: 
                digitalWrite(LED_PINS[LED2_INDEX], HIGH); 
                currentLED = LED3_INDEX; 
                break;
            case LED3_INDEX: 
                digitalWrite(LED_PINS[LED3_INDEX], HIGH); 
                currentLED = LED1_INDEX; 
                break;
        }
    }

    lastButtonState = buttonState;
    server.handleClient();
}
