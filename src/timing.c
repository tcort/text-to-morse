 /*
    text-to-morse -- converts text into a morse code audio file
    Copyright (C) 2024  Thomas Cort

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "timing.h"

#include <stdint.h>
#include <sys/time.h>

uint64_t now_ms(void) {

	uint64_t ms;
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000);

	return ms;
}
