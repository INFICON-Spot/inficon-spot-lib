/*
 * inficon_spot.h
 *
 * Arduino library for INFICON Spot sensors
 *
 * Copyright (c) 2018-2021 INFICON Ltd.
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

#ifndef H_INFICON_SPOT_H
#define H_INFICON_SPOT_H

#include <SPI.h>
#include "Arduino.h"

class InficonSpot
{
  public:
    enum SpotRegister {
      REG_PRESSURE = 0x01,
      REG_PRESSURE1 = 0x06,
      REG_PRESSURE2 = 0x07,
      REG_TEMPERATURE = 0x0D,
      REG_STATUS = 0x08,
    };

    enum SpotStatus {
      STATUS_SPI_INTERFERENCE = 1UL << 23,
      STATUS_PRESSURE_ERROR = 1UL << 13,
      STATUS_PORT3_ERROR = 1UL << 8,
      STATUS_PORT2_ERROR = 1UL << 7,
      STATUS_PORT1_ERROR = 1UL << 6,
      STATUS_PORT0_ERROR = 1UL << 5,
      STATUS_TEMPERATURE_ERROR = 1UL << 3,
    };

    typedef String(InficonSpot::*ReadLabelFunction) (void);

    InficonSpot();
    InficonSpot(int ss_pin, int rdy_pin, unsigned long freq);
    void begin();
    void setFullscale(float fullscale);
    bool isDataAvailable();
    void sendCommand(uint8_t cmd);
    void readOTP(uint16_t address, uint8_t *data, int length);
    void readMemory(uint16_t address, uint8_t *data, int length);
    void writeMemory(uint16_t address, const uint8_t *data, int length);
    void resetSensor();
    uint32_t readRegister(byte reg);
    void writeRegister(byte reg, uint32_t data);
    float convertPressure(uint32_t reg);
    float convertPressure(uint32_t result, float fullscale);
    float convertTemperature(uint32_t reg);

    String readProductNo();
    String readSerialNo();
    String readFullscale1();
    String readFullscale2();
    String readType();
    String readSpeed();

    uint32_t readSramCrc();
    uint32_t readOtpCrc();

  protected:
    int _ss_pin;
    int _rdy_pin;
    unsigned long _spi_freq;
    float _fullscale;

    // label data addresses
    const uint16_t ADDR_PRODUCT_NO = 0x0ef0;
    const uint16_t ADDR_SERIAL_NO = 0x0f10;
    const uint16_t ADDR_FULLSCALE1 = 0x0f30;
    const uint16_t ADDR_FULLSCALE2 = 0x0f40;
    const uint16_t ADDR_TYPE = 0x0f50;
    const uint16_t ADDR_SPEED = 0x0f60;
    const uint16_t ADDR_SRAMCRC = 0x0ee8;
    const uint16_t ADDR_OTPCRC = 0x0eec;

    String readLabel(uint16_t address, byte length);
};

#endif
