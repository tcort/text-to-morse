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

/* useful timing details: https://morsecode.world/international/timing.html */
int nsamples_unit(int wpm) { return SAMPLE_RATE * (60.0 / (50.0 * wpm)); }
int nsamples_dit(int wpm) { return 1 * nsamples_unit(wpm); }
int nsamples_dah(int wpm) { return 3 * nsamples_unit(wpm); }
int nsamples_intra_character_space(int wpm) { return 1 * nsamples_unit(wpm); }
int nsamples_inter_character_space(int wpm) { return 3 * nsamples_unit(wpm); }
int nsamples_inter_word_space(int wpm)      { return 5 * nsamples_unit(wpm); }
/* inter word space is 5 because there are nsamples_intra_character_space
   before and after the space to bring it up to 7 */

/* shape output waveform so sound isn't as harsh, rise and fall is Xms */
int nsamples_per_ms(void) { return SAMPLE_RATE / 1000; }
int nsamples_rise_time(int wpm) { return ((wpm > 30) ? 5 : 6) * nsamples_per_ms(); }
int nsamples_fall_time(int wpm) { return ((wpm > 30) ? 5 : 6) * nsamples_per_ms(); }
/* Values suggested by Petr, OK1RP https://groups.io/g/RGO-ONE/message/805 */

