#include <ESP8266WiFi.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "611VVA";
const char *password = "123qwerty9";

bool isDigitalMode = false;
bool lastButtonState = HIGH;

const long gmtOffset_sec = 2 * 3600;
const int daylightOffset_sec = 3600;

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.println("Starting...");

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    Serial.println("Display initialized");

    display.clearDisplay();
    display.setTextColor(WHITE);

    WiFi.begin(ssid, password);

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Connecting to WiFi...");
    display.display();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    Serial.println("WiFi connected");

    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connected!");
    display.display();
    delay(1000);

    Serial.println("Setup complete");
}

void displayAnalogClock(struct tm *timeinfo)
{
    display.clearDisplay();

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int clockRadius = 30;

    display.drawCircle(centerX, centerY, clockRadius, WHITE);

    for (int i = 0; i < 12; i++)
    {
        float angle = i * 30 * PI / 180;
        int x1 = centerX + (clockRadius - 2) * sin(angle);
        int y1 = centerY - (clockRadius - 2) * cos(angle);
        int x2 = centerX + clockRadius * sin(angle);
        int y2 = centerY - clockRadius * cos(angle);
        display.drawLine(x1, y1, x2, y2, WHITE);
    }

    float hourAngle = ((timeinfo->tm_hour % 12) + timeinfo->tm_min / 60.0) * 30 * PI / 180;
    int hourX = centerX + 20 * sin(hourAngle);
    int hourY = centerY - 20 * cos(hourAngle);
    display.drawLine(centerX, centerY, hourX, hourY, WHITE);

    float minAngle = timeinfo->tm_min * 6 * PI / 180;
    int minX = centerX + 25 * sin(minAngle);
    int minY = centerY - 25 * cos(minAngle);
    display.drawLine(centerX, centerY, minX, minY, WHITE);

    float secAngle = timeinfo->tm_sec * 6 * PI / 180;
    int secX = centerX + 28 * sin(secAngle);
    int secY = centerY - 28 * cos(secAngle);
    display.drawLine(centerX, centerY, secX, secY, WHITE);
}

void displayDigitalClock(struct tm *timeinfo)
{
    display.clearDisplay();
    display.setTextSize(2);

    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d",
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, SCREEN_HEIGHT / 2 - h);
    display.println(timeStr);

    display.setTextSize(1);
    char dateStr[11];
    sprintf(dateStr, "%02d/%02d/%04d",
            timeinfo->tm_mday,
            timeinfo->tm_mon + 1,
            timeinfo->tm_year + 1900);

    display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, SCREEN_HEIGHT - h - 10);
    display.println(dateStr);
}

void loop()
{
    int buttonState = digitalRead(BUTTON_PIN);

    static int lastPrintedState = -1;
    if (buttonState != lastPrintedState)
    {
        Serial.print("Button state: ");
        Serial.println(buttonState);
        lastPrintedState = buttonState;
    }

    if (buttonState == LOW && lastButtonState == HIGH)
    {
        Serial.println("Button pressed!");
        isDigitalMode = !isDigitalMode;
        Serial.print("Mode changed to: ");
        Serial.println(isDigitalMode ? "Digital" : "Analog");
        delay(200);
    }
    lastButtonState = buttonState;

    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);

    if (isDigitalMode)
    {
        displayDigitalClock(timeinfo);
    }
    else
    {
        displayAnalogClock(timeinfo);
    }

    display.display();
    delay(50);
}