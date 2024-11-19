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

#include "args.h"
#include "encoder.h"
#include "morse.h"
#include "nsamples.h"
#include "render.h"
#include "space.h"
#include "tone.h"
#include "timing.h"
#include "version.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* 18 wpm default, 600 Hz tone default */
#define WPM (18)
#define FREQUENCY (600)

static int verbose = 0; /* verbose output, higher number === more verbosity */

/*
 * main program: accepts command line arguments, pre-renders elements,
 * renders and encodes the audio, cleans-up, exits
 */
int main(int argc, char *argv[]) {

	FILE *input = NULL;
	char ch = '\0';
	int wpm = WPM;
	int fwpm = 0;
	int rc = 0;
	int i = 0;
	int frequency = FREQUENCY;

	uint64_t ms_started;
	uint64_t ms_rendered;
	uint64_t ms_encoded;

	struct prog_arg *arg;

	static struct prog_arg args[] = {
		{
			.arg = 'f',
			.longarg = "fwpm",
			.description = "Farnsworth spacing words per minute. Default 18.",
			.has_value = 1
		},
		PROG_ARG_HELP,
		{
			.arg = 't',
			.longarg = "tone",
			.description = "the frequency of the generated tone in Hertz. Default 600.",
			.has_value = 1
		},
		{
			.arg = 'v',
			.longarg = "verbose",
			.description = "verbose debugging output",
			.has_value = 0 },
		PROG_ARG_VERSION,
		{
			.arg = 'w',
			.longarg = "wpm",
			.description = "words per minute. Default 18.",
			.has_value = 1
		},
		PROG_ARG_END
	};

	static struct prog_example examples[] = {
		{ .command = "text-to-morse hello.txt hello.flac", .description = "converts text file 'hello.txt' into a morse code audio file 'hello.flac'" },
		{ .command = "text-to-morse -w 20 -f 12 -t 760 hello.txt hello.flac", .description = "convert hello.txt to hello.flac with 760 Hz tone, 20 WPM, 12 FWPM" },
		PROG_EXAMPLE_END
	};

	static struct prog prog = {
		.program = "text-to-morse",
		.usage = "[OPTIONS] INPUT.TXT OUTPUT.FLAC",
		.description = "converts text into a morse code audio file",
		.package = TEXT_TO_MORSE_PROJECT_NAME,
		.version = TEXT_TO_MORSE_PROJECT_VERSION,
		.copyright = TEXT_TO_MORSE_PROJECT_COPYRIGHT,
		.license = TEXT_TO_MORSE_PROJECT_LICENSE,
		.author =  TEXT_TO_MORSE_PROJECT_AUTHOR,
		.args = args,
		.examples = examples
	};

	ms_started = now_ms();

	while ((arg = args_process(&prog, argc, argv)) != NULL) {
		switch (arg->arg) {
			case 'f':
				fwpm = atoi(argval);
				fwpm = fwpm < 1 || fwpm > 100 ? 0 : fwpm;
				break;
			case 'h':
				args_show_help(&prog);
				break;
			case 't':
				frequency = atoi(argval);
				frequency = frequency < 100 || frequency > 1000 ? FREQUENCY : frequency;
				break;
			case 'v':
				verbose += 1;
				break;
			case 'V':
				args_show_version(&prog);
				break;
			case 'w':
				wpm = atoi(argval);
				wpm = wpm < 1 || wpm > 100 ? WPM : wpm;
				break;
		}

	}

	argc -= argi;
	argv += argi;

	if (fwpm == 0) {
		fwpm = wpm;
	}

	if (argc != 2) {
		args_show_usage(&prog);
	}

	input = fopen(argv[0], "r");
	if (input == NULL) {
		fprintf(stderr, "Could not open input file '%s'\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = space_init(wpm, fwpm);
	if (rc == -1) {
		fprintf(stderr, "Failed to initialize space\n");
		exit(EXIT_FAILURE);
	}

	rc = tone_init(wpm, frequency);
	if (rc == -1) {
		fprintf(stderr, "Failed to initialize tones\n");
		exit(EXIT_FAILURE);
	}

	render_text(input);

	tone_exit();
	space_exit();

	fclose(input);

	ms_rendered = now_ms();

	rc = encoder_encode(argv[1], render_get_buf(), render_get_buf_len(), render_get_total_samples());

	ms_encoded = now_ms();

	if (verbose > 0) {

		fprintf(stdout, "Render Time: %llu ms\n", ms_rendered - ms_started);
		fprintf(stdout, "Encode Time: %llu ms\n", ms_encoded - ms_rendered);
		fprintf(stdout, "Memory Usage: %lu bytes\n", render_get_buf_len());
	}

	render_exit();

	exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
