#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2     
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "ESP32-AP";
const char *password =  "LetMeInPlz";

const char *msg_toggle_led = "toggleLED";
const char *msg_get_led = "getLEDState";

const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
String Buzzer_State;

const int Buzzer_pin = 16;
const int smoke_pin = 14; //smoke detect 0 or 1
const int body_temp = 12; //body temperature (too high or normal)
const int sos_button = 13;




AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int led_state = 0;

void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {
  switch(type) {

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;
      
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;
  /*
    case WStype_TEXT:

     
      Serial.printf("[%u] Received text: %s\n", client_num, payload);

      
      if ( strcmp((char *)payload, "toggleLED") == 0 ) {
        led_state = led_state ? 0 : 1;
        Serial.printf("Toggling LED to %u\n", led_state);
        digitalWrite(led_pin, led_state);

      // Report the state of the LED
      } else if ( strcmp((char *)payload, "getLEDState") == 0 ) {
        sprintf(msg_buf, "%d", led_state);
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);

      } else {
        Serial.println("[%u] Message not recognized");
      }
      break;
      */

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}
///////////////////////////////////////////////////////////////////////// send data to web server
String getTemperature() {
  
  float temperature = 60; //dht.readTemperature();
  Serial.println(temperature);
  return String(temperature);
}
  
String getHumidity() {
  float humidity = 70; // dht.readHumidity();
  Serial.println(humidity);
  return String(humidity);
}

String getSmoke(){
  if(digitalRead(smoke_pin)){
    return String(1);
  }
  else{
    return String(2);
  }
}

String getBPM(){
  float bpm = Serial.read();
  return String(bpm);
}

String  getB_Temperature(){
  if(digitalRead(body_temp)){
    return String(3);
  }
  else{
    return String(4);
  }
}


String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(Buzzer_pin)){
      Buzzer_State = "ON";
    }
    else{
      Buzzer_State = "OFF";
    }
    Serial.print(Buzzer_State);
    return Buzzer_State;
  }
  
  else if (var == "TEMPERATURE"){
    return getTemperature();
  }
  else if (var == "HUMIDITY"){
    return getHumidity();
  }
  else if (var == "SMOKE"){
    return getSmoke();
  }
  else if(var == "BTEMPERATURE"){
    return  getB_Temperature();
  }
  else if(var == "BPM"){
    return getBPM();
  }
   
}
//////////////////////////////////////////////////////////////////////////////////////

void setup() {

  pinMode(Buzzer_pin, OUTPUT);
  pinMode(smoke_pin,INPUT);
  pinMode(body_temp,INPUT);
  pinMode(sos_button,INPUT);
  
  digitalWrite(Buzzer_pin, LOW);

  Serial.begin(115200);

  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }

  WiFi.softAP(ssid, password);


  Serial.println();
  Serial.println("AP running");
  Serial.print("My IP address: ");
  Serial.println(WiFi.softAPIP());

  
  server.on("/", HTTP_GET, onIndexRequest);
  server.on("/style.css", HTTP_GET, onCSSRequest);
  server.onNotFound(onPageNotFound);

  server.begin();
  
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

/////////////////////////////////////////////////////////////////////////////// get data from web server

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(Buzzer_pin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(Buzzer_pin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str(),processor);
  });
  
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHumidity().c_str(),processor);
  });

  server.on("/smoke", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getSmoke().c_str(),processor);
  });
  
  server.on("/btemperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getB_Temperature().c_str(),processor);
  });

  server.on("/bpm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getBPM().c_str(),processor);
  });
  
///////////////////////////////////////////////////////////////////////////////
  
}

void loop() {
  webSocket.loop();
  
}
