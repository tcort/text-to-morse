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

#include "encoder.h"
#include "nsamples.h"
#include "tone.h"

#include <stdint.h>
#include <math.h>

/* prebuilt waveforms for dit, dah, and spaces */
static int16_t *dit_tone = NULL;		static size_t dit_tone_len = 0;
static int16_t *dah_tone = NULL;		static size_t dah_tone_len = 0;

int16_t *tone_get_dit(void)	{ return dit_tone;	}
size_t  tone_get_dit_len(void)	{ return dit_tone_len;	}
int16_t *tone_get_dah(void)	{ return dah_tone;	}
size_t  tone_get_dah_len(void)	{ return dah_tone_len;	}

/*
 * Writes a sine wave of `nsamples` to `samples` at `frequency` with the given `rise_time` and `fall_time`
 */
static void tone_make(int16_t *samples, size_t nsamples, int rise_time, int fall_time, int frequency) {
	int i;
	for (i = 0; i < nsamples; i++) {
		double t = (double) i / SAMPLE_RATE;
		samples[i] = VOLUME * sin(frequency*t*2*M_PI);

		if (i < rise_time) {
			samples[i] = samples[i] * (i*1.0/rise_time*1.0);
		} else if (i > nsamples - fall_time) {
			samples[i] = samples[i] * ((nsamples-i)*1.0/fall_time*1.0);
		}
	}
}

/*
 * Pre-render dit and dah samples to use when building characters
 */
int tone_init(int wpm, int frequency) {

	int rise_time;
	int fall_time;

	rise_time = nsamples_rise_time(wpm);
	fall_time = nsamples_fall_time(wpm);

	dit_tone_len = nsamples_dit(wpm);
	dit_tone = (int16_t *) malloc(dit_tone_len * sizeof(int16_t));
	if (dit_tone == NULL) {
		return -1;
	}
	tone_make(dit_tone, dit_tone_len, rise_time, fall_time, frequency);

	dah_tone_len = nsamples_dah(wpm);
	dah_tone = (int16_t *) malloc(dah_tone_len * sizeof(int16_t));
	if (dah_tone == NULL) {
		return -1;
	}
	tone_make(dah_tone, dah_tone_len, rise_time, fall_time, frequency);

	return 0;
}

/* clean-up dit and dah samples */
void tone_exit(void) {
	free(dit_tone);	free(dah_tone);
	dit_tone	= dah_tone	= NULL;
	dit_tone_len	= dah_tone_len	= 0;
}

