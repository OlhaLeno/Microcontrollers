const char *ssid = "611VVA";
const char *password = "123qwerty9";

#define BUTTON 14
#define LED1 0
#define LED2 2
#define LED3 13

const int MIN_INTERVAL = 200;
const int DEFAULT_INTERVAL = 1000;
const int DECREASE_INTERVAL = 200;
const int BUTTON_PRESS_TIMEOUT = 2000;
const int MAX_WIFI_ATTEMPTS = 30;
const int WIFI_RETRY_INTERVAL_MS = 1000;
const int LED1_INDEX = 0;
const int LED2_INDEX = 1;
const int LED3_INDEX = 2;
const int LED_PINS[] = {LED1, LED2, LED3};

int interval = DEFAULT_INTERVAL;
int lastButtonState = HIGH;
unsigned long lastTime = 0;
int currentLED = 1;
unsigned long buttonPressStartTime = 0;
bool intervalDecreased = false;
