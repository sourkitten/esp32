#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <TaskManagerIO.h>

#define BAUD 115200
#define DEBUG_ESP_PORT Serial
#define DEBUG_OUTPUT true

const char* ssid = "Vodafone-E25331383";
const char* password = "fk9P2pHbrfNEbCpr";

ESP8266WebServer server(80);
/*
void* wifi_connect(void* arg) {
    while(1) {
        if (WiFi.status() == WL_CONNECTED) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(2500);
            continue;
        }
        while (1) {
            delay(500);
            if (WiFi.status() != WL_CONNECTED) {
                digitalWrite(LED_BUILTIN, HIGH);
                Serial.print(".");
                continue;
            }
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("");
            Serial.print("Connected to ");
            Serial.println(ssid);
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            break;
        }
    }
}
*/

void handleRoot() {
    DEBUG_ESP_PORT.println("Debug: Request received");
    server.send(200, "text/plain", "I love you my Lilith <3     :3");
    DEBUG_ESP_PORT.println("Debug: Response sent");
}

void setup() {
    Serial.setDebugOutput(true);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(BAUD);
    WiFi.setHostname("webclient-esp8266");
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.begin((char*) ssid, (char*) password);
    //taskManager.scheduleOnce(1, [] {wifi_connect(NULL);}, TIME_SECONDS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print(".");
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    MDNS.begin("esp8266");
    DEBUG_ESP_PORT.println("Debug: MDNS responder started");
    server.on("/", HTTP_GET, handleRoot);
    DEBUG_ESP_PORT.println("Debug: root handler registered");
    server.begin();
    DEBUG_ESP_PORT.println("Debug: server started");
}

void loop() {
    server.handleClient();
    MDNS.update();
    //taskManager.runLoop();
}