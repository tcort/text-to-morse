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

#ifndef TEXT_TO_MORSE_ENCODER_H
#define TEXT_TO_MORSE_ENCODER_H

#include <stdint.h>
#include <stdlib.h>

/* Audio Settings - 8 kHz sample rate, 16 bits per sample, mono.
 * Original was 44.1 kHz but no perceptable difference at 8 kHz,
 * so it was reduced to keep output file sizes small.
 */
#define CHANNELS (1)
#define SAMPLE_RATE (8000)
#define BPS (16)

/* encoder settings - max compression level (8), verify enabled */
#define COMPRESSION_LEVEL (8)
#define VERIFY (1)

/* 50% volume, 18 wpm default, 600 Hz tone default */
#define VOLUME (8192)

int encoder_encode(char *filepath, int16_t *result, size_t result_len, size_t total_samples);

#endif
