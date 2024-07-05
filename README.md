# ESP32 RFID Indoor Map System
This project is an RFID-based Indoor Mapping system built using an ESP32 microcontroller. 
This project** has a web-based Tracking system**, once a visitor taps on to an RFID card, he can see his current location and destination on the nearby screen.
This Project addresses the difficulties of indoor tracking when using Google Maps.
** Visitors Can go in between floors and find out specific locations inside a building**
The system integrates a **keypad, LCD, RFID module, and NeoPixel LEDs** to manage visitor registrations efficiently. The project connects to a WiFi network and sends the RFID data to a specified server endpoint.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Components](#components)
- [Circuit Diagram](#circuit-diagram)
- [Installation](#installation)
- [Usage](#usage)
- [Code Overview](#code-overview)
- [Contributing](#contributing)
- [License](#license)

## Introduction
This project is designed to register visitors using RFID cards.
An administrator logs in using a keypad before scanning RFID cards.
Admin Can register the visitor by taking his data, and the location he needs to visit.
The programmed RFID card will be given to the user.
The RFID data is then sent to a server endpoint for processing. The system also uses NeoPixel LEDs to provide visual feedback during the registration process.

Users can use checkpoints created inside the uni premises to findout where he/ she is now and where to head up.

## Features
**RFID-based registration**: Each visitor uses an RFID card for registration.

**Admin login**: Admin must log in using a keypad before starting the registration.

**WiFi connectivity**: Sends RFID data to a server endpoint over WiFi.

**Visual feedback**: NeoPixel LEDs provide visual feedback for various stages of the process.

**LCD display**: Provides information and instructions to the user.

## Components
ESP32 microcontroller

MFRC522 RFID module

4x4 Matrix Keypad

16x2 I2C LCD display

NeoPixel LED strip (16 LEDs)

Miscellaneous components (wires, breadboard, resistors, etc.)

## Circuit Diagram

### Pin Configuration
_RFID_

SDA/SS: GPIO 5

RST: GPIO 4

_Keypad_

Row Pins: GPIO 13, 12, 14, 27

Column Pins: GPIO 26, 25, 33, 32
_
LCD_

SDA: GPIO 21

SCL: GPIO 22

_NeoPixel_

Data Pin: GPIO 15

_Diagram_ : in the Gerber files

## Installation
Clone the repository:
```bash
https://github.com/akindukodithuwakku/level-01-hardware-project/tree/main
```

**Install necessary libraries:**

Ensure you have the following libraries installed in your Arduino IDE:
- WiFi
- HTTPClient
- Keypad
- LiquidCrystal_I2C
- MFRC522
- Adafruit_NeoPixel
- ArduinoJson


**Open the project:**

Open `RFID_MainGate_Final.ino` file in the Arduino IDE.


**Configure WiFi credentials:**
Update the following lines with your WiFi credentials:

```cpp
const char* ssid = "your-ssid";
const char* password = "your-password";
const char* serverURL = "http://yourserver.com/getBoardData/";
```

**Upload the code:**
Connect your ESP32 to the computer and upload the code.

## Usage
1. **Power on the system:**

   Please make sure the ESP32 is powered and connected to the WiFi network.

2. **Admin login:**

   Enter the admin password using the keypad.

3. **Scan RFID card:**

   Place the RFID card near the RFID module to scan. The system will send the RFID data to the server endpoint and provide visual feedback using the NeoPixel LEDs.


## Code Overview

- **setup():** Initializes the LCD, RFID module, NeoPixel LEDs, and WiFi connection.
- **loop():** Manages the admin login and RFID scanning processes.
- **adminLogin():** Handles the admin login process using the keypad.
- **scanRFID():** Scans the RFID card and sends the data to the server.
- **sendToServer():** Sends the scanned RFID data to the server endpoint using HTTP POST requests.
- **loadingAnimation():** Provides visual feedback using NeoPixel LEDs.


Contributing
Contributions are welcome! Please fork the repository and submit a pull request.

License
This project is licensed under the Creative Common V2 License. See the LICENSE file for details.
