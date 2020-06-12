// WiFi
#include <ESP8266WiFi.h>
#include <Ticker.h>
// Webserver
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Time managment
#include <Time.h>

// WiFi
#define WIFI_SSID "!!SSID!!"
#define WIFI_PASSWORD "!!PASSWORD!!"
#define WIFI_HOSTNAME "ESP_API_Webserver"

// Set up Web server
AsyncWebServer server(80);
// Set up WiFi events
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

// Time
struct tm * timeinfo;
// IP values
IPAddress MyIP;
IPAddress MyGW;

// Serial input
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;     // whether the string is complete
// Arrays to hold API data
// Max 6 AVPs with 10 chars each
char AttributeName[10][10];
char ValueName[10][10];
byte TotAttrs = 0;

// Handle WiFi
void connectToWifi() {
  Serial.println("ESP_API:Connecting to Wi-Fi");
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("ESP_API:Connected to Wi-Fi");
  MyIP = WiFi.localIP();
  MyGW = WiFi.gatewayIP();
  Serial.print("ESP_API:IP Address ");
  Serial.println(MyIP);
  Serial.print("ESP_API:GW Address ");
  Serial.println(MyGW);
  // Start Web server
  server.begin();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("ESP_API:Disconnected from Wi-Fi");
  WiFi.disconnect(true);
  wifiReconnectTimer.once(2, connectToWifi);
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("ESP_API:Webserver start up");

  inputString.reserve(50);

  // Set up WiFi event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  // Start WiFi for the first time
  connectToWifi();

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Automatically sync UTC time using NTP
  configTime(0, 0, "pool.ntp.org");
  //  Time zone description
https://sites.google.com/a/usapiens.com/opnode/time-zones
  setenv("TZ", "NZST-12NZDT-13,M10.1.0/02:00:00,M3.3.0/03:00:00", 1);
  tzset();

  // Webserver handler
  // Send a GET request to <IP>/?message=<message>
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String message;
    String myTime;
    // Get Time
    time_t now = time(nullptr);
    timeinfo = localtime(&now);
    myTime = String(asctime(timeinfo));
    myTime.trim();
    message =   "{\n\"ESP_RSSI_dBm\":" + String(WiFi.RSSI()) + ",\n";
    message +=  "\"ESP_Time\":\"" + myTime + "\"";
    for (byte i = 0; i < TotAttrs; i++) {
      message += ",\n\"" + String(AttributeName[i]) + "\":" +
                 String(ValueName[i]);
    }
    message +=  "\n}";
    request->send(200, "text/plain",  message );
  });
  server.onNotFound(notFound);

  Serial.println("ESP_API:Webserver start up complete");

}

void loop() {
  if (Serial.available()) serialEvent();
  if (stringComplete) {
    Serial.print("ESP_API:InputString-");
    Serial.println(inputString);
    // Parse string into seperate arrays
    analyzeString(inputString.c_str());
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();  // get the new byte:
    if (inChar == '\n') {
      stringComplete = true;            // if the incoming character is
      a newline, set the flag
    } else inputString += inChar;       // add it to the inputString:
  }
}

void analyzeString ( const char* attr )
{
  char*  value ;                                 // Points to value
after : in command

  value = strstr ( attr, ":" ) ;                 // See if command
  contains a ":"
  if ( value )
  {
    *value = '\0' ;                              // Separate command
    from value
    value++ ;                                    // Points to value
    after ":"
  }
  else
  {
    value = (char*) "0" ;                        // No value, assume zero
  }
  AddToAVPArray ( attr, value ) ;                // Update AVP array
}

void AddToAVPArray ( const char* atr, const char* val )
{
  String attr;
  String value;
  bool FoundOne = false;
  byte ThisOne = 0;
  attr = String(atr);
  attr.trim();
  value = String(val);
  value.trim();

  Serial.print("ESP_API:Attribute-");
  Serial.println(attr);
  Serial.print("ESP_API:Value-");
  Serial.println(value);

  for (byte i = 0; i < TotAttrs; i++) {
    if ( attr == String(AttributeName[i]) ) {
      FoundOne = true;
      ThisOne = i;
    }
  }
  if (!FoundOne) { // Not found so add
    //Serial.println(TotAttrs);
    attr.toCharArray(AttributeName[TotAttrs], 9);
    value.toCharArray(ValueName[TotAttrs], 9);
    TotAttrs ++;
    if (TotAttrs > 9) TotAttrs = 9;
  } else { // Have one already
    value.toCharArray(ValueName[ThisOne], 9);
  }

  //  for (byte i = 0; i < 6; i++) {
  //    Serial.print(i);
  //    Serial.print(" ");
  //    Serial.print(AttributeName[i]);
  //    Serial.print(" ");
  //    Serial.println(ValueName[i]);
  //  }
}
