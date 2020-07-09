#include "CTBot.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
CTBot myBot;
WiFiClient client;
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
String ssid = "Menut31";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "77778888"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "1052230276:AAHLUkTe3cbO1l63ozKgpPsyympSQ0NdJw8";
String textkirim;
int idkirim = 991424988;
int pompa = D5;
int sentanah = A0;
float t, h;
int val,nilaiadc;
unsigned long intervalkirim = 3600000;
unsigned long milissekarang, milisdulu,milisserial;
void setup() {
  // initialize the Serial
  Serial.begin(115200);
  dht.begin();
  pinMode(pompa, OUTPUT);
  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);
  // set the telegram bot token
  myBot.setTelegramToken(token);
  digitalWrite(pompa, LOW);
}

void loop() {
  milissekarang = millis();
  nilaiadc = analogRead(sentanah);
  val = map(nilaiadc, 650, 500, 0, 100);
  if(val>=100)
  {
    val=100;
  }
  if(val<=0)
  {
    val=0;
  }
  if(val<=50)
  {
    digitalWrite(pompa,HIGH);
  }
  if(val>=80)
  {
    digitalWrite(pompa,LOW);
  }
  bacadht();
  TBMessage msg;
  if (myBot.getNewMessage(msg))
  {
    if (msg.text.equalsIgnoreCase("sensor"))
    {
      textkirim = "Data sensor:\nSoil moisture: ";
      textkirim += val;
      textkirim += "%.\nSuhu ";
      textkirim += t;
      textkirim += "C.\nKelembaban ";
      textkirim += h;
      textkirim += "%.\nPompa dalam kondisi ";
      boolean a = digitalRead(pompa);
      if (a)textkirim += "menyala.";
      else textkirim += "mati.";
      myBot.sendMessage(idkirim, textkirim);
      Serial.println("Kirim Status Sensor!");
    }
    if (msg.text.equalsIgnoreCase("siram"))
    {
      myBot.sendMessage(idkirim, "Menyiram Tanaman");
      digitalWrite(pompa, HIGH);
      delay(8000);
      digitalWrite(pompa, LOW);
      Serial.println("Kirim Status Sensor!");
    }
//    if (msg.text.equalsIgnoreCase("pompa off"))
//    {
//      digitalWrite(pompa, LOW);
//      myBot.sendMessage(msg.sender.id, "Pompa Mati");
//      Serial.println("Kirim Status Sensor!");
//    }
    delay(100);
  }
  if (milissekarang - milisdulu >= intervalkirim)
  {
    textkirim = "Data sensor:(otomatis)\nSoil moisture: ";
    textkirim += val;
    textkirim += "%.\nSuhu ";
    textkirim += t;
    textkirim += "C.\nKelembaban ";
    textkirim += h;
    textkirim += "%.\nRelay dalam kondisi ";
    boolean a = digitalRead(pompa);
    if (a)textkirim += "menyala.";
    else textkirim += "mati.";
    myBot.sendMessage(idkirim, textkirim);
    Serial.println("Kirim Status Sensor!");
    milisdulu = milissekarang;
    delay(100);
  }
  if (milissekarang - milisserial >= 1000)
  {
    Serial.print("tanah: ");
    Serial.println(val);
    Serial.print("suhu: ");
    Serial.println(t);
    Serial.print("Kel:");
    Serial.println(h);
    Serial.print("ADC Sensor:");
    Serial.println(nilaiadc);
    Serial.print("milisdulu:");
    Serial.println(milisdulu);
    Serial.print("milissekarng:");
    Serial.println(milissekarang);
    milisserial=milissekarang;
  }
}
void bacadht()
{
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(10);
  }
}
