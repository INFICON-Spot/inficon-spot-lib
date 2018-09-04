/*
 * inficon_spot.cpp
 *
 * Arduino library for INFICON Spot sensors
 *
 * Copyright (c) 2018 INFICON Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Arduino.h"
#include "inficon_spot.h"

InficonSpot::InficonSpot()
{
  _ss_pin = 10;
  _rdy_pin = 7;
  _spi_freq = 4000000UL;
  _fullscale = 0;
}

InficonSpot::InficonSpot(int ss_pin, int rdy_pin, unsigned long spi_freq)
{
  _ss_pin = ss_pin;
  _rdy_pin = rdy_pin;
  _spi_freq = spi_freq;
  _fullscale = 0;
}

void InficonSpot::begin()
{
  SPI.begin();

  // initialize the data ready and slave select pins:
  digitalWrite(_ss_pin, HIGH);
  pinMode(_ss_pin, OUTPUT);
  pinMode(_rdy_pin, INPUT_PULLUP);
}

/*
 * send the reset command (0x88) to the sensor
 */
void InficonSpot::resetSensor()
{
  SPI.beginTransaction(SPISettings(_spi_freq, MSBFIRST, SPI_MODE1));
  digitalWrite(_ss_pin, LOW);
  SPI.transfer(0x88);
  digitalWrite(_ss_pin, HIGH);
  SPI.endTransaction();
}

bool InficonSpot::isDataAvailable()
{
  return (digitalRead(_rdy_pin) == LOW);
}

/*
 * read a result register from the sensor
 *
 * The input parameter reg specifies which result
 * shall be read. The return value is the content
 * of the 24 bit result register.
 */
uint32_t InficonSpot::readRegister(InficonSpot::SpotRegister reg)
{
  uint32_t result;
  byte buf[4] = {0, 0, 0, 0}; // buffer for the 4 byte SPI transfer

  buf[0] = reg; // copy opcode to the first byte of the buffer

  SPI.beginTransaction(SPISettings(_spi_freq, MSBFIRST, SPI_MODE1));
  // take the chip select low to select the device:
  digitalWrite(_ss_pin, LOW);
  // send the opcode and three zero bytes. Four received bytes will be placed in buf
  SPI.transfer(buf, 4);
  // take the chip select high to de-select:
  digitalWrite(_ss_pin, HIGH);
  SPI.endTransaction();

  // copy the 24 bit result from the buffer
  result = ((uint32_t) buf[1] << 16) | ((uint32_t) buf[2] << 8) | (uint32_t) buf[3];
  return result;
}

void InficonSpot::setFullscale(float fullscale)
{
  _fullscale = fullscale;
}

/*
 * Convert a the pressure result registers content to pressure.
 * requires the fullscale parameter to be set to the full scale pressure
 * in the desired pressure unit.
 */
float InficonSpot::convertPressure(uint32_t result)
{
  int32_t rawPressure = *((int32_t *) &result); // convert to signed integer
  return  rawPressure / 2097152.0f * _fullscale;
}

/*
 * convert the temperature result register to degree Celsius
 */
float InficonSpot::convertTemperature(uint32_t result)
{
  int32_t rawTemperature = *((int32_t *) &result); // convert to signed integer
  return rawTemperature / 2097152.0f * 25;
}
