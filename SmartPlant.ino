#define BLYNK_TEMPLATE_ID   "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "Akilli Tarim"
#define BLYNK_AUTH_TOKEN    "Senin_Blynk_Auth_Tokenin"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Wi-Fi Bilgileri
char ssid[] = "Wifi_Adin";
char pass[] = "Wifi_Sifren";

// Pin Tanımlamaları
#define DHTPIN 21
#define DHTTYPE DHT22
#define SOIL_PIN 2
#define BUZZER_PIN 4

#define RGB_RED 35
#define RGB_GREEN 36
#define RGB_BLUE 37

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Sensör Okuma ve Mantık Fonksiyonu
void checkSensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilValue = analogRead(SOIL_PIN);
  
  // Okuma hatası kontrolü
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT Sensöründen veri okunamadı!");
    return;
  }

  // Blynk Bulutuna Veri Gönderme
  Blynk.virtualWrite(V1, t);          // V1: Sıcaklık
  Blynk.virtualWrite(V2, h);          // V2: Hava Nemi
  Blynk.virtualWrite(V3, soilValue);   // V3: Toprak Nemi

  // Seri Port Çıktısı (Debugging)
  Serial.printf("Sıcaklık: %.1f°C | Nem: %.1f%% | Toprak Nemi: %d\n", t, h, soilValue);

  // --- ALARM VE LED MANTIĞI ---
  
  // 1. Durum: Toprak Kritik Seviyede Kuru (Örn: 3000 eşik değeri)
  if (soilValue > 3000) { 
    digitalWrite(RGB_RED, LOW);
    digitalWrite(RGB_GREEN, LOW);
    digitalWrite(RGB_BLUE, HIGH); // Mavi LED: Su lazım
    digitalWrite(BUZZER_PIN, HIGH); // Sesli Alarm Aktif
  } 
  // 2. Durum: Hava Çok Kuru / Sıcak (Örn: Nem %30 altı)
  else if (h < 30.0) {
    digitalWrite(RGB_RED, HIGH);  // Kırmızı LED: Kuru hava uyarısı
    digitalWrite(RGB_GREEN, LOW);
    digitalWrite(RGB_BLUE, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  // 3. Durum: Her Şey İdeal
  else {
    digitalWrite(RGB_RED, LOW);
    digitalWrite(RGB_GREEN, HIGH); // Yeşil LED: Sağlıklı durum
    digitalWrite(RGB_BLUE, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  
  // Pin Modları
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  // Başlangıçta tüm çıkışları kapat
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, LOW);

  dht.begin();
  
  // Blynk Bağlantısı
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Sensörleri her 2 saniyede bir oku (delay kullanmadan)
  timer.setInterval(2000L, checkSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}
