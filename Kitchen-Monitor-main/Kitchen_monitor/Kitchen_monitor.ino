#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WebServer.h>
#include "ThingSpeak.h"

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
const char* ssid= "realme 3 Pro";
const char* pass="khusi123";
unsigned long channel_number = 1561693;            // Channel ID
const char * write_api_key = "Z0MW4UJW1TBTPE96"; // Read API Key
int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
int fsrVoltage;     // the analog reading converted to voltage
double fsrResistance;  // The voltage converted to resistance
double fsrConductance; 
double fsrForce;       // Finally, the resistance converted to force
float weight;

WiFiClient  client;
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;


void setup() {
  Serial.begin(9600);
  pinMode(fsrPin,INPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  Serial.println("Connecting to Wifi");
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Wifi Connected");
  ThingSpeak.begin(client);
}

void find_weight(){
  fsrReading = analogRead(fsrPin);
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  if (fsrVoltage == 0) {
    Serial.println("No pressure");  
  } else {
    fsrResistance = ((5000 - fsrVoltage)*10000)/fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);      
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);            
    }
    weight=fsrForce/9.81;
  }
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "6713017420") {
      find_weight();
      Serial.println(weight);
      ThingSpeak.writeField(channel_number, 1, weight, write_api_key);
    } 
    else if(tag == "2331965140") {
      find_weight();
      Serial.println(weight);
      ThingSpeak.writeField(channel_number, 2, weight, write_api_key);
    }
    else if(tag == "18423624039") {
      find_weight();
      Serial.println(weight);
      ThingSpeak.writeField(channel_number, 3, weight, write_api_key);
    }
    else if(tag == "2498621663") {
      find_weight();
      Serial.println(weight);
      ThingSpeak.writeField(channel_number, 4, weight, write_api_key);
    }
    else if(tag == "21812519690") {
      find_weight();
      Serial.println(weight);
      ThingSpeak.writeField(channel_number, 5, weight, write_api_key);
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
