#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#define NEOPIXEL_PIN 4     // Pin connected to the NeoPixels
#define NUM_PIXELS   16    // Number of NeoPixels
#define DELAY_MS     100   // Animation delay in milliseconds

Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define SS_PIN 5 // SDA / SS is connected to GPIO 21
#define RST_PIN 27 // RST is connected to GPIO 22
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

#define ON_Board_LED 2 // Defining an On Board LED, used for indicators when the process of connecting to a WiFi router

const char *ssid = "Dialog 4G 877";
const char *password = "b85722e7";

WiFiServer server(80); // Server on port 80

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

void setup()
{
    Serial.begin(115200); // Initialize serial communications with the PC
    SPI.begin();          // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522 card

    delay(500);

    WiFi.begin(ssid, password); // Connect to your WiFi router
    Serial.println("");

    pinMode(ON_Board_LED, OUTPUT);
    digitalWrite(ON_Board_LED, HIGH); // Turn off LED On Board

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(ON_Board_LED, LOW);
        delay(250);
        digitalWrite(ON_Board_LED, HIGH);
        delay(250);
    }
    digitalWrite(ON_Board_LED, HIGH);

    Serial.println("");
    Serial.print("Successfully connected to : ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Please tag a card or keychain to see the UID !");
    Serial.println("");
}

void loop()
{// Run loading animation
 loadingAnimation(0x00, 0x00, 0xFF, 200); // Blue color loading animation
    readsuccess = getid();

    if (readsuccess)
    {
        digitalWrite(ON_Board_LED, LOW);
        HTTPClient http; // Declare object of class HTTPClient

        String UIDresultSend, postData;
        UIDresultSend = StrUID;

        // Post Data
        postData = "UIDresult=" + UIDresultSend;

        http.begin("http://192.168.8.190/pathfinder/getUID.php"); // Specify request destination
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");              // Specify content-type header

        int httpCode = http.POST(postData); // Send the request
        String payload = http.getString();  // Get the response payload

        Serial.println(UIDresultSend);
        Serial.println(httpCode); // Print HTTP return code
        Serial.println(payload);  // Print request response payload

        http.end(); // Close connection
        delay(1000);
        digitalWrite(ON_Board_LED, HIGH);

          //----------------------------------------------------------
          //success loading animation
          
        loadingAnimation(0x00, 0xFF, 0x00, 50); // Green color loading animation
        delay(50);
        loadingAnimation(0x00, 0xFF, 0x00, 50); // Green color loading animation

    }
}


//---------------------------------------------------------------------------------
// Loading animation function
void loadingAnimation(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
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
//--------------------------------------------------

int getid()
{
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return 0;
    }
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return 0;
    }

    Serial.print("THE UID OF THE SCANNED CARD IS : ");

    for (int i = 0; i < 4; i++)
    {
        readcard[i] = mfrc522.uid.uidByte[i]; // Storing the UID of the tag in readcard
        array_to_string(readcard, 4, str);
        StrUID = str;
    }
    mfrc522.PICC_HaltA();
    return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
    }
    buffer[len * 2] = '\0';
}
