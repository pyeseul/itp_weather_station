/*  This is a sketch that access the ITP Weather Station's database 
 *  and get weather data for selected dates.
 *  Modified by Yeseul Song & Arnab Chakravarty for the ITP Weather Band
 *  Based on Tom Igoe's Connected Devices example
 *  April 2020
*/
 
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h> // you might want to use WiFi101.h instead depending on which arduino you're using

#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
String mac_identity = MAC_ID;
String session_identity = SESSION_KEY;

const char serverAddress[] = "tigoe.io";  // server address
int port = 443;  // for https

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);     // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {  
  
  getData();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(100000);
}

void getData() {
  
  // add the query to the route:
  String path = "/itpower-data/by-time";
  String contentType = "application/json";

  String dateFrom = "4-26-2020 23:20:00";
  String dateTo = "4-26-2020 23:30:00";
  
  // Assembling unique creadentials to access server to get data
  String mac = "\"macAddress\":\"" + mac_identity + "\"";
  String session = "\"sessionKey\":" + session_identity + "";
  String dFrom = "\"dateFrom\":\"" + dateFrom + "\"";
  String dTo = "\"dateTo\":\"" + dateTo + "\"";  

  //Combining GET request data as a string object
  String getBody = "{" + mac + "," + session + "," + dFrom + "," + dTo + "}";

  Serial.println(path);
  Serial.println(getBody);
  
  // make the request:
  client.beginRequest();
  client.get(path);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", getBody.length());
  client.beginBody();
  client.print(getBody);
  client.endRequest();
  
}
