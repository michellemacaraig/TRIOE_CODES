include <SPI.h>
#include <LoRa.h>

#define SS 5    // LoRa SS pin
#define RST 14    // LoRa RST pin
#define DIO0 2   // LoRa DIO0 pin
#define BUTTON_2 13  // Button 2 (Trigger Move to 130°)
#define BUTTON_3 15  // Button 3 (Trigger Move to 180°)

void setup() {
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("[LoRa 1] LoRa module initialization failed!");
    while (1);
  }

  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);

  Serial.println("[LoRa 1] LoRa module initialized successfully!");
}

void loop() {
  if (digitalRead(BUTTON_2) == LOW) {
    Serial.println("[LoRa 1] Button 2 Pressed! Sending MOVE_130 command to LoRa 2...");
    LoRa.beginPacket();
    LoRa.print("MOVE_130");
    LoRa.endPacket();
    delay(500); // Debounce delay
  }

  if (digitalRead(BUTTON_3) == LOW) {
    Serial.println("[LoRa 1] Button 3 Pressed! Sending MOVE_180 command to LoRa 2...");
    LoRa.beginPacket();
    LoRa.print("MOVE_180");
    LoRa.endPacket();
    delay(500); // Debounce delay
  }
}