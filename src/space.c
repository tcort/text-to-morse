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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "nsamples.h"
#include "space.h"

/* prebuilt waveforms for spaces */
static int16_t *inter_character_space = NULL;	static size_t inter_character_space_len = 0;
static int16_t *intra_character_space = NULL;	static size_t intra_character_space_len = 0;
static int16_t *inter_word_space = NULL;	static size_t inter_word_space_len = 0;

int16_t *space_get_inter_character(void)	{ return inter_character_space; }
size_t space_get_inter_character_len(void)	{ return inter_character_space_len; }
int16_t *space_get_intra_character(void)	{ return intra_character_space; }
size_t space_get_intra_character_len(void)	{ return intra_character_space_len; }
int16_t *space_get_inter_word_space(void)	{ return inter_word_space; }
size_t space_get_inter_word_len(void)		{ return inter_word_space_len; }

/* writes silence of `nsamples` to `samples` */
static void space_make(int16_t *samples, size_t nsamples) {
	memset(samples, 0, sizeof(int16_t) * nsamples);
}

/* Pre-render silence samples for space between elements, characters, and words */
int space_init(int wpm, int fwpm) {
	inter_character_space_len = nsamples_inter_character_space(fwpm);
	inter_character_space = (int16_t *) malloc(inter_character_space_len * sizeof(int16_t));
	if (inter_character_space == NULL) {
		return -1;
	}
	space_make(inter_character_space, inter_character_space_len);

	intra_character_space_len = nsamples_intra_character_space(wpm);
	intra_character_space = (int16_t *) malloc(intra_character_space_len * sizeof(int16_t));
	if (intra_character_space == NULL) {
		return -1;
	}
	space_make(intra_character_space, intra_character_space_len);

	inter_word_space_len = nsamples_inter_word_space(fwpm);
	inter_word_space = (int16_t *) malloc(inter_word_space_len * sizeof(int16_t));
	if (inter_word_space == NULL) {
		return -1;
	}
	space_make(inter_word_space, inter_word_space_len);

	return 0;
}

/* clean-up silence */
void space_exit(void) {
	free(inter_character_space);	free(intra_character_space);	free(inter_word_space);
	inter_character_space		= intra_character_space		= inter_word_space	= NULL;
	inter_character_space_len	= intra_character_space_len	= inter_word_space_len	= 0;
}
