#include <WiFi.h>
#include <HTTPClient.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// RFID pins
#define SS_PIN 5
#define RST_PIN 4

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RFID setup
MFRC522 rfid(SS_PIN, RST_PIN);

// NeoPixel setup
#define NEOPIXEL_PIN 15     // Pin connected to the NeoPixels
#define NUM_PIXELS   16     // Number of NeoPixels
#define DELAY_MS     100    // Animation delay in milliseconds

Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// WiFi credentials and server URL
const char* ssid = "OnePlus";
const char* password = "Perjo5008";
const char* serverURL = "http://192.168.9.10:5000/getBoardData/";

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

  // Initialize NeoPixel
  pixels.begin();

  // WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  adminLogin();
  scanRFID();
}

void adminLogin() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Admin Key:");
  String enteredKey = "";

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        if (enteredKey == "1234") { // Change "1234" to your admin password
          lcd.clear();
          lcd.print("Login Successful");
          delay(2000);
          lcd.clear();
          return;
        } else {
          lcd.clear();
          lcd.print("Invalid Key");
          delay(2000);
          enteredKey = "";
          lcd.clear();
          lcd.print("Enter Admin Key:");
        }
      } else {
        enteredKey += key;
        lcd.setCursor(0, 1);
        lcd.print(enteredKey);
      }
    }
  }
}

void scanRFID() {
  lcd.clear();
  lcd.print("Scan RFID Card");
  while (true) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      String uid = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(rfid.uid.uidByte[i], HEX);
      }
      uid.toUpperCase();
      lcd.clear();
      lcd.print("RFID: " + uid);
      delay(2000);
      lcd.clear();
      lcd.print("Data Sending...");
      delay(1500);
      lcd.clear();


      sendToServer(uid);
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();

      lcd.print("Sent Success");
      delay("1000");
      lcd.clear();

      lcd.print("Scan RFID Card:")
    }
  }
}

void sendToServer(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    // Blue color loading animation before sending data
    loadingAnimation(0x00, 0x00, 0xFF);

    HTTPClient http;
    http.begin(serverURL); // Use the new endpoint URL
    http.addHeader("Content-Type", "application/json");

    // Prepare JSON payload
    StaticJsonDocument<200> doc;
    doc["RFIDKey"] = uid;

    String jsonData;
    serializeJson(doc, jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      // Green color animation for successful data transmission
      loadingAnimation(0x00, 0xFF, 0x00);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      // Red color animation for failed data transmission
      loadingAnimation(0xFF, 0x00, 0x00);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

//---------------------------------------------------------------------------------
// Loading animation function
void loadingAnimation(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    // Clear all pixels
    pixels.clear();

    // Set the pixels from 0 to i (inclusive) to the specified color
    for (int j = 0; j <= i; j++) {
      pixels.setPixelColor(j, pixels.Color(red, green, blue));
    }

    // Show the updated pixels
    pixels.show();

    // Delay between each pixel update
    delay(DELAY_MS);
  }
}

