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

#ifndef TEXT_TO_MORSE_SPACE_H
#define TEXT_TO_MORSE_SPACE_H

int16_t *space_get_inter_character(void);
size_t space_get_inter_character_len(void);
int16_t *space_get_intra_character(void);
size_t space_get_intra_character_len(void);
int16_t *space_get_inter_word_space(void);
size_t space_get_inter_word_len(void);

int space_init(int wpm, int fwpm);
void space_exit(void);

#endif
