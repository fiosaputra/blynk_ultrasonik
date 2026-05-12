# 🤖 IoT ESP32 + Blynk: Ultrasonic Sensor & LED Control

Proyek IoT menggunakan **ESP32** dengan integrasi **Blynk IoT** untuk monitoring jarak via sensor ultrasonik HC-SR04 dan kontrol 2 LED secara real-time melalui aplikasi mobile.

---

## 📋 Daftar Isi

- [Fitur](#-fitur)
- [Komponen yang Dibutuhkan](#-komponen-yang-dibutuhkan)
- [Skema Rangkaian](#-skema-rangkaian)
- [Konfigurasi Pin](#-konfigurasi-pin)
- [Instalasi & Setup](#-instalasi--setup)
  - [Software](#software)
  - [Hardware](#hardware)
- [Konfigurasi Blynk IoT](#-konfigurasi-blynk-iot)
  - [Template Setup](#template-setup)
  - [Widget Configuration](#widget-configuration)
- [Kode Program](#-kode-program)
- [Cara Penggunaan](#-cara-penggunaan)
- [Troubleshooting](#-troubleshooting)
- [Lisensi](#-lisensi)

---

## ✨ Fitur

| Fitur | Deskripsi |
|-------|-----------|
| 📏 **Monitoring Jarak Real-time** | Membaca jarak dari sensor ultrasonik HC-SR04 dan menampilkannya di dashboard Blynk |
| 💡 **Kontrol LED dari Jarak** | 2 LED dapat dihidupkan/matikan melalui aplikasi Blynk dari mana saja |
| 📡 **Koneksi WiFi Otomatis** | ESP32 otomatis terhubung ke WiFi dan Blynk server |
| 🔧 **Konfigurasi Mudah** | Virtual Pin yang jelas dan terstruktur |
| 📊 **Data Logging** | Data jarak terekam di dashboard Blynk |

---

## 🛠️ Komponen yang Dibutuhkan

| No | Komponen | Jumlah | Spesifikasi |
|----|----------|--------|-------------|
| 1 | ESP32 Development Board | 1 | ESP32-WROOM-32 / ESP32 DevKit V1 |
| 2 | Sensor Ultrasonik HC-SR04 | 1 | 5V, Range 2cm - 400cm |
| 3 | LED | 2 | Warna bebas (Merah, Hijau, Biru, dll) |
| 4 | Resistor | 2 | 220Ω - 330Ω (current limiting LED) |
| 5 | Breadboard | 1 | - |
| 6 | Kabel Jumper | Secukupnya | Male-to-Male |
| 7 | Kabel USB | 1 | Micro USB / Type-C (data + power) |
| 8 | Power Supply (opsional) | 1 | 5V Adapter untuk operasi standalone |

---

## 🔌 Skema Rangkaian

```
ESP32 DevKit V1
┌─────────────────────────────┐
│                             │
│  3.3V  ──────┬────────────  │
│  5V    ──────┼──┬──┬──────  │  ← Power untuk HC-SR04
│  GND   ───┬──┼──┼──┼──┬──  │  ← Ground bersama
│  GPIO2 ───┼──┼──┼──┼──┼──  │  ← Built-in LED (WiFi Status)
│  GPIO5 ───┼──┼──┼──┼──┼──  │  ← TRIG (HC-SR04)
│ GPIO18 ───┼──┼──┼──┼──┼──  │  ← ECHO (HC-SR04)
│ GPIO25 ───┼──┼──┼──┼──┼──  │  ← LED 1 (Kontrol Blynk)
│ GPIO26 ───┼──┼──┼──┼──┼──  │  ← LED 2 (Kontrol Blynk)
│                             │
└─────────────────────────────┘

HC-SR04 Ultrasonic Sensor
┌─────────────┐
│  VCC  ──────┼──→ ESP32 5V
│  TRIG ──────┼──→ ESP32 GPIO5
│  ECHO ──────┼──→ ESP32 GPIO18
│  GND  ──────┼──→ ESP32 GND
└─────────────┘

LED 1 (GPIO25)
┌─────────────┐
│  Anoda(+)   ├──→ ESP32 GPIO25
│  Katoda(-)  ├──→ Resistor 220Ω ──→ GND
└─────────────┘

LED 2 (GPIO26)
┌─────────────┐
│  Anoda(+)   ├──→ ESP32 GPIO26
│  Katoda(-)  ├──→ Resistor 220Ω ──→ GND
└─────────────┘
```

### Diagram Koneksi Detail

| ESP32 Pin | Komponen | Pin Komponen | Fungsi |
|-----------|----------|--------------|--------|
| 5V / VIN  | HC-SR04  | VCC          | Power Supply Sensor |
| GND       | HC-SR04  | GND          | Ground Sensor |
| GPIO 5    | HC-SR04  | TRIG         | Trigger Signal |
| GPIO 18   | HC-SR04  | ECHO         | Echo Signal |
| GPIO 25   | LED 1    | Anoda (+)    | Kontrol LED 1 |
| GPIO 26   | LED 2    | Anoda (+)    | Kontrol LED 2 |
| GND       | LED 1    | Katoda (-) via 220Ω | Ground LED 1 |
| GND       | LED 2    | Katoda (-) via 220Ω | Ground LED 2 |
| GPIO 2    | Built-in | LED ESP32    | Indikator WiFi Status |

---

## 🔧 Konfigurasi Pin

```cpp
// Pin Definitions
#define TRIG_PIN 5      // Pin Trigger Ultrasonik
#define ECHO_PIN 18     // Pin Echo Ultrasonik
#define LED1_PIN 25     // LED 1 - Kontrol Blynk Virtual Pin V1
#define LED2_PIN 26     // LED 2 - Kontrol Blynk Virtual Pin V2
#define WIFI_LED 2      // Built-in LED ESP32 (indikator WiFi)
```

### Virtual Pin Mapping (Blynk)

| Virtual Pin | Widget Blynk | Fungsi | Tipe Data |
|-------------|--------------|--------|-----------|
| V0 | Gauge / Value Display | Menampilkan jarak (cm) | Float |
| V1 | Button / Switch | Kontrol LED 1 ON/OFF | Integer (0/1) |
| V2 | Button / Switch | Kontrol LED 2 ON/OFF | Integer (0/1) |

---

## 🚀 Instalasi & Setup

### Software

#### 1. Install Arduino IDE
- Download dari [arduino.cc](https://www.arduino.cc/en/software)
- Install versi terbaru (1.8.x atau 2.x)

#### 2. Install Board ESP32

Buka **Arduino IDE** → **File** → **Preferences** → tambahkan URL di **Additional Boards Manager URLs**:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Kemudian:
- **Tools** → **Board** → **Boards Manager**
- Cari "**ESP32 by Espressif Systems**"
- Klik **Install**

#### 3. Install Library Blynk

- **Sketch** → **Include Library** → **Manage Libraries**
- Cari "**Blynk** by Volodymyr Shymanskyy"
- Klik **Install**

Atau install via Library Manager dengan nama:
```
Blynk
```

#### 4. Install Library WiFi (built-in)
Library `WiFi.h` sudah tersedia secara default saat install board ESP32.

### Hardware

1. **Rakit rangkaian** sesuai skema di atas
2. **Perhatikan polaritas LED**:
   - Anoda (kaki panjang) → ke GPIO ESP32
   - Katoda (kaki pendek) → ke Resistor → GND
3. **Pastikan HC-SR04** mendapat supply 5V (bisa dari VIN ESP32 jika di-power via USB)
4. **Hubungkan ESP32 ke PC** via kabel USB

---

## 📱 Konfigurasi Blynk IoT

### Template Setup

1. Buka [Blynk.Console](https://blynk.cloud/) atau aplikasi **Blynk IoT**
2. Buat **Template Baru**:
   - **Template Name**: `ESP32 Ultrasonic`
   - **Hardware**: `ESP32`
   - **Connection Type**: `WiFi`
3. Tambahkan **Datastreams**:

| Datastream | Virtual Pin | Min Value | Max Value | Type |
|------------|-------------|-----------|-----------|------|
| Distance   | V0          | 0         | 400       | Double |
| LED1       | V1          | 0         | 1         | Integer |
| LED2       | V2          | 0         | 1         | Integer |

4. Simpan **Template ID**, **Template Name**, dan **Auth Token** untuk digunakan di kode

### Widget Configuration

#### Web Dashboard (Blynk.Console)

Tambahkan widget berikut di **Web Dashboard**:

| Widget | Datastream | Label | Settings |
|--------|-----------|-------|----------|
| **Gauge** | Distance (V0) | "Jarak (cm)" | Min: 0, Max: 400 |
| **Switch** | LED1 (V1) | "LED 1" | Mode: Switch |
| **Switch** | LED2 (V2) | "LED 2" | Mode: Switch |
| **Label** | Distance (V0) | "Status Jarak" | Format: `{0} cm` |

#### Mobile Dashboard (Blynk App)

Tambahkan widget berikut di **Mobile Dashboard**:

| Widget | Datastream | Properties |
|--------|-----------|------------|
| **Value Display** | V0 | Font: Large, Suffix: " cm" |
| **Button** | V1 | Mode: Switch, Color: Red |
| **Button** | V2 | Mode: Switch, Color: Green |

---

## 💻 Kode Program

### File Utama: `ESP32_Blynk_Ultrasonic.ino`

```cpp
/*
 * ╔═══════════════════════════════════════════════════════════════╗
 * ║  ESP32 + Blynk IoT + Ultrasonic Sensor + 2 LED Control      ║
 * ║  ---------------------------------------------------------  ║
 * ║  Membaca jarak dari sensor HC-SR04 dan mengirim ke Blynk   ║
 * ║  2 LED dikontrol ON/OFF dari aplikasi Blynk                 ║
 * ╚═══════════════════════════════════════════════════════════════╝
 * 
 * Author: [Nama Anda]
 * Date: 2026-05-12
 * Board: ESP32 DevKit V1
 */

/* ========== BLYNK CONFIGURATION ========== */
// Ganti dengan kredensial dari Blynk.Console
#define BLYNK_TEMPLATE_ID "TMPxxxxxxxxx"       // Template ID dari Blynk
#define BLYNK_TEMPLATE_NAME "ESP32 Ultrasonic" // Nama Template
#define BLYNK_AUTH_TOKEN "YourAuthTokenHere"    // Auth Token (32 karakter)

/* ========== INCLUDE LIBRARY ========== */
#include <WiFi.h>           // Library WiFi untuk ESP32
#include <BlynkSimpleEsp32.h> // Library Blynk untuk ESP32

/* ========== WIFI CREDENTIALS ========== */
// Ganti dengan SSID dan Password WiFi Anda
char ssid[] = "YourWiFiSSID";      // Nama WiFi
char pass[] = "YourWiFiPassword";  // Password WiFi

/* ========== PIN DEFINITIONS ========== */
#define TRIG_PIN 5      // GPIO5 - Trigger HC-SR04
#define ECHO_PIN 18     // GPIO18 - Echo HC-SR04
#define LED1_PIN 25     // GPIO25 - LED 1 (Kontrol Blynk V1)
#define LED2_PIN 26     // GPIO26 - LED 2 (Kontrol Blynk V2)
#define WIFI_LED 2      // GPIO2 - Built-in LED ESP32 (Status WiFi)

/* ========== KONSTANTA ========== */
#define SOUND_SPEED 0.034   // Kecepatan suara: 340 m/s = 0.034 cm/us
#define MAX_DISTANCE 400    // Jarak maksimum valid HC-SR04 (cm)
#define MIN_DISTANCE 2      // Jarak minimum valid HC-SR04 (cm)

/* ========== VARIABLES ========== */
long duration;                // Durasi sinyal echo (microseconds)
float distanceCm;           // Jarak terukur (cm)
unsigned long lastSensorRead = 0;   // Timestamp pembacaan terakhir
const unsigned long sensorInterval = 1000; // Interval baca sensor (ms)

/* ========== VIRTUAL PIN MAPPING ========== */
#define VPIN_DISTANCE V0   // V0 - Data jarak ke Blynk
#define VPIN_LED1 V1       // V1 - Kontrol LED 1 dari Blynk
#define VPIN_LED2 V2       // V2 - Kontrol LED 2 dari Blynk

/* ═══════════════════════════════════════════
   BLYNK CALLBACK FUNCTIONS
   ═══════════════════════════════════════════ */

/**
 * Callback: Saat widget LED 1 di Blynk berubah state
 * @param param nilai dari Blynk (0 = OFF, 1 = ON)
 */
BLYNK_WRITE(VPIN_LED1) {
  int pinValue = param.asInt();  // Ambil nilai integer dari Blynk
  digitalWrite(LED1_PIN, pinValue);

  // Log ke Serial Monitor
  Serial.print("[BLYNK] LED 1: ");
  Serial.println(pinValue ? "ON ✓" : "OFF ✗");

  // Kirim status balik ke Blynk (optional, untuk sync)
  Blynk.virtualWrite(VPIN_LED1, pinValue);
}

/**
 * Callback: Saat widget LED 2 di Blynk berubah state
 * @param param nilai dari Blynk (0 = OFF, 1 = ON)
 */
BLYNK_WRITE(VPIN_LED2) {
  int pinValue = param.asInt();
  digitalWrite(LED2_PIN, pinValue);

  Serial.print("[BLYNK] LED 2: ");
  Serial.println(pinValue ? "ON ✓" : "OFF ✗");

  Blynk.virtualWrite(VPIN_LED2, pinValue);
}

/**
 * Callback: Saat ESP32 terhubung ke Blynk Server
 */
BLYNK_CONNECTED() {
  Serial.println("[STATUS] ✅ Terhubung ke Blynk Server!");
  digitalWrite(WIFI_LED, HIGH);  // Nyalakan LED indikator

  // Sync state terakhir dari server (jika ada)
  Blynk.syncVirtual(VPIN_LED1, VPIN_LED2);
}

/**
 * Callback: Saat ESP32 disconnect dari Blynk Server
 */
BLYNK_DISCONNECTED() {
  Serial.println("[STATUS] ❌ Disconnect dari Blynk Server!");
  digitalWrite(WIFI_LED, LOW);   // Matikan LED indikator
}

/* ═══════════════════════════════════════════
   CUSTOM FUNCTIONS
   ═══════════════════════════════════════════ */

/**
 * Membaca jarak dari sensor ultrasonik HC-SR04
 * @return float jarak dalam centimeter (cm)
 */
float readDistance() {
  // 1. Pastikan trigger pin dalam keadaan LOW
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 2. Kirim sinyal trigger HIGH selama 10μs
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 3. Baca durasi pulse HIGH pada pin ECHO (timeout 30ms)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // 4. Hitung jarak: (durasi × kecepatan suara) / 2 (round-trip)
  float distance = duration * SOUND_SPEED / 2.0;

  return distance;
}

/**
 * Validasi hasil pembacaan sensor
 * @param distance nilai jarak yang akan divalidasi
 * @return true jika valid, false jika tidak
 */
bool isValidDistance(float distance) {
  return (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE);
}

/* ═══════════════════════════════════════════
   ARDUINO STANDARD FUNCTIONS
   ═══════════════════════════════════════════ */

void setup() {
  // ===== Inisialisasi Serial Monitor =====
  Serial.begin(115200);
  delay(1000);

  // Banner
  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║     ESP32 Blynk Ultrasonic Project       ║");
  Serial.println("║     IoT Distance Monitor & LED Control   ║");
  Serial.println("╚══════════════════════════════════════════╝");
  Serial.println();

  // ===== Konfigurasi Pin Mode =====
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  // ===== Inisialisasi Output =====
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(WIFI_LED, LOW);

  Serial.println("[SETUP] Pin konfigurasi selesai");

  // ===== Koneksi ke Blynk =====
  Serial.print("[WIFI] Menghubungkan ke: ");
  Serial.println(ssid);

  Blynk.config(BLYNK_AUTH_TOKEN);  // Set auth token
  Blynk.connectWiFi(ssid, pass);   // Connect WiFi + Blynk

  // Tunggu koneksi (timeout 30 detik)
  int timeout = 0;
  while (!Blynk.connected() && timeout < 30) {
    delay(1000);
    Serial.print(".");
    timeout++;
  }

  if (Blynk.connected()) {
    Serial.println("
[STATUS] ✅ Sistem siap!");
  } else {
    Serial.println("
[ERROR] ❌ Gagal terhubung ke Blynk!");
    Serial.println("        Periksa WiFi/Auth Token");
  }

  Serial.println("----------------------------------------");
}

void loop() {
  // ===== Jalankan Blynk (wajib dipanggil terus-menerus) =====
  Blynk.run();

  // ===== Baca Sensor dengan Interval =====
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;

    // Baca jarak dari sensor
    distanceCm = readDistance();

    // Validasi dan kirim ke Blynk
    if (isValidDistance(distanceCm)) {
      // Kirim data ke Blynk Virtual Pin V0
      Blynk.virtualWrite(VPIN_DISTANCE, distanceCm);

      // Tampilkan di Serial Monitor
      Serial.print("[SENSOR] Jarak: ");
      Serial.print(distanceCm, 1);  // 1 digit desimal
      Serial.println(" cm");

    } else {
      Serial.println("[ERROR] Pembacaan sensor tidak valid!");
    }
  }

  // Delay kecil untuk stabilitas (non-blocking)
  delay(10);
}
```

---

## 📖 Cara Penggunaan

### 1. Persiapan Kode

1. Buka file `.ino` di **Arduino IDE**
2. Ganti konfigurasi berikut:

```cpp
#define BLYNK_TEMPLATE_ID "TMPxxxxxxxxx"       // ← Template ID Anda
#define BLYNK_TEMPLATE_NAME "ESP32 Ultrasonic" // ← Nama Template
#define BLYNK_AUTH_TOKEN "YourAuthTokenHere"    // ← Auth Token Anda

char ssid[] = "YourWiFiSSID";      // ← Nama WiFi
char pass[] = "YourWiFiPassword";  // ← Password WiFi
```

### 2. Upload Kode

1. Pilih board: **Tools** → **Board** → **ESP32 Arduino** → **ESP32 Dev Module**
2. Pilih port: **Tools** → **Port** → *(pilih port ESP32)*
3. Klik **Upload** (tombol →)
4. Tunggu sampai "Done uploading"

### 3. Monitoring

1. Buka **Serial Monitor** (Ctrl+Shift+M)
2. Set baud rate ke **115200**
3. Amati output sistem

### 4. Kontrol via Blynk

1. Buka aplikasi **Blynk IoT** di smartphone
2. Pilih device yang telah dibuat
3. **Monitoring**: Lihat nilai jarak real-time di widget Gauge/Value
4. **Kontrol LED**: Tekan switch untuk menghidupkan/mematikan LED

---

## 🔍 Troubleshooting

### Masalah Umum & Solusi

| Masalah | Kemungkinan Penyebab | Solusi |
|---------|---------------------|--------|
| **ESP32 tidak ter-upload** | Driver CH340/CP210x belum install | Install driver USB-to-UART |
| **Tidak bisa connect WiFi** | SSID/Password salah | Periksa kredensial WiFi |
| **Tidak connect ke Blynk** | Auth Token salah | Copy ulang dari Blynk.Console |
| **Data jarak selalu 0** | Pin TRIG/ECHO salah | Periksa wiring HC-SR04 |
| **Data jarak tidak stabil** | Noise/Interferensi | Tambahkan kapasitor 100nF di VCC-GND sensor |
| **LED tidak menyala** | Polaritas terbalik | Periksa anoda/katoda LED |
| **Blynk timeout** | Koneksi internet lemah | Periksa sinyal WiFi |
| **Sensor tidak terbaca** | Supply voltage kurang | Pastikan HC-SR04 dapat 5V |

### Debug Serial Output

```
[STATUS] ✅ Terhubung ke Blynk Server!
[SENSOR] Jarak: 45.3 cm
[BLYNK] LED 1: ON ✓
[SENSOR] Jarak: 45.1 cm
[SENSOR] Jarak: 45.4 cm
[BLYNK] LED 2: ON ✓
```

---

## 📐 Spesifikasi Teknis

### Sensor HC-SR04
- **Voltage**: 5V DC
- **Current**: 15mA
- **Range**: 2cm - 400cm (±3mm akurasi)
- **Angle**: 15°
- **Frequency**: 40kHz

### ESP32 DevKit V1
- **Processor**: Tensilica Xtensa LX6 Dual-core
- **Clock**: 240MHz
- **WiFi**: 802.11 b/g/n
- **GPIO**: 34 pin programmable
- **ADC**: 12-bit SAR

### Blynk IoT
- **Protocol**: TLS/SSL encrypted
- **Update rate**: ~10 virtual writes/detik (limit free tier)
- **Platform**: iOS, Android, Web

---

## 🔄 Versi & Update

| Versi | Tanggal | Perubahan |
|-------|---------|-----------|
| v1.0.0 | 2026-05-12 | Initial release |

---

## 🤝 Kontribusi

Kontribusi sangat diterima! Silakan:
1. Fork repository ini
2. Buat branch fitur (`git checkout -b fitur-baru`)
3. Commit perubahan (`git commit -m 'Tambah fitur'`)
4. Push ke branch (`git push origin fitur-baru`)
5. Buat Pull Request

---

## 📄 Lisensi

Proyek ini dilisensikan di bawah **MIT License**.

```
MIT License

Copyright (c) 2026 [Nama Anda]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
```

---

## 📞 Kontak & Dukungan

- **Email**: [email@example.com]
- **GitHub Issues**: [Buat Issue Baru](../../issues)
- **Blynk Forum**: [community.blynk.cc](https://community.blynk.cc)

---

## 🙏 Acknowledgments

- [Blynk IoT Platform](https://blynk.io/) - Platform IoT yang powerful
- [Espressif Systems](https://www.espressif.com/) - ESP32 SoC
- [Arduino Community](https://forum.arduino.cc/) - Komunitas yang supportive

---

<div align="center">
  <p><strong>⭐ Star repo ini jika bermanfaat! ⭐</strong></p>
  <p>Made with ❤️ and ☕</p>
</div>
