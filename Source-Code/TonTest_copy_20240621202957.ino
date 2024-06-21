// Define the pin connected to the speaker
const int speakerPin = D2; // Adjust this to match your setup

void setup() {
  // Set speaker pin as an output
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // Generate a 1000 Hz tone for 1000 ms (1 second)
  Serial.println("Playing Tone!");
  tone(speakerPin, 1000, 1000);

  delay(2000); // Wait for 2 seconds before generating the next tone
}