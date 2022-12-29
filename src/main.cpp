#include <Arduino.h>
#include "max6675.h"
#include <Wire.h>                                                   //zde naimportujeme potřebné knihovny
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define WIFI_SSID "WIFI SSID"                                                                                        //Network Name
#define WIFI_PASSWORD "WIFI PASSWORD"                                                                                //Network Password
#define INFLUXDB_URL "InfluxDB URL"                                                                                     //InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_TOKEN "InfluxDB Token"                                                                                 //InfluxDB v2 server or cloud API token (Use: InfluxDB UI -> Data -> API Tokens -> <select token>)
#define INFLUXDB_ORG "InfluxDB name"                                                                                     //InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_BUCKET "InfluxDB bucket"                                                                               //InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define TZ_INFO "AEDT+11"

int temp = 0; 
int thermoDO = 19;                                                 //deklarace proměnných
int thermoCS = 23;
int thermoCLK = 5;

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert); 

Point sensor("pokoj"); 

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() 
{
  Serial.begin(115200);                                             //Zahajíme serialní komunikaci                                                 //Connect to the DHT Sensor
 
  WiFi.mode(WIFI_STA);                                              //Nastavíme WIFI připojení
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");                               //Připojíme se k WIFI
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  sensor.addTag("device", DEVICE);                                   //Přidáme tagy
  sensor.addTag("SSID", WIFI_SSID);

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");                 //Zadefinujeme čas (musí být pro buckety v Influxu)

  if (client.validateConnection())                                   //Zkontrolujeme propojení se serverem
  {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } 
  else 
  {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  temp = thermocouple.readCelsius();                                  //Zaznamenáme teplotu

  sensor.clearFields();                                              //Smažeme pole (tagy zůstavají)

  sensor.addField("teplota", temp);                                  // Uložíme teplotu z čidlamdo proměnné
                              
  if (wifiMulti.run() != WL_CONNECTED)                               //Zkontrolujeme připojneí k WIFI a případně jej obnovíme
    Serial.println("Wifi connection lost");

  if (!client.writePoint(sensor))                                    //Zapíšeme datový bot
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  Serial.print("Teplota: ");                                            //Vypíšeme teplotu v konzoli
  Serial.println(temp);
 
  delay(10000); 
}