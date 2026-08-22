#ifndef CONFIG_H
#define CONFIG_H

// --- Hardware Pin Definitions ---
static const int PIR_PIN = 14;          // GPIO 14 for PIR sensor output
static const int VIBRATION_PIN = 27;    // GPIO 27 for SW-420 vibration sensor
static const int SERVO_PIN = 13;        // GPIO 13 for Servo motor signal
static const int BUZZER_PIN = 4;        // GPIO 4 for alarm buzzer
static const int LED_FAULT_RED = 12;    // Fault indication LED
static const int LED_STATUS_GREEN = 2;  // Ready state LED

// --- GPS Hardware Serial 2 (UART2) ---
#define RXD2 16
#define TXD2 17
static const uint32_t GPS_BAUD = 9600;

// --- System Thresholds & Parameters ---
#define GEOFENCE_RADIUS_KM 2.0
#define WINDOW_VENT_ANGLE 45            // Servo angle for partial window opening
#define STATUS_PRINT_INTERVAL_MS 5000   // Periodic serial telemetry rate

#endif // CONFIG_H
