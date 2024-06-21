#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SS_PIN 15  // Pin: D8
#define RST_PIN 0  // Pin: D3
#define SPEAKER_PIN A0  // Pin for the speaker
const int speakerPin = T2; // Adjust this to match your setup
long chipID;

// WiFi credentials
const char* ssid = "DEIN_SSID";
const char* password = "DEIN_PASSWORT";

// IFTTT Webhook settings
const char* host = "maker.ifttt.com";
const char* eventName = "send_email";
const char* iftttKey = "Hqo44H7KFakeQz9PAzMit";

// MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

  // Set up the speaker pin
  pinMode(SPEAKER_PIN, OUTPUT);

  // Initialize SPI bus
  SPI.begin();

  // Initialize MFRC522
  mfrc522.PCD_Init();

  // Short delay after initialization
  delay(10);
  Serial.println("");

  // Print details from MFRC522 RFID reader/writer
  mfrc522.PCD_DumpVersionToSerial();

  // Set speaker pin as an output
  pinMode(speakerPin, OUTPUT);

  Serial.println("Place RFID chip to display UID...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void loop() {
  // If a new card is present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Reset CardID
    chipID = 0;

    // Load chip ID into variable chipID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      chipID = ((chipID + mfrc522.uid.uidByte[i]) * 10);
    }

    // Print chip ID
    Serial.println(chipID);

    if (chipID == 643060) {  // Wenn der kleine Chip gescannt wird (Richtiger Scan)
      tone(speakerPin, 523, 200);
      delay(225);
      tone(speakerPin, 1047, 200);
    }

    if (chipID == 2110000) { // Wenn die Karte gescannt wird (Falscher scan)
      tone(speakerPin, 523, 200);
      delay(225);
      tone(speakerPin, 262, 200);
      sendIFTTTRequest(); // Sende eine E-Mail
    }

    // Generate a 1000 Hz tone for 1000 ms (1 second)
    tone(speakerPin, 250, 250);

    // Pause for 5 seconds to prevent multiple reads/executions
    delay(5000);
  }
}

void sendIFTTTRequest() {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection status
    HTTPClient http;
    String url = String("http://") + host + "/trigger/" + eventName + "/with/key/" + iftttKey;
    http.begin(url);
    int httpCode = http.GET(); // Sende HTTP GET Anfrage

    if (httpCode > 0) {
      Serial.printf("IFTTT trigger sent, response code: %d\n", httpCode);
    } else {
      Serial.printf("Error sending IFTTT trigger, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
