ESP32 Temperature Monitoring with MAX6675 Thermocouple and InfluxDB

This Arduino sketch runs on an ESP32 development board and uses a MAX6675 thermocouple amplifier to measure temperature. The temperature data is sent to an InfluxDB database via WiFi and can be visualized on a Grafana dashboard. The sketch also synchronizes time with a time server.
Hardware

* ESP32 development board
* MAX6675 thermocouple amplifier
* K-type thermocouple
* WiFi module
Circuit Diagram
￼
Dependencies
* InfluxDB Arduino Client
* WiFiMulti

```c++
#include <Arduino.h>
#include "max6675.h"
#include <Wire.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "WIFI PASSWORD"
#define INFLUXDB_URL "InfluxDB URL"
#define INFLUXDB_TOKEN "InfluxDB Token"
#define INFLUXDB_ORG "InfluxDB name"
#define INFLUXDB_BUCKET "InfluxDB bucket"
#define TZ_INFO "AEDT+11"

int temp = 0; 
int thermoDO = 19;
int thermoCS = 23;
int thermoCLK = 5;

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert); 

Point sensor("pokoj"); 

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WIFI_SSID);

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  if (client.validateConnection())
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
```

Usage
1. Connect the hardware as shown in the circuit diagram above.
2. Install the InfluxDB Arduino Client library.
3. Update the ssid, password, influxDbUrl, and influxDbDatabase variables with your WiFi and InfluxDB connection details.
4. Upload the code to the ESP32.
5. The ESP32 should connect to the WiFi network and start sending temperature data to the InfluxDB database.
6. Use Grafana to create a dashboard and visualize the temperature data from the InfluxDB database.


Notes
* Make sure the thermocouple is properly connected to the MAX6675 amplifier.
* The thermocouple can measure temperatures from -200°C to +1350°C, but the MAX6675 has a range of 0°C to 1024°C.
* The MAX6675 can also measure temperatures in Fahrenheit and Kelvin, but the code above only sends the temperature in Celsius. To send the temperature in Fahrenheit or Kelvin, use the readFahrenheit() or readKelvin() functions instead of readCelsius().
