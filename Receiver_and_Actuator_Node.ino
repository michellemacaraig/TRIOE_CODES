#include <SPI.h>
#include <LoRa.h>
#include <ESP32Servo.h>

#define SS 5
#define RST 14
#define DIO0 2
#define SERVO_PIN 15

Servo myservo;
unsigned long previousMillis = 0;
unsigned long countdownMillis = 0;
int state = 0;  // 0 = Startup, 1 = 130° for 2 mins, 2 = 180° for 30 mins
bool waitingToMove = true;

void setup() {
  Serial.begin(115200);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("[LoRa 3] LoRa initialization failed!");
    while (1);
  }

  myservo.attach(SERVO_PIN);

  // Start sequence
  myservo.write(180);
  Serial.println("[LoRa 3] Moving Servo: 180° (Startup, 2s)");
  delay(2000);

  myservo.write(130);
  Serial.println("[LoRa 3] Moving Servo: 130° (Start 2 min countdown)");
  previousMillis = millis();
  countdownMillis = millis();
  state = 1;
}

void loop() {
  unsigned long currentMillis = millis();

  // Countdown display every 5 seconds
  if (currentMillis - countdownMillis >= 5000) {
    int remainingTime = (state == 1) ? 
                        (120 - (currentMillis - previousMillis) / 1000) : 
                        (1800 - (currentMillis - previousMillis) / 1000); // 30 mins = 1800s
    Serial.print("[LoRa 3] Time remaining: ");
    Serial.print(remainingTime);
    Serial.println("s");
    countdownMillis = millis();
  }

  // State machine for automatic movement
  if (state == 1 && currentMillis - previousMillis >= 120000) {  // 2 minutes
    myservo.write(180);
    Serial.println("[LoRa 3] Moving Servo: 180° (Start 30 min countdown)");
    previousMillis = millis();
    state = 2;
  } else if (state == 2 && currentMillis - previousMillis >= 1800000) {  // 30 minutes
    myservo.write(130);
    Serial.println("[LoRa 3] Moving Servo: 130° (Restarting cycle)");
    previousMillis = millis();
    state = 1;
  }

  // LoRa message handling
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String message = "";
    while (LoRa.available()) {
      message += (char)LoRa.read();
    }

    Serial.println("[LoRa 3] Received: " + message);

    if (message == "MOVE_130") {
      myservo.write(130);
      Serial.println("[LoRa 3] Moving Servo to 130° (Restarting cycle)");
      previousMillis = millis();
      state = 1;
    } 
    else if (message == "MOVE_180") {
      myservo.write(180);
      Serial.println("[LoRa 3] Moving Servo to 180° (Restarting cycle)");
      previousMillis = millis();
      state = 2;
    }
  }
}
