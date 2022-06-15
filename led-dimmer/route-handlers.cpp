#include "route-handlers.h"


void sendUserError(String text, int code=400) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "text/plain", text);
}

void sendSuccess(String text, int code=200, String contentType="text/plain") {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, contentType, text);
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
    if(server.header(contentType) != "text/plain") {
      sendUserError("Send header Content-Type: text/plain");
      return;
    }
    
    String body = server.arg("plain");
    int newValue = parsePercentage(body);

    if(newValue == INCORRECT_DIGIT_COUNT) {
      sendUserError("Input too short or too long");
      return;
    }

    if(newValue == MALFORMATTED_STRING) {
      sendUserError("Malformatted dim value");
      return;
    }

    if(newValue == OUT_OF_RANGE) {
      sendUserError("Out of range (must be -1 < x < 101)");
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

void httpServerHandleConfig(int index, AutoConfig& config) {
  if(server.method() == HTTP_GET) {
    sendSuccess(config.toJson(), 200, "application/json");
    return;
  }

  if(server.method() == HTTP_POST) {
    if(!AutoConfig::fromRequest(server, config)) {
      sendUserError("Malformatted request");
      return;
    }
    config.storeInEeprom(index);
    sendSuccess(config.toJson(), 200, "application/json");
  }
}


void httpServerHandleLog() {
  sendSuccess(logger.read());
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
