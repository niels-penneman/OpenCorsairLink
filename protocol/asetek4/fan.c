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
#include "../../lowlevel/asetek4.h"
#include "../../device.h"
#include "../../driver.h"
#include "core.h"

int corsairlink_asetek_fan_mode(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	uint8_t fan_mode)
{
	int r;
	struct fan_table curve;
	if (fan_mode == PERFORMANCE) {
		ASETEK_FAN_TABLE_EXTREME(curve);
	}
	else if (fan_mode == QUIET) {
		ASETEK_FAN_TABLE_QUIET(curve);
	}
	r = dev->driver->fan.custom(dev, handle, &curve);

	return r;
}

int corsairlink_asetek_fan_curve(struct corsair_device_info *dev, struct libusb_device_handle *handle,
	struct fan_table *fan)
{
	int r;
	uint8_t response[32];
	uint8_t commands[32] ;
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = FanCurve;
	commands[1] = UnknownFanCurve;
	
	commands[2] = fan->t1;
	commands[3] = fan->t2;
	commands[4] = fan->t3;
	commands[5] = fan->t4;
	commands[6] = fan->t5;
	commands[7] = fan->t6;

	commands[8] = fan->s1;
	commands[9] = fan->s2;
	commands[10] = fan->s3;
	commands[11] = fan->s4;
	commands[12] = fan->s5;
	commands[13] = fan->s6;
	
	r = dev->driver->write(handle, dev->write_endpoint, commands, 14);
	r = dev->driver->read(handle, dev->read_endpoint, response, 32);

	return r;
}
