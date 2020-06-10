/*
  i2c.h - I2C interface

  Driver code for IMXRT1062 processor (on Teensy 4.0 board)

  Part of GrblHAL

  Copyright (c) 2019 Terje Io

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include "driver.h"
#include "src/grbl/plugins.h"

#if TRINAMIC_ENABLE && TRINAMIC_I2C

#include "src/trinamic/trinamic2130.h"
#include "src/trinamic/TMC2130_I2C_map.h"

#define I2C_ADR_I2CBRIDGE 0x47

void I2C_DriverInit (TMC_io_driver_t *drv);

#endif

#if KEYPAD_ENABLE

#include "src/keypad/keypad.h"

void I2C_GetKeycode (uint32_t i2cAddr, keycode_callback_ptr callback);

#endif

#endif

uint8_t *I2C_Receive (uint32_t i2cAddr, uint8_t *buf, uint8_t bytes, bool block);
void I2C_Send (uint32_t i2cAddr, uint8_t *buf, uint8_t bytes, bool block);
uint8_t *I2C_ReadRegister (uint32_t i2cAddr, uint8_t *buf, uint8_t abytes, uint8_t bytes, bool block);