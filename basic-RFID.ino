//A thing made by Daniel. https://danielgray.me

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
char* myTags[100] = {"YOUR CARD UID"};
byte readCard[4]; 
String tagID = "";
constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above
Servo myservo;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
int accepted = 0;

void setup() {
  myservo.attach(6);
  myservo.write(10);
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  Serial.println("RFID Door Access Control. Made by Daniel.");
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  tagID = "";
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

    for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  for (int i = 0; i < 2; i++) {
    if (tagID == myTags[i]) {
    Serial.println("ACCESS GRANTED");
    openDoor();
    return;
  }
 }
if(accepted == 0) {
  digitalWrite(2, HIGH);
  Serial.println("Access Denied!");
  delay(2000);
  digitalWrite(2, LOW);
}
}

void openDoor() {
    digitalWrite(3, HIGH);
    myservo.write(180);
    delay(6000);
    digitalWrite(3, LOW);
    myservo.write(10);
}


