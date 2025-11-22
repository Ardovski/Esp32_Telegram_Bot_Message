ESP32 Telegram Bot Kontrol Projesi

Bu proje, ESP32 kullanarak iki ayrı Telegram botu üzerinden mesaj gönderip almayı sağlar. ESP32, rastgele sayı üretip “Tek/Çift” mesajları gönderebilir ve Telegram üzerinden gelen start ve stop komutlarıyla mesaj gönderimini kontrol edebilir.

Özellikler

ESP32 üzerinden WiFi ile Telegram’a bağlanma.

İki ayrı bot ile mesaj gönderimi.

Rastgele sayı üreterek “Tek/Çift” mesajları gönderme.

Telegram üzerinden mesaj gönderimini başlatma/durdurma (start/stop komutları).

Basit, bloklamayan ve hızlı çalışan loop yapısı.

Seri monitör üzerinden loglama.

Gereksinimler

ESP32 geliştirme kartı

Arduino IDE veya PlatformIO

İnternet bağlantısı (WiFi)

Telegram hesabı ve bot token’ları

Kurulum

Arduino IDE’ye ESP32 desteğini ekleyin

Arduino IDE’yi açın.

Araçlar > Kart > Kart Yöneticisi’nden ESP32 paketlerini yükleyin.

Telegram Bot Token ve Chat ID alın

BotFather
 ile bot oluşturun.

Bot token’ınızı kopyalayın.

Telegram’da kendinize mesaj gönderip Chat ID’nizi öğrenin. (Örn: @userinfobot
 kullanabilirsiniz.)

Kodda gerekli bilgileri değiştirin

const char* ssid = "Wifi Name";        // WiFi adı
const char* password = "Wifi Passwd";  // WiFi şifre

String botToken1 = "Bot1_token";       // Bot 1 token
String chatID1   = "Your_Chat_Id";    // Bot 1 chat ID

String botToken2 = "Bot2_token";       // Bot 2 token
String chatID2   = "Your_Chat_Id";    // Bot 2 chat ID


ESP32’ye yükleyin

Araçlar > Kart > ESP32 Dev Module seçin.

COM portu doğru seçin.

Kodu yükleyin ve Seri Monitör’ü 115200 baud ile açın.

ESP32 WiFi ağına bağlandığında “ESP32 bağlandı!” mesajını göreceksiniz.

Kullanım

ESP32 açıldığında iki bot üzerinden mesaj göndermeye başlar.

Telegram üzerinden:

stop yazarsanız mesaj gönderimi durur.

start yazarsanız mesaj gönderimi tekrar başlar.

Rastgele sayı üretimi ile Tek/Çift mesajları belirli aralıklarla gönderilir.

Kod Yapısı

setup(): WiFi bağlanır ve botlara başlangıç mesajı gönderir.

loop(): Komutları kontrol eder ve rastgele sayı mesajlarını gönderir.

checkCommands(): Telegram’dan start/stop komutlarını okur ve sendEnabled durumunu değiştirir.

sendToBot(): Telegram botuna mesaj gönderir.

luck(): Rastgele sayı üretir ve “Tek/Çift” mesajlarını botlara gönderir.

Örnek Seri Monitör Çıktısı
WiFi bağlanıyor...
WiFi bağlı!
ESP32 bağlandı! (Bot 1)
ESP32 bağlandı! (Bot 2)
Çift Geldi ✓ (Bot 1)
Çift Geldi ✓ (Bot 2)