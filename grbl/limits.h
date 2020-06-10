/*
  limits.h - code pertaining to limit-switches and performing the homing cycle
  Part of Grbl

  Copyright (c) 2017-2018 Terje Io
  Copyright (c) 2012-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#ifndef limits_h
#define limits_h

#include "nuts_bolts.h"

// Perform one portion of the homing cycle based on the input settings.
bool limits_go_home(axes_signals_t cycle);

// Check for soft limit violations
void limits_soft_check(float *target);

// Set axes to be homed from settings.
void limits_set_homing_axes (void);
void limits_set_machine_positions (axes_signals_t cycle, bool add_pulloff);

void limit_interrupt_handler (axes_signals_t state);

#endif
