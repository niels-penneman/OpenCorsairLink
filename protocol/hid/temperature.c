/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>

#include "../../lowlevel/hid.h"
#include "../../device.h"
#include "../../driver.h"
#include "core.h"

int corsairlink_hid_temperature(struct corsair_device_info *dev, struct libusb_device_handle *handle, uint8_t selector, uint16_t *temperature)
{
	int r;
	uint8_t response[64];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	uint8_t i = 1;

	commands[i++] = CommandId++; // Command ID
	commands[i++] = WriteOneByte; // Command Opcode
	commands[i++] = TEMP_SelectActiveSensor; // Command data...
	commands[i++] = selector;

	commands[i++] = CommandId++; // Command ID
	commands[i++] = ReadTwoBytes; // Command Opcode
	commands[i++] = TEMP_Read; // Command data...
	commands[i++] = 0x00;
	commands[i++] = 0x00;

	commands[0] = i; // Length

	r = dev->driver->write(handle, dev->write_endpoint, commands, i);
	r = dev->driver->read(handle, dev->read_endpoint, response, 64);

	*(temperature) = (response[5]<<8) + response[4];

	return r;
}
