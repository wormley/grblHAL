/*
  sdcard.h - An embedded CNC Controller with rs274/ngc (g-code) support

  Driver code for Texas Instruments Tiva C (TM4C123GH6PM) ARM processor

  Part of Grbl

  Copyright (c) 2018-2020 Terje Io

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

#ifndef _SDCARD_H_
#define _SDCARD_H_

#ifdef ARDUINO
#include "../../driver.h"
#include "../grbl/grbl.h"
#else
#include "driver.h"
#include "grbl/grbl.h"
#endif

#if SDCARD_ENABLE

#ifdef __MSP432E401Y__
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include <ti/drivers/SDFatFS.h>
#include <ti/boards/MSP_EXP432E401Y/Board.h>
#elif defined(ESP_PLATFORM)
#include "esp_vfs_fat.h"
#elif defined(__LPC176x__)
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#elif defined(ARDUINO_SAMD_MKRZERO) || defined(STM32F103xB) || defined(__LPC17XX__)
#include "ff.h"
#include "diskio.h"
#else
#include "fatfs/src/ff.h"
#include "fatfs/src/diskio.h"
#endif

void sdcard_init (void);
FATFS *sdcard_getfs(void);

#endif // SDCARD_ENABLE

#endif
