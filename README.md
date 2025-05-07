# LiFi Access Control

This repository includes two Approaches and one testing setup:

1. **Approaches**
   - **Approach 1 (USED IN PAPER)**
     - Arduino Uno + LDR decoder → ESP8266 gateway → Firebase → Node.js service
     - Path: `Approach 1 (USED IN PAPER)/`
   - **Approach 2 (working on with only ESP)**
     - ESP8266 directly reads LDR → Firebase → Node.js service
     - Path: `Approach 2 (working on with only ESP)/`
     - note: try to reduce the time taken by the system to process and send data

2. **Testing Data Collection**
   - Instrumented Arduino Uno sketches to log SEND/RECV events and a 50-trial summary
   - Path: `Test data collection files/`

---

## Getting Started

```bash
# Clone the repo
git clone https://github.com/AyushNP03/LiFi-AccessControl-System.git
cd LiFi-AccessControl-System

# Install Node.js deps for both approaches
cd "Approach 1 (USED IN PAPER)/VSCloud(acces)"
npm install
```

### Running Approach 1

1. Flash **transmitter/transmitter.ino** to the Arduino LED sender.
2. Flash **UNODecoder/UNODecoder.ino** to the Arduino LDR decoder.
3. Flash **UNODecoder/ESPGateway/ESPGateway.ino** to the ESP8266.
4. In `VSCloud(acces)`:
   ```bash
   node accessControl.js
   ```
5. Trigger a transmission and watch `/accessLogs` in Firebase Console.

---

### Firebase Configuration

To match the deployed Firebase backend used in the system:

- `firebase.json`: Firebase project configuration
- `database.rules.json`: Security rules (read/write settings)
- `lifiaccesscontrol-default-rtdb-export.json`: Sample seed structure for Realtime DB

You can find these files inside:
```
Approach 1 (USED IN PAPER)/Firebase/
Approach 2 (working on with only ESP)/Firebase/
```

---

### Testing Data Collection

See **Test data collection files/README.md** for how to run the 50-trial accuracy/FPR/latency test.

## Tools & Environment

- **Arduino IDE** (1.8+)
- **ESP8266 Board Package** for Arduino
- **Firebase Console** (Realtime Database)
- **Node.js** (v14+)
- **npm**
- **VS Code** (used for development)
- **Serial Monitor** (for data collection tests)

## Hardware Components

- **Arduino Uno** 
- **ESP8266 NodeMCU** (used in both approaches)
- **LDR Sensor (Photoresistor)** (for light detection)
- **LEDs** (for transmitting OOK + CAP signals)
- **Jumper wires** (for circuit connections)
- **Breadboard** (for prototyping circuits)
- **USB cables** (for programming and powering boards)
