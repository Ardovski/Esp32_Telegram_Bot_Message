#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wifi Name";
const char* password = "Wifi Passwd";

// Bot 1
String botToken1 = "Bot1_token";
String chatID1   = "Your_Cheat_Id";

// Bot 2
String botToken2 = "Bot2_token";
String chatID2   = "Your_Cheat_Id";

// Gönderim kontrolü
volatile bool sendEnabled = true;

// Son okunan update ID’leri
long lastUpdateId1 = 0;
long lastUpdateId2 = 0;

unsigned long counter = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("WiFi bağlanıyor...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi bağlı!");

  sendToBot(botToken1, chatID1, "ESP32 bağlandı! (Bot 1)");
  sendToBot(botToken2, chatID2, "ESP32 bağlandı! (Bot 2)");
}

// --- Luck fonksiyonu ---
void luck() {
  counter++;
  if (counter < 5) return; // eşiği düşürdük, daha sık mesaj

  int number = random(0, 1000); // 0-999 arası rastgele sayı
  String message;

  if (number % 2 == 0) message = "Çift Geldi ✓";
  else message = "Tek Geldi ✓";

  sendToBot(botToken1, chatID1, message + " (Bot 1)");
  sendToBot(botToken2, chatID2, message + " (Bot 2)");

  counter = 0; // sayacı sıfırla
}


void loop() {
  // Telegram komutlarını sürekli kontrol et
  checkCommands(botToken1, lastUpdateId1);
  checkCommands(botToken2, lastUpdateId2);

  // Mesaj göndermeye izin varsa
  if (sendEnabled) {
    luck();
  }

  delay(100); // Çok kısa, ESP32’yi bloklamadan çalışır
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
          sendToBot(botToken1, chatID1, "Mesaj gönderimi durduruldu! (Bot 1)");
          sendToBot(botToken2, chatID2, "Mesaj gönderimi durduruldu! (Bot 2)");
        }
        if (text == "start") {
          sendEnabled = true;
          sendToBot(botToken1, chatID1, "Mesaj gönderimi başlatıldı! (Bot 1)");
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


