/*
 * ESP32 + Blynk IoT + Ultrasonic Sensor + 2 LED Control
 * 
 * Fitur:
 * - Membaca jarak dari sensor ultrasonik HC-SR04
 * - Mengirim data jarak ke Blynk App (Virtual Pin V0)
 * - 2 LED dikontrol dari Blynk App (Virtual Pin V1 & V2)
 * - LED indikator status WiFi pada GPIO 2 (built-in)
 */

/* ========== BLYNK CONFIGURATION ========== */
#define BLYNK_TEMPLATE_ID "YourTemplateID"      // Ganti dengan Template ID dari Blynk.Console
#define BLYNK_TEMPLATE_NAME "ESP32 Ultrasonic"   // Ganti dengan nama template
#define BLYNK_AUTH_TOKEN "YourAuthToken"         // Ganti dengan Auth Token dari Blynk

/* ========== LIBRARY ========== */
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* ========== WIFI CREDENTIALS ========== */
char ssid[] = "YourWiFiSSID";      // Ganti dengan nama WiFi
char pass[] = "YourWiFiPassword";  // Ganti dengan password WiFi

/* ========== PIN DEFINITIONS ========== */
#define TRIG_PIN 5      // Pin Trigger Ultrasonik
#define ECHO_PIN 18     // Pin Echo Ultrasonik
#define LED1_PIN 25     // LED 1 - Kontrol Blynk V1
#define LED2_PIN 26     // LED 2 - Kontrol Blynk V2
#define WIFI_LED 2      // Built-in LED ESP32 (indikator WiFi)

/* ========== VARIABLES ========== */
#define SOUND_SPEED 0.034  // Kecepatan suara dalam cm/us
long duration;
float distanceCm;
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 1000; // Baca sensor setiap 1 detik

/* ========== BLYNK VIRTUAL PINS ========== */
#define VPIN_DISTANCE V0   // Virtual pin untuk data jarak
#define VPIN_LED1 V1       // Virtual pin untuk LED 1
#define VPIN_LED2 V2       // Virtual pin untuk LED 2

/* ========== BLYNK CALLBACKS ========== */
// Callback saat tombol LED 1 di Blynk ditekan
BLYNK_WRITE(VPIN_LED1) {
  int pinValue = param.asInt();  // Baca nilai dari Blynk (0 atau 1)
  digitalWrite(LED1_PIN, pinValue);
  Serial.print("LED 1: ");
  Serial.println(pinValue ? "ON" : "OFF");
}

// Callback saat tombol LED 2 di Blynk ditekan
BLYNK_WRITE(VPIN_LED2) {
  int pinValue = param.asInt();  // Baca nilai dari Blynk (0 atau 1)
  digitalWrite(LED2_PIN, pinValue);
  Serial.print("LED 2: ");
  Serial.println(pinValue ? "ON" : "OFF");
}

/* ========== SETUP ========== */
void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  Serial.println("\n=== ESP32 Blynk Ultrasonic Project ===");

  // Konfigurasi pin mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  // Inisialisasi LED dalam keadaan mati
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(WIFI_LED, LOW);

  // Koneksi ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  Serial.println("Connected to Blynk!");
  digitalWrite(WIFI_LED, HIGH); // Nyalakan LED indikator WiFi
}

/* ========== FUNGSI BACA JARAK ========== */
float readDistance() {
  // Bersihkan trigPin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Kirim sinyal trigger HIGH selama 10 mikrodetik
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Baca durasi echo
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Hitung jarak dalam cm
  distanceCm = duration * SOUND_SPEED / 2;
  
  return distanceCm;
}

/* ========== LOOP ========== */
void loop() {
  // Jalankan Blynk (harus dipanggil terus-menerus)
  Blynk.run();

  // Baca sensor ultrasonik setiap interval tertentu
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;
    
    // Baca jarak
    distanceCm = readDistance();
    
    // Validasi hasil pembacaan (0-400 cm adalah rentang normal HC-SR04)
    if (distanceCm > 0 && distanceCm < 400) {
      // Kirim data ke Blynk
      Blynk.virtualWrite(VPIN_DISTANCE, distanceCm);
      
      // Tampilkan di Serial Monitor
      Serial.print("Jarak: ");
      Serial.print(distanceCm);
      Serial.println(" cm");
    } else {
      Serial.println("Error: Pembacaan tidak valid");
    }
  }
}
