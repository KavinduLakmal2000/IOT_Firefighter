#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;  
SoftwareSerial SerialGPS(4, 5,false);

#define DHTPIN 2     
#define DHTTYPE DHT22    

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "ESP32-AP";
const char *password =  "LetMeInPlz";

const char *msg_toggle_led = "toggleLED";
const char *msg_get_led = "getLEDState";

const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
String Buzzer_State;

float latitude , longitude;
String lat_str , lng_str;


const int Buzzer_pin = 16;
const int smoke_pin = 14; //smoke detect 0 or 1 (connect primini pin 5)
const int body_temp = A0; //body temperature 
const int sos_button = 13;
const int aroundTemp = 15; // signal to promini

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float temp;
int ran;
int ran1;
int ran2;
float Lng = (0.0000,4);
float Lat = (0.0000,4);

void ranTemp(){
  ran = random(25); // less than 25 random

  switch (ran){
    
    case 0:
    temp = 34.4;
    break;
    
    case 1:
    temp = 34.5;
    break;

     case 2:
    temp = 34.6;
    break;

     case 3:
    temp = 34.7;
    break;

     case 4:
    temp = 34.8;
    break;

     case 5:
    temp = 34.9;
    break;

     case 6:
    temp = 35.0;
    break;

     case 7:
    temp = 35.1;
    break;

    case 8:
    temp = 35.2;
    break;

    case 9:
    temp = 35.5;
    break;

    case 10:
    temp = 35.8;
    break;

    case 11:
    temp = 35.9;
    break;

    case 12:
    temp = 36.2;
    break;

    case 13:
    temp = 36.3;
    break;

    case 14:
    temp = 36.4;
    break;

    case 15:
    temp = 36.7;
    break;

    case 16:
    temp = 36.8;
    break;

    case 17:
    temp = 36.9;
    break;

    case 18:
    temp = 37.1;
    break;

    case 19:
    temp = 37.2;
    break;

    case 20:
    temp = 37.4;
    break;

    case 21:
    temp = 37.6;
    break;

    case 22:
    temp = 37.7;
    break;

    case 23:
    temp = 37.8;
    break;

    case 24:
    temp = 37.9;
    break;
    
  }
}
void ranBPM(){
  ran1 = random(490,540);
}
void ranLoc(){
  ran2 = random(5);

  switch(ran2){
    case 1:
    Lng = 7.1870;
    Lat = 79.8209;
    break;

    case 2:
    Lng = 6.9271;
    Lat = 79.8612;
    break;

    case 3:
    Lng = 6.8649;
    Lat = 79.8997;
    break;

    case 4:
    Lng = 6.9094;
    Lat = 79.8943;
    break;
    
  }
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

// send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// send sos page
void onsosRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/sos.html", "text/html");
}

// send js page
void onjsRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/java_s.js", "text/js");
}

// send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/style.css", "text/css");
}


// send 404 if requested file does not exist  onsosRequest
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}
///////////////////////////////////////////////////////////////////////// send data to web server
String getTemperature() {
  float temperature = dht.readTemperature();
  return String(temperature);
  if (temperature < 50){
    digitalWrite(aroundTemp,HIGH);
  }
  else{
    digitalWrite(aroundTemp,LOW);
  }
}
  
String getHumidity() {
  float humidity = dht.readHumidity();
  return String(humidity);
}

String getSmoke(){
  if(digitalRead(smoke_pin)==HIGH){
    return String(0);
  }
  else{
    return String(1);
  }
}

String getBPM(){
  /*
  float bpm = Serial.read();
  return String(bpm);
  */
  return String(ran1);
}

String  getB_Temperature(){
  /*
  int reading = analogRead(body_temp);
  float voltage = reading * (5.0 / 1024.0);
  float b_temp = voltage * 100; 
  
  */
return String(37.6);
}

String getlng(){
   return String(7.1870);
  /*
  while (SerialGPS.available() > 0){
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        longitude = gps.location.lng();
        lng_str = String(longitude, 6);
        return String(lng_str);
      }
    }
  }
*/
  
}

String getlat(){
  return String(79.8209);

/*
  while (SerialGPS.available() > 0){
    if (gps.encode(SerialGPS.read()))
    {
     if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude, 6);
        return String(lat_str);
      }
    }
  }
  */
}

String getSOS(){
  if(digitalRead(sos_button)==HIGH){
    return String(3);
    digitalWrite(Buzzer_pin,HIGH);
  }
  else{
    return String(2);
    digitalWrite(Buzzer_pin,LOW);
  }
}


String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(Buzzer_pin)){
      Buzzer_State = "ON";
    }
    else{
      Buzzer_State = "OFF";
    }
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
  else if(var == "LNG"){
    return getlng();
  }
  else if (var == "LAT"){
    return getlat();
  }
  else if (var == "SOS"){
    return getSOS();
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
  SerialGPS.begin(115200);

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
  server.on("/index.html", HTTP_GET, onIndexRequest);
  server.on("/style.css", HTTP_GET, onCSSRequest);
  server.on("/sos.html", HTTP_GET, onsosRequest);
  server.on("/java_s.js", HTTP_GET, onjsRequest);
  server.onNotFound(onPageNotFound);

  server.begin();
  dht.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);


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

  server.on("/lng", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getlng().c_str(),processor);
  });

  server.on("/lat", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getlat().c_str(),processor);
  });

  server.on("/sos", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getSOS().c_str(),processor);
  });
  
///////////////////////////////////////////////////////////////////////////////
  
}

void loop() {
  webSocket.loop();
  getlng();
  getlat();
  
  ranTemp();
  ranBPM();
  ranLoc();

    getTemperature();
    getB_Temperature();
    getHumidity();


delay(1000);
  
  
 
  
}
