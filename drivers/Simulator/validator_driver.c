/*
  validator_driver.c - driver code for simulator MCU

  Part of GrblHAL

  Copyright (c) 2020 Terje Io

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

#include "mcu.h"
#include "driver.h"
#include "serial.h"
#include "eeprom.h"
#include "grbl_eeprom_extensions.h"
#include "platform.h"

#include "grbl/grbl.h"

/* don't delay at all in validator */
static void driver_delay_ms (uint32_t ms, void (*callback)(void))
{
    if(callback)
        callback();
}

/* Dummy functions */

static void stepperEnable (axes_signals_t enable)
{
}

static void stepperWakeUp (void)
{
}

static void stepperGoIdle (bool clear_signals)
{
}

static void stepperCyclesPerTick (uint32_t cycles_per_tick)
{
}

static void stepperPulseStart (stepper_t *stepper)
{
}

static void limitsEnable (bool on, bool homing)
{
}

static axes_signals_t limitsGetState()
{
    axes_signals_t signals = {0};

    return signals;
}

static control_signals_t systemGetState (void)
{
    control_signals_t signals = {0};

    return signals;
}

static void probeConfigureInvertMask (bool is_probe_away)
{
}

probe_state_t probeGetState (void)
{
    probe_state_t state = {
        .connected = Off
    };

    state.triggered = false;

    return state;
}

// Start or stop spindle
static void spindleSetState (spindle_state_t state, float rpm)
{
}

// Variable spindle control functions

// Sets spindle speed
static void spindle_set_speed (uint_fast16_t pwm_value)
{
}

#ifdef SPINDLE_PWM_DIRECT

static uint_fast16_t spindleGetPWM (float rpm)
{
    return 0; //spindle_compute_pwm_value(&spindle_pwm, rpm, false);
}

#else

static void spindleUpdateRPM (float rpm)
{
}

#endif

// Returns spindle state in a spindle_state_t variable
static spindle_state_t spindleGetState (void)
{
    spindle_state_t state = {0};

    return state;
}

static void coolantSetState (coolant_state_t mode)
{
}

static coolant_state_t coolantGetState (void)
{
    coolant_state_t state = {0};

    return state;
}

static void bitsSetAtomic (volatile uint_fast16_t *ptr, uint_fast16_t bits)
{
    *ptr |= bits;
}

static uint_fast16_t bitsClearAtomic (volatile uint_fast16_t *ptr, uint_fast16_t bits)
{
    uint_fast16_t prev = *ptr;
    *ptr &= ~bits;
    return prev;
}

static uint_fast16_t valueSetAtomic (volatile uint_fast16_t *ptr, uint_fast16_t value)
{
    uint_fast16_t prev = *ptr;
    *ptr = value;
    return prev;
}

void settings_changed (settings_t *settings)
{
}

bool driver_setup (settings_t *settings)
{
    return true;
}

uint16_t serial_get_rx_buffer_available()
{
    return RX_BUFFER_SIZE;
}

bool driver_init ()
{
    hal.info = "Validator";
    hal.driver_version = "200528";
    hal.driver_setup = driver_setup;
    hal.rx_buffer_size = RX_BUFFER_SIZE;
    hal.f_step_timer = F_CPU;
    hal.delay_ms = driver_delay_ms;
    hal.settings_changed = settings_changed;

    hal.stepper_wake_up = stepperWakeUp;
    hal.stepper_go_idle = stepperGoIdle;
    hal.stepper_enable = stepperEnable;
    hal.stepper_cycles_per_tick = stepperCyclesPerTick;
    hal.stepper_pulse_start = stepperPulseStart;

    hal.limits_enable = limitsEnable;

    hal.limits_get_state = limitsGetState;

    hal.coolant_set_state = coolantSetState;
    hal.coolant_get_state = coolantGetState;

    hal.probe_get_state = probeGetState;
    hal.probe_configure_invert_mask = probeConfigureInvertMask;

    hal.spindle_set_state = spindleSetState;
    hal.spindle_get_state = spindleGetState;
#ifdef SPINDLE_PWM_DIRECT
    hal.spindle_get_pwm = spindleGetPWM;
    hal.spindle_update_pwm = spindle_set_speed;
#else
    hal.spindle_update_rpm = spindleUpdateRPM;
#endif

    hal.system_control_get_state = systemGetState;

    hal.eeprom.type = EEPROM_None;

    hal.set_bits_atomic = bitsSetAtomic;
    hal.clear_bits_atomic = bitsClearAtomic;
    hal.set_value_atomic = valueSetAtomic;

  // driver capabilities, used for announcing and negotiating (with Grbl) driver functionality

    hal.driver_cap.amass_level = 3;
    hal.driver_cap.spindle_dir = On;

    hal.driver_cap.variable_spindle = On;
    hal.driver_cap.spindle_dir = On;

    hal.driver_cap.variable_spindle = On;
    hal.driver_cap.spindle_pwm_invert = On;
    hal.driver_cap.spindle_pwm_linearization = On;
    hal.driver_cap.mist_control = On;

    hal.driver_cap.safety_door = On;
    hal.driver_cap.control_pull_up = On;
    hal.driver_cap.limits_pull_up = On;
    hal.driver_cap.probe_pull_up = On;

    // no need to move version check before init - compiler will fail any signature mismatch for existing entries
    return hal.version == 6;
}
