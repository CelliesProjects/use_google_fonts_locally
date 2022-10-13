#include <AsyncTCP.h>              /* https://github.com/me-no-dev/AsyncTCP */
#include <ESPAsyncWebServer.h>     /* https://github.com/me-no-dev/ESPAsyncWebServer */

#include "index_htm.h"
#include "Roboto-latin.h"
#include "Roboto-latin-ext.h"

const char* WIFI_NETWORK = "xxx";
const char* WIFI_PASSWORD = "xxx";

void setup() {
    Serial.begin(115200);

    Serial.printf("\n\nConnecting to %s\n", WIFI_NETWORK);

    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    WiFi.setSleep(false);

    while (!WiFi.isConnected())
        delay(10);

    static AsyncWebServer http_server(80);
    static const char* CONTENT_TYPE_HTML {"text/html"};
    static const char* CONTENT_TYPE_WOFF2 {"application/x-font-woff2"};

    http_server.on("/", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_HTML, index_htm, index_htm_len);
        request->send(response);
    });

    http_server.on("/Roboto-latin-ext.woff2", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_WOFF2, Roboto_latin_ext_woff2, Roboto_latin_ext_woff2_len);
        request->send(response);
    });

    http_server.on("/Roboto-latin.woff2", HTTP_GET, [](AsyncWebServerRequest * const request) {
        AsyncWebServerResponse* const response = request->beginResponse_P(200, CONTENT_TYPE_WOFF2, Roboto_latin_woff2, Roboto_latin_woff2_len);
        request->send(response);
    });

    http_server.begin();

    Serial.printf("Webserver started at %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
    delay(100);
}
