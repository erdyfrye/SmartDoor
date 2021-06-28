#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#define DHTTYPE DHT11
#define DHTPIN 16
#define BOTtoken "863121937:AAH1GHt1vI5wpqvWAhKqNTTxMu2FAE-wVuE"

char ssid[] = "AndroidAPC36B";
char password[] = "erdyfrye";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(13, 12);
const int pintu = 4; 
const int PIR = 5;
float t = dht.readTemperature();
float l = dht.readHumidity();
void ICACHE_RAM_ATTR sensorpintu();
void ICACHE_RAM_ATTR sensorpir();
int Bot_mtbs = 1000;
long Bot_lasttime;
bool Start = false;

void setup()
{
  bot._debug=true;
  Serial.begin(115200);
  pinMode(PIR, INPUT);
  attachInterrupt(digitalPinToInterrupt(pintu), sensorpintu, RISING);
  
  mySerial.begin (9600);
  mp3_set_serial (mySerial);
  delay(5);
  mp3_reset();
  delay(5);
  mp3_set_volume (15);
}


void sensorpintu()
{
  Serial.println("Selamat Datang");

  mp3_play (1);
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/status") {
      bot.sendMessage(chat_id, SState(), "");
    }
    
    if(deteksi())
    {
      bot.sendMessage(chat_id, "Gerakan Terdeteksi");   
    }

    if (text == "Halo") {
      String welcome = "Halo saya adalah kocheng bot, " + from_name + ".\n";
      welcome += "Saya adalah tugas kerja praktik Erdy Yudhistira dan Fauzan Al-Maghribi.\n\n";
      welcome += "Untuk saat ini saya hanya bisa mengecek suhu dengan mengetik: /status \n";
      bot.sendMessage(chat_id, welcome);
    }
  }

}
String SState() {
  Serial.print(t);
    String(t);
  Serial.print(suhu);
  String msg = "Status.\n";
  msg += "Suhu: " + String(t) + " °C\n";
  msg += "Kelembaban: " + String(l) + " %\n";
  return msg;
}
boolean deteksi()
{
  int pirr=digitalRead(PIR);
  if(pirr==HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void  loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
    delay(300);
  }
}
