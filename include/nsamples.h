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

#ifndef TEXT_TO_MORSE_NSAMPLES_H
#define TEXT_TO_MORSE_NSAMPLES_H

int nsamples_unit(int wpm);
int nsamples_dit(int wpm);
int nsamples_dah(int wpm);
int nsamples_intra_character_space(int wpm);
int nsamples_inter_character_space(int wpm);
int nsamples_inter_word_space(int wpm);
int nsamples_per_ms(void);
int nsamples_rise_time(int wpm);
int nsamples_fall_time(int wpm);

#endif
