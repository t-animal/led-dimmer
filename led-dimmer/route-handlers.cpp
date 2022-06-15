#include "route-handlers.h"

// char *indexPage = "<!DOCTYPE html> <html> <head>  <meta charset=\"utf-8\">  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <title>LedDimmer config page</title>  <script>   const urlBase='';   /*const urlBase='http://leddimmer.fritz.box';*/    function setCurrent(value) {    const headers = {'Content-Type': 'text/plain'};    fetch(     `${urlBase}/current`, {     headers,     method: 'POST',     body: value    });   }    async function getAndWriteTime() {    const time = await (await fetch(`${urlBase}/time`)).text();    document.querySelector('#currentTime').textContent = time;   }    function forceTimeUpdate() {    const time = fetch(`${urlBase}/time/update`);    getAndWriteTime();   }    async function getAndWriteConfig(index) {    const config = await (await fetch(`${urlBase}/config/${index}`)).json();    document.querySelector(`[data-config-index=\"${index}\"] [name=enabled]`).checked = config.enabled;    document.querySelector(`[data-config-index=\"${index}\"] [name=startTime]`).value = config.startTime;    document.querySelector(`[data-config-index=\"${index}\"] [name=endTime]`).value = config.endTime;    document.querySelector(`[data-config-index=\"${index}\"] [name=percentage]`).value = config.percentage;    document.querySelector(`[data-config-index=\"${index}\"] .loading`).style.display = 'none';   }    async function updateConfig(index) {    document.querySelector(`[data-config-index=\"${index}\"] .loading`).style.display = 'block';    const form = document.querySelector(`[data-config-index=\"${index}\"] form`);    const data = new URLSearchParams(new FormData(form));    await fetch(     `${urlBase}/config/${index}`, {     method: 'POST',     body: data    });    await getAndWriteConfig(index);   }    getAndWriteTime();   setInterval(getAndWriteTime, 10000);   getAndWriteConfig(0);   getAndWriteConfig(1);   getAndWriteConfig(2);   getAndWriteConfig(3);   getAndWriteConfig(4);  </script>   <style>   label { margin-right: 5ex; }   .auto-config-set { position: relative; }   .auto-config-set .loading {    position: absolute;    width: calc(100% - 15px);    height: calc(100% - 15px);    background: rgba(255,255,255,0.9);    font-size:  1.5em;   }  </style> </head> <body> <h1>LedDimmer</h1>  <fieldset>  <legend>Presets</legend>  <button onclick=\"setCurrent(100)\">An (100%)</button>  <button onclick=\"setCurrent(70)\">An (70%)</button>  <button onclick=\"setCurrent(70)\">An (30%)</button>  <button onclick=\"setCurrent(0)\">Aus (0%)</button> </fieldset>  <fieldset>  <legend>Datum und Uhrzeit</legend>  <span id=\"currentTime\">Lade...</span> (UTC)  <button onclick=\"forceTimeUpdate()\">Force Update</button> </fieldset>  <h2>Automatisiertes Schalten</h2>  <p>Beachte: Uhrzeiten müssen in UTC angegeben werden (vergleiche Uhrzeit oben).</p>  <!-- TODO: minimize using template? --> <fieldset class=\"auto-config-set\" data-config-index=\"0\">  <div class=\"loading\">Lade...</div>  <legend>Automatisches ein- und ausschalten Config 1</legend>  <form action=\"/config/0\" method=\"POST\">   <label>Aktiviert <input type=\"checkbox\" name=\"enabled\"></label>   <label>Anschalten um <input type=\"time\" name=\"startTime\" required></label>   <label>Abschalten um <input type=\"time\" name=\"endTime\" required></label>   <label>Wert <input type=\"number\" min=\"0\" max=\"100\" name=\"percentage\" required></label>   <button onclick=\"updateConfig(0);return false;\" type=\"submit\">Speichern</button>  </form> </fieldset>  <fieldset class=\"auto-config-set\" data-config-index=\"1\">  <div class=\"loading\">Lade...</div>  <legend>Automatisches ein- und ausschalten Config 2</legend>  <form action=\"/config/1\" method=\"POST\">   <label>Aktiviert <input type=\"checkbox\" name=\"enabled\"></label>   <label>Anschalten um <input type=\"time\" name=\"startTime\" required></label>   <label>Abschalten um <input type=\"time\" name=\"endTime\" required></label>   <label>Wert <input type=\"number\" min=\"0\" max=\"100\" name=\"percentage\" required></label>   <button onclick=\"updateConfig(1);return false;\" type=\"submit\">Speichern</button>  </form> </fieldset>  <fieldset class=\"auto-config-set\" data-config-index=\"2\">  <div class=\"loading\">Lade...</div>  <legend>Automatisches ein- und ausschalten Config 3</legend>  <form action=\"/config/2\" method=\"POST\">   <label>Aktiviert <input type=\"checkbox\" name=\"enabled\"></label>   <label>Anschalten um <input type=\"time\" name=\"startTime\" required></label>   <label>Abschalten um <input type=\"time\" name=\"endTime\" required></label>   <label>Wert <input type=\"number\" min=\"0\" max=\"100\" name=\"percentage\" required></label>   <button onclick=\"updateConfig(2);return false;\" type=\"submit\">Speichern</button>  </form> </fieldset>  <fieldset class=\"auto-config-set\" data-config-index=\"3\">  <div class=\"loading\">Lade...</div>  <legend>Automatisches ein- und ausschalten Config 4</legend>  <form action=\"/config/3\" method=\"POST\">   <label>Aktiviert <input type=\"checkbox\" name=\"enabled\"></label>   <label>Anschalten um <input type=\"time\" name=\"startTime\" required></label>   <label>Abschalten um <input type=\"time\" name=\"endTime\" required></label>   <label>Wert <input type=\"number\" min=\"0\" max=\"100\" name=\"percentage\" required></label>   <button onclick=\"updateConfig(3);return false;\" type=\"submit\">Speichern</button>  </form> </fieldset>  <fieldset class=\"auto-config-set\" data-config-index=\"4\">  <div class=\"loading\">Lade...</div>  <legend>Automatisches ein- und ausschalten Config 5</legend>  <form action=\"/config/4\" method=\"POST\">   <label>Aktiviert <input type=\"checkbox\" name=\"enabled\"></label>   <label>Anschalten um <input type=\"time\" name=\"startTime\" required></label>   <label>Abschalten um <input type=\"time\" name=\"endTime\" required></label>   <label>Wert <input type=\"number\" min=\"0\" max=\"100\" name=\"percentage\" required></label>   <button onclick=\"updateConfig(4);return false;\" type=\"submit\">Speichern</button>  </form> </fieldset>   </body> </html>";
// const char *contentType = "Content-Type";

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
