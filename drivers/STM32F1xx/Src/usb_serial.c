/*

  usb_serial.c - USB serial port implementation for STM32F103C8 ARM processors

  Part of GrblHAL

  Copyright (c) 2019-2020 Terje Io

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

#include "driver.h"
#include "serial.h"
#include "../grbl/grbl.h"

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

static stream_rx_buffer_t rxbuf = {0}, rxbackup;

#define USB_TXLEN 200

typedef struct {
    size_t length;
    char *s;
    char data[USB_TXLEN];
} usb_tx_buf;

usb_tx_buf txbuf = {0};

void usbInit (void)
{
    MX_USB_DEVICE_Init();

    txbuf.s = txbuf.data;
}

//
// Returns number of free characters in the input buffer
//
uint16_t usbRxFree (void)
{
    uint16_t tail = rxbuf.tail, head = rxbuf.head;
    return RX_BUFFER_SIZE - BUFCOUNT(head, tail, RX_BUFFER_SIZE);
}

//
// Flushes the input buffer
//
void usbRxFlush (void)
{
    rxbuf.head = rxbuf.tail = 0;
}

//
// Flushes and adds a CAN character to the input buffer
//
void usbRxCancel (void)
{
    rxbuf.data[rxbuf.head] = ASCII_CAN;
    rxbuf.tail = rxbuf.head;
    rxbuf.head = (rxbuf.tail + 1) & (RX_BUFFER_SIZE - 1);
}

//
// Writes a null terminated string to the USB output stream, blocks if buffer full
// Buffers string up to EOL (LF) before transmitting
//
void usbWriteS (const char *s)
{
    size_t length = strlen(s);

    if(length + txbuf.length < USB_TXLEN) {
        memcpy(txbuf.s, s, length);
        txbuf.length += length;
        txbuf.s += length;
        if(s[length - 1] == '\n') {
            length = txbuf.length;
            txbuf.length = 0;
            txbuf.s = txbuf.data;
            while(CDC_Transmit_FS((uint8_t *)txbuf.data, length) == USBD_BUSY) {
                if(!hal.stream_blocking_callback())
                    return;
            }
            if(length % 64 == 0) {
            	while(CDC_Transmit_FS((uint8_t *)txbuf.data, 0) == USBD_BUSY) {
                    if(!hal.stream_blocking_callback())
                        return;
                }
            }
        }
    }
//  while(CDC_Transmit_FS((uint8_t*)s, strlen(s)) == USBD_BUSY);
}

//
// usbGetC - returns -1 if no data available
//
int16_t usbGetC (void)
{
    uint16_t bptr = rxbuf.tail;

    if(bptr == rxbuf.head)
        return -1; // no data available else EOF

    char data = rxbuf.data[bptr++];             // Get next character, increment tmp pointer
    rxbuf.tail = bptr & (RX_BUFFER_SIZE - 1);   // and update pointer

    return (int16_t)data;
}

// "dummy" version of serialGetC
static int16_t usbGetNull (void)
{
    return -1;
}

bool usbSuspendInput (bool suspend)
{
    if(suspend)
        hal.stream.read = usbGetNull;
    else if(rxbuf.backup)
        memcpy(&rxbuf, &rxbackup, sizeof(stream_rx_buffer_t));

    return rxbuf.tail != rxbuf.head;
}

void usbBufferInput (uint8_t *data, uint32_t length)
{
    while(length--) {

        uint_fast16_t next_head = (rxbuf.head + 1)  & (RX_BUFFER_SIZE - 1); // Get and increment buffer pointer

        if(rxbuf.tail == next_head) {                                       // If buffer full
            rxbuf.overflow = 1;                                             // flag overflow
        } else {
            if(*data == CMD_TOOL_ACK && !rxbuf.backup) {

                memcpy(&rxbackup, &rxbuf, sizeof(stream_rx_buffer_t));
                rxbuf.backup = true;
                rxbuf.tail = rxbuf.head;
                hal.stream.read = usbGetC; // restore normal input

            } else if(!hal.stream.enqueue_realtime_command(*data)) {        // Check and strip realtime commands,
                rxbuf.data[rxbuf.head] = *data;                             // if not add data to buffer
                rxbuf.head = next_head;                                     // and update pointer
            }
        }
        data++;                                                             // next
    }
}
