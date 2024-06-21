#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 15  // Pin: D8
#define RST_PIN 0  // Pin: D3
long chipID;

// MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  // If a new card is present
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Reset CardID
    chipID = 0;

    mfrc522.PICC_ReadCardSerial();

    // Load chip ID into variable chipID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      chipID = ((chipID + mfrc522.uid.uidByte[i]) * 10);
    }

    // Print chip ID
    Serial.println(chipID);

    // Pause for 5 seconds to prevent multiple reads/executions
    delay(5000);
  }
}