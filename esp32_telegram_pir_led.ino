#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wifi Name";
const char* password = "Wifi Passwd";

// Bot 1
String botToken1 = "Bot2_Token";
String chatID1   = "Cheat_Id";

// Bot 2
String botToken2 = "Bot1_Token";
String chatID2   = "Cheat_Id";

// Gönderim kontrolü
volatile bool sendEnabled = true;

// Son okunan update ID’leri
long lastUpdateId1 = 0;
long lastUpdateId2 = 0;

// Pinler
const int pirPin = 34; // PIR sensör
const int ledPin = 12; // LED

// PIR durumu
bool lastPirState = LOW;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.print("WiFi bağlanıyor...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi bağlı!");

  //sendToBot(botToken1, chatID1, "ESP32 bağlandı! (Bot 1)");
  sendToBot(botToken2, chatID2, "ESP32 bağlandı! (Bot 2)");
}

void loop() {
  // Telegram komutlarını sürekli kontrol et
  checkCommands(botToken1, lastUpdateId1);
  checkCommands(botToken2, lastUpdateId2);

  // PIR sensör okuması
  bool pirState = digitalRead(pirPin);
  
  if (pirState != lastPirState && pirState == HIGH) {
    // Hareket algılandı
    digitalWrite(ledPin, HIGH); // LED yak
    if (sendEnabled) {
      //sendToBot(botToken1, chatID1, "Hareket algılandı! (Bot 1)");
      sendToBot(botToken2, chatID2, "Hareket algılandı! (Bot 2)");
    }
  } else if (pirState == LOW) {
    digitalWrite(ledPin, LOW); // LED söndür
  }

  lastPirState = pirState;

  delay(100); // Döngüyü biraz yavaşlat
}

// --- Komut kontrol fonksiyonu ---
void checkCommands(String token, long &lastUpdateId) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + token + "/getUpdates?offset=" + String(lastUpdateId + 1);
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();

    int index = 0;
    while ((index = payload.indexOf("\"update_id\":", index)) != -1) {
      int idStart = index + 12;
      int idEnd = payload.indexOf(",", idStart);
      long updateId = payload.substring(idStart, idEnd).toInt();

      int textIndex = payload.indexOf("\"text\":\"", index);
      if (textIndex != -1) {
        int textStart = textIndex + 8;
        int textEnd = payload.indexOf("\"", textStart);
        String text = payload.substring(textStart, textEnd);
        text.trim();
        text.toLowerCase();

        if (text == "stop") {
          sendEnabled = false;
          //sendToBot(botToken1, chatID1, "Mesaj gönderimi durduruldu! (Bot 1)");
          sendToBot(botToken2, chatID2, "Mesaj gönderimi durduruldu! (Bot 2)");
        }
        if (text == "start") {
          sendEnabled = true;
          //sendToBot(botToken1, chatID1, "Mesaj gönderimi başlatıldı! (Bot 1)");
          sendToBot(botToken2, chatID2, "Mesaj gönderimi başlatıldı! (Bot 2)");
        }
      }

      lastUpdateId = updateId;
      index = idEnd;
    }
  }

  http.end();
}

// --- Mesaj gönderme fonksiyonu ---
void sendToBot(String token, String chatID, String message) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + token + "/sendMessage";
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String data = "chat_id=" + chatID + "&text=" + message;

  int httpCode = http.POST(data);

  if (httpCode > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println(http.errorToString(httpCode));
  }

  http.end();
}
