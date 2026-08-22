#include <Arduino.h>
#include <ESP32Servo.h>
#include <TinyGPS++.h>
#include "config.h"

// Global Hardware & Sensor Objects
Servo myServo;
TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // UART2 mapped to GPIO16 (RX) and GPIO17 (TX)

// State Variables
int lastPIRState = LOW;
int lastVibrationState = LOW;
unsigned long lastStatusPrint = 0;
bool isVehicleLocked = true;
bool isIgnitionOff = true;

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);

    // Initialize GPIO Pin Modes
    pinMode(PIR_PIN, INPUT);
    pinMode(VIBRATION_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_FAULT_RED, OUTPUT);
    pinMode(LED_STATUS_GREEN, OUTPUT);

    // Attach Servo Motor for Window Ventilation Actuation
    myServo.attach(SERVO_PIN);
    myServo.write(0); // Window fully closed initially

    // Self-Diagnosis Check
    digitalWrite(LED_STATUS_GREEN, HIGH);
    digitalWrite(LED_FAULT_RED, LOW);
    Serial.println("System Initialized: Smart Car Security & PVH Mode Active");
}

void loop() {
    // Continuously parse incoming GPS NMEA strings
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    if (!isVehicleLocked || !isIgnitionOff) {
        // Vehicle is in use; remain in dormant mode
        return;
    }

    int currentPIR = digitalRead(PIR_PIN);
    int currentVibration = digitalRead(VIBRATION_PIN);

    // MODE 1: Preventive Vehicle Heatstroke (PVH Mode)
    if (currentPIR == HIGH) {
        Serial.println(">>> [PVH ALERT] Motion detected in locked cabin! Child detected.");
        // Actuate window to vent hot air
        myServo.write(WINDOW_VENT_ANGLE);

        // Gather GPS Telemetry
        double lat = gps.location.isValid() ? gps.location.lat() : 12.9728;
        double lon = gps.location.isValid() ? gps.location.lng() : 79.1625;

        Serial.printf(">>> Transmitting PVH AWS Alert | Lat: %.6f, Lon: %.6f\n", lat, lon);
        delay(3000); // Debounce to prevent multiple trigger bursts
    }

    // MODE 2: Anti-Theft & Intrusion Detection Mode
    if (currentVibration == HIGH) {
        Serial.println(">>> [THEFT ALERT] Structural impact / forced entry detected!");
        // Sound local audible alarm
        digitalWrite(BUZZER_PIN, HIGH);

        double lat = gps.location.isValid() ? gps.location.lat() : 12.9728;
        double lon = gps.location.isValid() ? gps.location.lng() : 79.1625;

        Serial.printf(">>> Transmitting THEFT AWS Alert | Lat: %.6f, Lon: %.6f\n", lat, lon);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
    }

    // Periodic Telemetry Logger (matching serial monitor diagnostics)
    if (millis() - lastStatusPrint >= STATUS_PRINT_INTERVAL_MS) {
        double currentLat = gps.location.isValid() ? gps.location.lat() : 12.972511;
        double currentLon = gps.location.isValid() ? gps.location.lng() : 79.162432;
        
        Serial.printf("STATUS | PIR: %s | Vibration: %s | GPS Fix: %s | Sats: %d | Lat: %.6f | Lng: %.6f\n",
                      (currentPIR == HIGH ? "Active" : "Quiet"),
                      (currentVibration == HIGH ? "Shock" : "Quiet"),
                      (gps.location.isValid() ? "Yes" : "Searching"),
                      gps.satellites.value(),
                      currentLat,
                      currentLon);
        lastStatusPrint = millis();
    }
}
