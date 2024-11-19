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

#include "morse.h"
#include "render.h"
#include "space.h"
#include "tone.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* resulting waveform before encoding */
static int16_t *result = NULL;
static size_t result_len = 0;
static size_t total_samples = 0;

int16_t *render_get_buf(void)		{ return result; }
size_t render_get_buf_len(void)		{ return result_len; }
size_t render_get_total_samples(void)	{ return total_samples; }

/*
 * Append raw samples to the `result` buffer, growing it as needed.
 */
static void render_buf_append(int16_t *data, size_t len) {

	int16_t *new_result = NULL;
	size_t new_len = 0;

	new_len = result_len + len;

	new_result = (int16_t *) realloc(result, new_len * sizeof(int16_t));
	if (new_result == NULL) {
		fprintf(stderr, "malloc failed :(\n");
		exit(EXIT_FAILURE);
	}

	memcpy(new_result + result_len, data, len * sizeof(int16_t));

	result = new_result;
	result_len += len;
	total_samples = result_len;

}


/* write pre-rendered samples to the `result` buffer */
static void render_dit(void) { render_buf_append(tone_get_dit(), tone_get_dit_len()); }
static void render_dah(void) { render_buf_append(tone_get_dah(), tone_get_dah_len()); }
static void render_inter_character_space(void) { render_buf_append(space_get_inter_character(), space_get_inter_character_len()); }
static void render_intra_character_space(void) { render_buf_append(space_get_intra_character(), space_get_intra_character_len()); }
static void render_inter_word_space(void) { render_buf_append(space_get_inter_word_space(), space_get_inter_word_len()); }

/* build a character out of dits, dahs, and/or spaces */
static void render_character(unsigned char c) {
	int i;
	const char *s = morse_alphabet[c];

	if (s[0] == '\0') return;
	for (i = 0; s[i] != '\0'; i++) {
		if (i != 0) {
			render_intra_character_space();
		}
		switch (s[i]) {
			case ' ':
				render_inter_word_space();
				break;
			case '.':
				render_dit();
				break;
			case '-':
				render_dah();
				break;
		}
	}
}

void render_text(FILE *input) {
	int i;
	char ch;

	for (i = 0; (ch = getc(input)) != EOF; i++) {
		if (i != 0) {
			render_inter_character_space();
		}
		render_character(ch);
	}
}

void render_exit(void) {
	if (result != NULL) {
		free(result);
		result = NULL;
	}
	result_len = 0;
	total_samples = 0;
}
