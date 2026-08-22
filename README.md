# System for Preventing Pediatric Vehicular Heatstroke (PVH) and Vehicle Theft with Remote Monitoring

An IoT-enabled automotive safety and security architecture deployed on the **ESP32 Dev Module / ESP32-CAM**. The system features a novel dual-mode operation: **Preventive Vehicle Heatstroke (PVH) Mode** for child occupant protection and **Anti-Theft Mode** for intrusion deterrence, visual proof collection, and GPS tracking via AWS Cloud.

## Authors & Affiliation
- **V.R. Ganapathy Raja** (VIT Vellore)
- **Sasshank Anand** (23BEC0099, VIT Vellore)
- **Ramani Srinivasan** (VIT Vellore)
- **Mrityunjay Tiwari** (School of Electronics Engineering, VIT Vellore)
- **Dr. Sujata R** (Faculty Advisor, School of Electronics Engineering, VIT Vellore)

## Dual-Mode Operation

### 1. Pediatric Vehicular Heatstroke (PVH) Mode[cite: 4]
- **Trigger:** Roof-mounted PIR motion sensor detects occupant movement inside a locked car with ignition OFF[cite: 4].
- **Actuation:** Micro servo motor partially actuates the window regulator to create ventilation and reduce cabin heat build-up[cite: 4].
- **Cloud Alert:** Captures occupant photo via ESP32-CAM and transmits a `PVH ALERT` package with GPS coordinates to AWS IoT / SES[cite: 4].

### 2. Anti-Theft & Intrusion Detection Mode[cite: 4]
- **Trigger:** SW-420 vibration sensor detects impact/shock (e.g., glass breakage or forced entry)[cite: 4].
- **Actuation:** Sounds high-decibel local buzzer/horn alarm and starts 2 km geofencing tracking[cite: 4].
- **Cloud Alert:** Captures intruder photo, streams GPS telemetry to AWS S3, and sends immediate email/SMS notifications[cite: 4].

## Hardware Pinout Configuration (ESP32)
| Component | ESP32 GPIO Pin | Function |
| :--- | :--- | :--- |
| **PIR Motion Sensor** | `GPIO 14` | In-cabin child/intruder movement detection[cite: 4] |
| **SW-420 Vibration Sensor** | `GPIO 27` | Shock, glass break, and tampering detection[cite: 4] |
| **Servo Motor** | `GPIO 13` | Automatic window ventilation actuator[cite: 4] |
| **GPS Module (NEO-6M RX)** | `GPIO 16` (`RXD2`) | NMEA latitude/longitude stream (UART2)[cite: 4] |
| **GPS Module (NEO-6M TX)** | `GPIO 17` (`TXD2`) | GPS serial transmit[cite: 4] |
| **Status Red / Green LEDs** | `GPIO 12` / `GPIO 2` | System self-diagnosis and operational state[cite: 4] |
| **Buzzer / Alarm** | `GPIO 4` | Audible local deterrence[cite: 4] |

## Repository Architecture
```text
├── firmware/
│   ├── vehicle_security_pvh.ino   # Main embedded application firmware (C++)
│   ├── config.h                   # Pinout mappings and AWS IoT endpoint configs
│   └── camera_handler.h           # ESP32-CAM frame buffer & image capture logic
├── cloud/
│   └── aws_lambda_alert.py        # AWS Lambda function for S3 upload & SES alerts
├── requirements.txt
└── README.md
```
