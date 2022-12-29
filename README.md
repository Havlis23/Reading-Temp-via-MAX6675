# Reading-Temp-via-MAX6675
School project about reading temp from MAX6675 thermocedule. Then data will write into Influx database


Arduino Temperature Measurement using MAX6675 Thermocouple
This Arduino project uses a MAX6675 thermocouple amplifier to measure temperature. The MAX6675 is a low-cost, high-precision thermocouple-to-digital converter that can accurately measure temperatures from 0°C to 1024°C with a 0.25°C resolution.

Hardware
Arduino Uno or similar
MAX6675 thermocouple amplifier
K-type thermocouple
Circuit Diagram
Circuit Diagram

Code
Copy code
#include <MAX6675.h>

MAX6675 thermocouple(9, 8, 7);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float celsius = thermocouple.readCelsius();
  Serial.println(celsius);
  delay(1000);
}
Usage
Connect the hardware as shown in the circuit diagram above.
Upload the code to the Arduino.
Open the serial monitor to see the temperature readings in degrees Celsius.
Notes
Make sure the thermocouple is properly connected to the MAX6675 amplifier.
The thermocouple can measure temperatures from -200°C to +1350°C, but the MAX6675 has a range of 0°C to 1024°C.
The MAX6675 can also measure temperatures in Fahrenheit and Kelvin, but the code above only displays the temperature in Celsius. To display the temperature in Fahrenheit or Kelvin, use the readFahrenheit() or readKelvin() functions instead of readCelsius().
