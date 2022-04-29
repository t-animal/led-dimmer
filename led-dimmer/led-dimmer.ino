#include "string"

#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>

#define PWM_PIN D4

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

ESP8266WebServer server(80);

void httpServerHandleRoot();
void httpServerHandleNotFound();
void httpServerHandleDimValue();
void httpServerHandleTime();
void httpServerHandleTimeUpdate();

unsigned int currentPercentage = 0;

const char *indexPage = "<!DOCTYPE html> <html> <head>  <meta charset=\"utf-8\">  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <title>LedDimmer config page</title>  <script>   function setCurrent(value) {    const headers = {'Content-Type': 'text/plain'};    fetch(     '/current', {     headers,     method: 'POST',     body: value    });   }    async function getAndWriteTime() {    const time = await (await fetch('/time')).text();    document.querySelector('#currentTime').textContent = time;   }    function forceTimeUpdate() {    const time = fetch('/time/update');    getAndWriteTime();   }    getAndWriteTime();   setInterval(getAndWriteTime, 10000);  </script> </head> <body> <h1>LedDimmer</h1>  <fieldset>  <legend>Presets</legend>  <button onclick=\"setCurrent(100)\">An (100%)</button>  <button onclick=\"setCurrent(70)\">An (70%)</button>  <button onclick=\"setCurrent(70)\">An (30%)</button>  <button onclick=\"setCurrent(0)\">Aus (0%)</button> </fieldset>  <fieldset>  <legend>Datum und Uhrzeit</legend>  <span id=\"currentTime\">Lade...</span> (UTC)  <button onclick=\"forceTimeUpdate()\">Force Update</button> </fieldset> </body> </html>";
const char* contentType = "Content-Type";

void setUpAndConnectWifi() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //  wm.resetSettings();

  bool res;
  wm.setHostname("LedDimmer");
  res = wm.autoConnect("LedDimmer");

  if(!res) {
    Serial.println("Connection to WiFi failed");
    ESP.restart();
  } 
  else {
    Serial.println("Successfully connected to Wifi)");
  }

  timeClient.begin();
  timeClient.setUpdateInterval(60*60*1000);
  timeClient.forceUpdate();
}

void setup() {
  setUpAndConnectWifi();
 
  Serial.begin(115200);

  pinMode(PWM_PIN, OUTPUT);

  server.on("/", httpServerHandleRoot);
  server.on("/current", httpServerHandleDimValue);
  server.on("/time", httpServerHandleTime);
  server.on("/time/update", httpServerHandleTimeUpdate);
  server.onNotFound(httpServerHandleNotFound);
  
  server.collectHeaders(&contentType, 1);
  server.begin();
}

void loop() {
  server.handleClient();
  timeClient.update();
  analogWrite(PWM_PIN, (100 - currentPercentage) * 1023 / 100); 
}

void sendUserError(String text, int code=400) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "text/plain", text);
}

void sendSuccess(String text) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", text);
}

void sendCurrentPercentage() {
  sendSuccess(String(currentPercentage));
}

void httpServerHandleDimValue() {
  if(server.method() == HTTP_GET) {
    sendCurrentPercentage();
    return;
  }

  if(server.method() == HTTP_POST) {
    String body = server.arg("plain");

    if(server.header(contentType) != "text/plain") {
      sendUserError("Send header Content-Type: text/plain");
      return;
    }

    if(body.length() > 3 || body.length() == 0) {
      sendUserError("Input too short or too long");
      return;
    }

    for (size_t i = 0; i < body.length(); i++) {
      if(!isDigit(body[i])) {
        sendUserError("Malformatted input at char " + i);
        return;
      }
    }

    int newValue = body.toInt();
    
    if(newValue < 0 || newValue > 100) {
      sendUserError("Out of range (must be -1 < x < 101)");
      return;
    }
    
    currentPercentage = newValue;
    
    sendCurrentPercentage();
    return;
  }

  sendUserError("Method Not Allowed", 405);
}

void httpServerHandleRoot() {
  server.send(200, "text/html", indexPage);
}

void httpServerHandleTime() {
  sendSuccess(timeClient.getFormattedTime());
}

void httpServerHandleTimeUpdate() {
  timeClient.forceUpdate();
  sendSuccess(timeClient.getFormattedTime());
}

void httpServerHandleNotFound() {
  String message = "File Not Found\n\n";
  message += "Time: " + timeClient.getFormattedTime();
  message += "\nURI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nContent-Type: " + server.headers() + server.header(contentType);
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
  message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "body: " + server.arg("plain");
  server.send(404, "text/plain", message);
}
