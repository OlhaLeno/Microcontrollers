#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "web.h"

ESP8266WebServer server(80);

void handleChange()
{
    interval = (interval <= MIN_INTERVAL) ? DEFAULT_INTERVAL : interval - DECREASE_INTERVAL;
    server.send(200, "text/plain", "Local sequence changed!");
}

void setup()
{
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, []()
              { server.send_P(200, "text/html", index_html); });
    server.on("/change", HTTP_GET, handleChange);
    server.on("/remote", HTTP_GET, []()
              { handleRemoteCommand(); });
    server.begin();
}

void handleRemoteCommand()
{
    Serial.write(0b1110);
    server.send(200, "text/plain", "Remote command sent!");
}

void loop()
{
    server.handleClient();

    int buttonState = digitalRead(BUTTON);

    if (buttonState == LOW && lastButtonState == HIGH)
    {
        buttonPressStartTime = millis();
        intervalDecreased = false;
    }

    if (Serial.available())
    {
        int inByte = Serial.read();
        if (inByte == 0b1110)
        {
            interval = (interval <= MIN_INTERVAL) ? DEFAULT_INTERVAL : interval - DECREASE_INTERVAL;
        }
    }

    if (buttonState == LOW && millis() - buttonPressStartTime >= BUTTON_PRESS_TIMEOUT && !intervalDecreased)
    {
        Serial.write(0b1110);
        interval = (interval <= MIN_INTERVAL) ? DEFAULT_INTERVAL : interval - DECREASE_INTERVAL;
        intervalDecreased = true;
    }

    if (millis() - lastTime >= interval)
    {
        lastTime = millis();
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);

        switch (currentLED)
        {
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
}
