#include <Adafruit_DPS310.h>
#include <math.h>

Adafruit_DPS310 dps;

float seaLevelPressure = 1013.25; // standard sea level pressure in hPa
float initialHeight = 0;
const int delayTime = 100;

float calculateAltitude(float pressure) {
  return 44330 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
}

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  //Just set the EEPROM to blank initially
  EEPROM.put(0,0.00f);

  Serial.println("DPS310 Pressure Sensor Test");

  if (!dps.begin_I2C()) {
    Serial.println("Failed to find DPS310 chip");
    while (1) delay(10);
  }
  Serial.println("DPS310 Found!");

  dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
  dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);

  // Take initial readings
  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);
  initialHeight = calculateAltitude(pressure_event.pressure);
  Serial.println("Initial height set. Ready to measure relative height.");
}

void loop() {
  sensors_event_t temp_event, pressure_event;

  if (dps.temperatureAvailable()) {
    dps.getEvents(&temp_event, &pressure_event);
    
    float currentAltitude = calculateAltitude(pressure_event.pressure);
    float relativeHeight = currentAltitude - initialHeight;
    float max = 0.00f;
    EEPROM.get(0, max);

    // Serial.print("Temperature = ");
    // Serial.print(temp_event.temperature);
    // Serial.println(" Â°C");

    // Serial.print("Pressure = ");
    // Serial.print(pressure_event.pressure);
    // Serial.println(" hPa");

    // Serial.print("Relative Height = ");
    // Serial.print(relativeHeight);
    // Serial.println(" meters");

    Serial.print("Max Height: ");
    Serial.print(max);
    Serial.println(" meters");

    Serial.println();
    if (currentAltitude > max) {
      EEPROM.put(0,currentAltitude)
    }
  }

  delay(delayTime);  // Wait for .1 seconds before next reading

}