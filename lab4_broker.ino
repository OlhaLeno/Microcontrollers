#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char *ssid = "*******";
const char *password = "*******";

const char *mqtt_server = "7322327735d543e787336569027987ff.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char *mqtt_user = "123qwe";
const char *mqtt_pass = "Qwerty123";
const char *mqtt_topic = "wemos/text";

WiFiClientSecure secureClient;
PubSubClient client(secureClient);

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message = "";
    
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Received on ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);
}

void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi");
    }
}

void connectMQTT() {
    secureClient.setInsecure();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);

    int attempts = 0;
    while (!client.connected() && attempts < 5) {
        Serial.print("Connecting to MQTT...");
        String clientId = "ESP8266Client-" + String(ESP.getChipId());
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
            Serial.println("connected");
            client.subscribe(mqtt_topic);
            Serial.print("Subscribed to ");
            Serial.println(mqtt_topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" - retrying in 5 seconds");
            delay(5000);
            attempts++;
        }
    }
    if (!client.connected()) {
        Serial.println("MQTT connect failed after 5 attempts");
    }
}

void setup() {
    Serial.begin(115200);
    connectWiFi();
    connectMQTT();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi lost, reconnecting...");
        connectWiFi();
    }
    
    if (!client.connected()) {
        connectMQTT();
    }

    client.loop();
}
