#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "MFRC522.h"
/*
   PINOUT
   +--------------------------------+---------------+
   | WEMOS D1 ESP8266 BOARD         | CONECT TO PIN |
   +--------------------------------+---------------+
   | PIN  | FUCTION  | ESP-8266 PIN | RC522 | RELAY |
   +------+----------+--------------+-------+-------+
   | 3.3V | POWER    | 3.3V         | 3.3V  |       |
   +------+----------+--------------+-------+-------+
   | 5V   | POWER    | 5V           |       | VCC   |
   +------+----------+--------------+-------+-------+
   | GND  | GND      | GND          | GND   | GND   |
   +------+----------+--------------+-------+-------+
   | D13  | SCK      | GPIO-14      | SCK   |       |
   +------+----------+--------------+-------+       +
   | D12  | MISO     | GPIO-12      | MISO  |       |
   +------+----------+--------------+-------+       +
   | D11  | MOSI     | GPIO-13      | MOSI  |       |
   +------+----------+--------------+-------+       +
   | D10  | SS (SDA) | GPIO-15      | SDA   |       |
   +------+----------+--------------+-------+       +
   | D8   | IO       | GPIO-0       | RESET |       |
   +------+----------+--------------+-------+-------+
   | D2   | IO       | GPIO-16      |       | IN1   |
   +------+----------+--------------+-------+-------+
*/

#define RST_PIN 0 // RST-PIN for RC522 - RFID - SPI - Module GPIO-0 
#define SS_PIN  15  // SDA-PIN for RC522 - RFID - SPI - Module GPIO-15

const char* ssid     = "WiFi_InvitadosUPC";
const char* password = "S3CC10NALP1L0T0@2018";
const char* host = "www.jccapital.xyz";

const char* userpass = "xxxx";
int estado = 0;
String a, b, c, d;
int Aire =  D2;
int Videobeam =  D3;
int buzzer = D4;
int Luz =  D14;
int Ventiladores =  D15;
int estadoo = LOW;

WiFiServer server(80);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "" : ".");
    Serial.print(buffer[i], HEX);
    a = String(buffer[0], HEX);
    b = String(buffer[1], HEX);
    c = String(buffer[2], HEX);
    d = String(buffer[3], HEX);
  }
}

void entrada(int bin) {

  if (bin == 1) {

    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(Aire, LOW);
    delay(1000);
    digitalWrite(Videobeam, LOW);
    delay(1000);
    digitalWrite(Luz, LOW);
    delay(1000);
    digitalWrite(Ventiladores, LOW);
  } else {

    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
        digitalWrite(Aire, HIGH);
    delay(1000);
    digitalWrite(Videobeam, HIGH);
    delay(1000);
    digitalWrite(Luz, HIGH);
    delay(1000);
    digitalWrite(Ventiladores, HIGH);
    


  }

}

void setup() {

  pinMode(buzzer, OUTPUT);
  pinMode(Aire, OUTPUT);
  pinMode(Videobeam, OUTPUT);
  pinMode(Luz, OUTPUT);
  pinMode(Ventiladores, OUTPUT);
        digitalWrite(Aire, HIGH);
 
    digitalWrite(Videobeam, HIGH);

    digitalWrite(Luz, HIGH);

    digitalWrite(Ventiladores, HIGH);

  // Initialize serial communications
  Serial.begin(115200);
  delay(10);
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // WiFi.begin(ssid, password);

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  //server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  if (estado == 0) {
    estado = 1;
  } else {
    estado = 0;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.println("RFID Tag Detected...");
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  //server.handleClient();

  WiFiClient client = server.available();
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/Santa_Ana/funciones/carga.php?a=";
  url += a;
  url += "&b=";
  url += b;
  url += "&c=";
  url += c;
  url += "&d=";
  url += d;
  url += "&est=";
  url += estado;

  Serial.print("Requesting URL: ");
  Serial.println(url);
  /* importante para el proyecto manejarlo con ususario, genera mayor seguridad

    // This will send the request to the server
    client.println(
    String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" + "Authorization: Basic " + userpass + "\r\n" +
    "Connection: close\r\n\r\n");
  */
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: www.jccapital.xyz");
  client.println("Connection: close"); // este es como un fin de consulta de GET
  client.println();
  entrada(estado);

  /*
    // This will send the request to the server
    client.print("GET " + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  */
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  /*
    // Read all the lines of the reply from server and print them to Serial
    while (client.connected()) {
      String response = client.readStringUntil('\n');
      if (response == "\r") {
        Serial.println("Headers Received:");
        break;
      }
    }
    String response = client.readStringUntil('\n');
    Serial.println(response);

    if (response.indexOf("true") >= 0 )
    {
      Serial.println("Access Granted");
      digitalWrite(RELAY_PIN, HIGH); //Relay ON
      Serial.println("Relay Activated");
      delay(10);
      digitalWrite(RELAY_PIN, LOW); //Relay OFF
    }
    else if (response.indexOf("null") >= 0 )
    {
      Serial.println("Access Unidentified");
    }
    else {
      Serial.println("False");
    }*/

  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  delay(5000);
}
