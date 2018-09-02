/*
  nuts_bolts.h - Header file for shared definitions, variables, and functions
  Part of Grbl

  Copyright (c) 2017-2018 Terje Io
  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
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

#ifndef nuts_bolts_h
#define nuts_bolts_h

#include <stdint.h>
#include <stdbool.h>

#ifndef true
#define false 0
#define true 1
#endif

#define Off 0
#define On 1

#define SOME_LARGE_VALUE 1.0E+38f
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Convert character to uppercase
#define CAPS(c) ((c >= 'a' && c <= 'z') ? c & 0x5F : c)

// Axis array index values. Must start with 0 and be continuous.
#define N_AXIS 3 // Number of axes
#define X_AXIS 0 // Axis indexing value.
#define Y_AXIS 1
#define Z_AXIS 2
#define X_AXIS_BIT bit(X_AXIS)
#define Y_AXIS_BIT bit(Y_AXIS)
#define Z_AXIS_BIT bit(Z_AXIS)
#if N_AXIS > 3
#define A_AXIS 3
#define A_AXIS_BIT bit(A_AXIS)
#endif
#if N_AXIS > 4
#define B_AXIS 4
#define B_AXIS_BIT bit(B_AXIS)
#endif
#if N_AXIS == 6
#define C_AXIS 5
#define C_AXIS_BIT bit(C_AXIS)
#define AXES_BITMASK (X_AXIS_BIT|Y_AXIS_BIT|Z_AXIS_BIT|A_AXIS_BIT|B_AXIS_BIT|C_AXIS_BIT)
#endif

#if N_AXIS == 3
#define AXES_BITMASK (X_AXIS_BIT|Y_AXIS_BIT|Z_AXIS_BIT)
#elif N_AXIS == 4
#define AXES_BITMASK (X_AXIS_BIT|Y_AXIS_BIT|Z_AXIS_BIT|A_AXIS_BIT)
#elif N_AXIS == 5
#define AXES_BITMASK (X_AXIS_BIT|Y_AXIS_BIT|Z_AXIS_BIT|A_AXIS_BIT|B_AXIS_BIT)
#endif

// CoreXY motor assignments. DO NOT ALTER.
// NOTE: If the A and B motor axis bindings are changed, this effects the CoreXY equations.
#ifdef COREXY
#define A_MOTOR X_AXIS // Must be X_AXIS
#define B_MOTOR Y_AXIS // Must be Y_AXIS
#endif

typedef union {
    uint8_t mask;
    uint8_t value;
    struct {
        uint8_t x :1,
                y :1,
                z :1,
                a :1,
                b :1,
                c :1;
    };
} axes_signals_t;

// Conversions
#define MM_PER_INCH (25.40f)
#define INCH_PER_MM (0.0393701f)

typedef enum {
    DelayMode_Dwell = 0,
    DelayMode_SysSuspend
} delaymode_t;

// Useful macros
#define clear_vector(a) memset(a, 0, sizeof(a))
#define clear_coord_data(a) memset(a, 0.0f, sizeof(coord_data_t))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define isequal_position_vector(a,b) !memcmp(a, b, sizeof(coord_data_t))

// Bit field and masking macros
#define bit(n) (1UL << n)
#define bit_true(x,mask) (x) |= (mask)
#define bit_false(x,mask) (x) &= ~(mask)
#define BIT_SET(x, bit, v) { if (v) { x |= (bit); } else { x &= ~(bit); } }
//#define bit_set(x, y, z) HWREGBITW(&x, y) = z;

#define bit_istrue(x,mask) ((x & mask) != 0)
#define bit_isfalse(x,mask) ((x & mask) == 0)

// Read a floating point value from a string. Line points to the input buffer, char_counter
// is the indexer pointing to the current character of the line, while float_ptr is
// a pointer to the result variable. Returns true when it succeeds
bool read_float(char *line, uint_fast8_t *char_counter, float *float_ptr);

// Non-blocking delay function used for general operation and suspend features.
void delay_sec(float seconds, delaymode_t mode);

float convert_delta_vector_to_unit_vector(float *vector);
float limit_value_by_axis_maximum(float *max_value, float *unit_vec);

// calculate checksum byte for EEPROM data
uint8_t calc_checksum (uint8_t *data, uint32_t size);

#endif