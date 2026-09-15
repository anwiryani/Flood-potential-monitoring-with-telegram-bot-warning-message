#include <Arduino.h>
#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#define soundSPEED 0.034

const int trigPIN = 18;
const int echoPIN = 17;
const int ledPIN = 5;

String ssid = "Wokwi-GUEST";
String pass = "";
String token = " ";
String chatID = "  ";

const int window = 5;
float sensorVALUE[window];
int readINDEX = 0;
float average;
float total = 0;

float duration;
float distance = 1;

WiFiClientSecure client;
UniversalTelegramBot bot(token, client);

void sensing(void *parameters)
{
  while (1)
  {
    digitalWrite(trigPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPIN, LOW);

    duration = pulseIn(echoPIN, HIGH);
    distance = duration * soundSPEED / 2;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void movingAVERAGE(void *parameters)
{
  while (1)
  {
    total = total - sensorVALUE[readINDEX];
    sensorVALUE[readINDEX] = distance;
    total = total + sensorVALUE[readINDEX];

    readINDEX++;

    if (readINDEX >= window)
    {
      readINDEX = 0;
    }
    average = total / window;

    Serial.print("Rerata data sensor: ");
    Serial.println(average);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void stateALERT(void *parameters)
{
  while (1)
  {
    if (average >= 70)
    {
      Serial.println("Kondisi: AMAN");
      digitalWrite(ledPIN, LOW);
    }
    else if (average < 70 && average >= 40)
    {
      Serial.println("kondisi: SIAGA");
      digitalWrite(ledPIN, LOW);
    }
    else
    {
      Serial.println("Kondisi: AWAS");
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void sendMESSAGE(void *parameters)
{
  while (1)
  {
    if (average < 40)
    {
      Serial.print("mau kirimmm");
      bot.sendMessage(chatID, "Kondisi AWAS terdeteksi", "");
      Serial.print("yeayyy kirimmm");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.print("Menghubungkan ke WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi terhubung");

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  bot.sendMessage(chatID, "Ini adalah BOT peringatan dini potensi benacana banjir. Pesan peringatan akan dikirimkan apabila situasi AWAS terdeteksi", "");

  pinMode(trigPIN, OUTPUT);
  pinMode(echoPIN, INPUT);
  pinMode(ledPIN, OUTPUT);

  xTaskCreatePinnedToCore(
      stateALERT,
      "state alert",
      4096,
      NULL,
      1,
      NULL,
      0);

  xTaskCreatePinnedToCore(
      movingAVERAGE,
      "moving average",
      2048,
      NULL,
      1,
      NULL,
      0);

  xTaskCreatePinnedToCore(
      sensing,
      "sensing",
      2048,
      NULL,
      1,
      NULL,
      0);

  xTaskCreatePinnedToCore(
      sendMESSAGE,
      "send message",
      8192,
      NULL,
      2,
      NULL,
      0);
}

void loop()
{
}
