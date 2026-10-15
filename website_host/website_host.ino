#include <ESP8266WiFi.h>

const char* ssid = "FLPublic";
const char* password = "M4k3rsp4c3#Freilab2022";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println();
  Serial.print("Verbinde mit ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("LAN verbunden!");

  server.begin();
  Serial.println("Server gestartet.");

  Serial.print("Nutze diese URL zum Verbinden: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Neuer Client verbunden");
  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = HIGH;
  if (request.indexOf("/LED=AN") != -1) {
    digitalWrite(LED_BUILTIN, LOW);
    value = LOW;
  }
  if (request.indexOf("/LED=AUS") != -1) {
    digitalWrite(LED_BUILTIN, HIGH);
    value = HIGH;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // Wichtige Leerzeile
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta charset='utf-8'><title>NodeMCU LED Steuerung</title></head>");
  client.println("<body style='text-align:center; font-family:sans-serif; margin-top:50px;'>");
  client.println("<h1>NodeMCU WLAN-Steuerung</h1>");

  if(value == LOW) {
    client.println("<p>Die blaue LED ist aktuell: <b>AN</b></p>");
  } else {
    client.println("<p>Die blaue LED ist aktuell: <b>AUS</b></p>");
  }
  
  client.println("<br><br>");
  client.println("<a href='/LED=AN' style='padding:15px 25px; background-color:green; color:white; text-decoration:none; font-size:20px; border-radius:5px;'>LED EINSCHALTEN</a>");
  client.println("<br><br><br>");
  client.println("<a href='/LED=AUS' style='padding:15px 25px; background-color:red; color:white; text-decoration:none; font-size:20px; border-radius:5px;'>LED AUSSCHALTEN</a>");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client getrennt");
  Serial.println("");
}
