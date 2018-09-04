#include <SPI.h>

// INFICON Spot sensor
#include <inficon_spot.h>

InficonSpot spot = InficonSpot();

const float fullscale = 500.0f; // 500 Torr fullscale range

void setup() {
  // setup serial port
  Serial.begin(2000000); // 2 MBaud

  spot.begin();
  spot.setFullscale(fullscale);
  Serial.println ("working");

  // reset the Spot sensor
  spot.resetSensor();
}

unsigned long lastresult;

void loop() {
  if (millis() - lastresult > 1000) {
    // no measurement received, in the last 1000 ms, try to reset Spot
    Serial.println("Timeout: Resetting Spot");
    spot.resetSensor();
    lastresult = millis(); // store the time of the last valid measurement
  }
  // don't do anything until the data ready pin is low
  if (spot.isDataAvailable()) {
    // Read the pressure data, temperature and status
    uint32_t pressure = spot.readRegister(InficonSpot::REG_PRESSURE);
    uint32_t temperature = spot.readRegister(InficonSpot::REG_TEMPERATURE);
    uint32_t status = spot.readRegister(InficonSpot::REG_STATUS);

    Serial.print(String(spot.convertPressure(pressure)));
    Serial.print(" " + String(spot.convertTemperature(temperature)));

    if (status & (InficonSpot::STATUS_SPI_INTERFERENCE | InficonSpot::STATUS_PRESSURE_ERROR |
        InficonSpot::STATUS_PORT3_ERROR | InficonSpot::STATUS_PORT2_ERROR | InficonSpot::STATUS_PORT1_ERROR |
        InficonSpot::STATUS_PORT0_ERROR | InficonSpot::STATUS_TEMPERATURE_ERROR)) {
      Serial.print(" " + String(status));
    }
    Serial.println("");
    digitalWrite(6, LOW);
    lastresult = millis(); // store the time of the last valid measurement
  }
}
