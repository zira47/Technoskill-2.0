// -- Blok Konfigurasi Awal --
// Baris ini digunakan untuk mengaktifkan pengaturan khusus jika diperlukan oleh library.
#define CUSTOM_SETTINGS
// Baris ini secara spesifik menyertakan modul Gamepad dari library Dabble.
#define INCLUDE_GAMEPAD_MODULE
// Menyertakan library utama Dabble untuk komunikasi dengan aplikasi di smartphone.
#include <DabbleESP32.h>
// Menyertakan library untuk mengontrol motor servo pada ESP32.
#include <ESP32Servo.h>


// -- Definisi Pin Hardware --
// Mendefinisikan pin 25 pada ESP32 untuk mengontrol motor agar bergerak maju.
#define IN1 25
// Mendefinisikan pin 26 pada ESP32 untuk mengontrol motor agar bergerak mundur.
// Catatan: Kode asli menggunakan IN4, saya sesuaikan definisinya agar konsisten menjadi IN4.
#define IN4 26
// Mendefinisikan pin 15 pada ESP32 sebagai pin sinyal untuk motor servo kemudi.
#define SERVO_PIN 15


// -- Deklarasi Objek Global --
// Membuat sebuah objek bernama 'steering' dari kelas Servo untuk mengontrol kemudi.
Servo steering;


// -- Fungsi Setup: Dijalankan sekali saat ESP32 pertama kali menyala --
void setup() {
  // Memulai komunikasi serial dengan kecepatan 115200 bps untuk debugging (melihat pesan di Serial Monitor).
  Serial.begin(115200);
  // Memulai library Dabble dan mengatur nama Bluetooth perangkat ESP32 ini menjadi "ESP32_RC_CAR".
  Dabble.begin("ROMY");

  // Menghubungkan objek 'steering' ke pin yang telah didefinisikan (SERVO_PIN).
  steering.attach(SERVO_PIN);
  // Mengatur posisi awal servo ke 90 derajat (posisi netral atau lurus).
  steering.write(90);

  // Mengatur pin IN1 sebagai pin OUTPUT untuk mengirim sinyal ke driver motor.
  pinMode(IN1, OUTPUT);
  // Mengatur pin IN4 sebagai pin OUTPUT untuk mengirim sinyal ke driver motor.
  pinMode(IN4, OUTPUT);

  // Memanggil fungsi berhenti() untuk memastikan motor tidak bergerak saat pertama kali dinyalakan.
  berhenti();
}


// -- Fungsi Loop: Dijalankan berulang kali setelah setup selesai --
void loop() {
  // Memproses input yang masuk dari aplikasi Dabble secara terus-menerus.
  Dabble.processInput();

  // === Blok Kontrol Gerak Maju/Mundur Motor ===
  // Cek apakah tombol 'Cross' (X) pada gamepad di aplikasi Dabble sedang ditekan.
  if (GamePad.isCrossPressed()) {
    // Jika ya, panggil fungsi maju() untuk menggerakkan mobil ke depan.
    maju();
  }
  // Jika tidak, cek apakah tombol 'Triangle' (Segitiga) sedang ditekan.
  else if (GamePad.isTrianglePressed()) {
    // Jika ya, panggil fungsi mundur() untuk menggerakkan mobil ke belakang.
    mundur();
  }
  // Jika tidak ada tombol arah maju atau mundur yang ditekan.
  else {
    // Panggil fungsi berhenti() untuk menghentikan mobil.
    berhenti();
  }

  // === Blok Kontrol Kemudi (Servo) ===
  // Membuat variabel 'steerAngle' dan mengaturnya ke 90 (posisi netral/lurus) sebagai default.
  int steerAngle = 90;

  // Cek apakah tombol 'Kiri' pada gamepad sedang ditekan.
  if (GamePad.isLeftPressed()) {
    // Jika ya, ubah nilai 'steerAngle' menjadi 60 untuk belok ke kiri.
    steerAngle = 60;

  }
  // Jika tidak, cek apakah tombol 'Kanan' pada gamepad sedang ditekan.
  else if (GamePad.isRightPressed()) {
    // Jika ya, ubah nilai 'steerAngle' menjadi 120 untuk belok ke kanan.
    steerAngle = 115;

  }

  // Mengirimkan nilai 'steerAngle' terakhir ke motor servo untuk mengatur arah kemudi.
  steering.write(steerAngle);
}


// -- Kumpulan Fungsi untuk Kontrol Motor --

// Fungsi untuk menggerakkan mobil maju.
void maju() {
  // Memberi sinyal HIGH ke pin IN1 dan LOW ke pin IN4.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN4, LOW);
}

// Fungsi untuk menggerakkan mobil mundur.
void mundur() {
  // Memberi sinyal LOW ke pin IN1 dan HIGH ke pin IN4.
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, HIGH);
}

// Fungsi untuk menghentikan mobil.
void berhenti() {
  // Memberi sinyal LOW ke kedua pin (IN1 dan IN4) untuk menghentikan motor.
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, LOW);
}