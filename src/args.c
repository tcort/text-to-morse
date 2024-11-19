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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Computes the length of a string.
 * Return the length of string s.
 */
static int tc_strlen(const char *s) {
	int n;

	if (s == NULL) {
		return 0;
	}

	n = 0;
	while (s[n] != '\0') {
		n++;
	}

	return n;
}

/*
 * Compare upto n characters of string x with string y
 * This is not a constant time equality check.
 * Returns 1 if equal, 0 if not equal
 */
static int tc_strneql(const char *x, const char *y, int n) {

	int i;

	if (x == y) { /* both the same */
		return 1;
	} else if (x == NULL || y == NULL) { /* one null */
		return 0;
	} else if (tc_strlen(x) < n || tc_strlen(y) < n) { /* at least n */
		return 0;
	}

	for (i = 0; i < n; i++) {
		if (x[i] != y[i]) {
			return 0;
		}
	}

	return 1;
}

/*
 * Compare string x with string y
 * This is not a constant time equality check.
 * Returns 1 if equal, 0 if not equal
 */
static int tc_streql(const char *x, const char *y) {

	int i;

	if (x == y) { /* both the same */
		return 1;
	} else if (x == NULL || y == NULL) { /* one null */
		return 0;
	} else if (tc_strlen(x) != tc_strlen(y)) { /* different lengths */
		return 0;
	}

	for (i = 0; i < tc_strlen(x); i++) {
		if (x[i] != y[i]) {
			return 0;
		}
	}

	return 1;
}

/*
 * Looks for character 'c' in string 's'
 * Returns index of first occurance or -1
 */
static int tc_strchr(char *s, char c) {
	int i;

	for (i = 0; i < tc_strlen(s); i++) {
		if (s[i] == c) {
			return i;
		}
	}

	return -1;
}

/*
 * Show usage and exit
 *
 * Parameters:
 *  prog - program info
 */
void args_show_usage(struct prog *prog) {

	/* usage: tcbasic [OPTIONS] FILENAME */
	fprintf(stdout, "usage: %s %s\n", prog->program, prog->usage);
	exit(EXIT_SUCCESS);
}

/*
 * Show help message and exit
 *
 * Parameters:
 *  prog - program info
 */
void args_show_help(struct prog *prog) {
	int i;

	/* tcbasic -- an interpreter for the programming language BASIC */
	fprintf(stdout, "%s -- %s\n", prog->program, prog->description);

	/* <br> */
	fprintf(stdout, "\n");

	/* usage: tcbasic [OPTIONS] FILENAME */
	fprintf(stdout, "usage: %s %s\n", prog->program, prog->usage);

	/* <br> */
	fprintf(stdout, "\n");

	/* <describe each argument> */
	for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != NULL; i++) {

		/*  -h, --help     print help text */
		fprintf(stdout, "  ");

		if (prog->args[i].arg != '\0') { /* have a short arg? */
			fprintf(stdout, "-%c", prog->args[i].arg);

			/* followed by a long arg? */
			if (prog->args[i].longarg != NULL) {
				fprintf(stdout, ", ");
			}
		}

		if (prog->args[i].longarg != NULL) { /* have a long arg? */
			fprintf(stdout, "--%s", prog->args[i].longarg);
		}

		fprintf(stdout, "\t%s\n", prog->args[i].description);
	}

	/* <br> */
	fprintf(stdout, "\n");

	/* examples */
	fprintf(stdout, "examples:\n");
	for (i = 0; prog->examples[i].command != NULL && prog->examples[i].description != NULL; i++) {

		/* <br> */
		fprintf(stdout, "\n");

		/* example commentary */
		if (prog->examples[i].description != NULL) {
			fprintf(stdout, "  # %s\n", prog->examples[i].description);
		}

		/* example command */
		if (prog->examples[i].command != NULL) {
			fprintf(stdout, "  %s\n", prog->examples[i].command);
		}

	}

	/* <br> */
	fprintf(stdout, "\n");

	exit(EXIT_SUCCESS);
}

/*
 * Show version message and exit
 *
 * Parameters:
 *  prog - program info
 */
void args_show_version(struct prog *prog) {

	fprintf(stdout, "%s v%s\n", prog->program, prog->version);
	fprintf(stdout, "%s\n", prog->copyright);
	fprintf(stdout, "%s\n", prog->license);
	fprintf(stdout, "\n");
	fprintf(stdout, "Written by %s\n", prog->author);

	exit(EXIT_SUCCESS);
}

char *argval = NULL;
int argi = 1;
static int argii = 1;

/*
 * Process an array of arguments, one at a time.
 *
 * Parameters:
 *  prog - description of the program
 *  argc - number of arguments in argv
 *  argv - array of command line arguments
 *
 * Returns a parsed command line argument or NULL
 */
struct prog_arg *args_process(struct prog *prog, int argc, char *argv[]) {

	int i;
	struct prog_arg *arg;
	static struct prog_arg arg_invalid = PROG_ARG_END;

	if (argi >= argc) { /* no more arguments to process */
		return NULL;
	}

	if (argv[argi][0] != '-') { /* argument isn't a arg/longarg, done */
		return NULL;
	}

 	if (tc_streql(argv[argi], "--") == 1) { /* explicit end of args */
		return NULL;
	}

	argval = NULL;

	if (tc_strneql(argv[argi], "--", 2) == 1) { /* longarg */

		int eql_sign = tc_strchr(argv[argi], '=');

		/* --longarg [value] */
		if (eql_sign == -1) {

			for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != NULL; i++) {
				if (tc_streql(prog->args[i].longarg, &(argv[argi][tc_strlen("--")])) == 1) {
					struct prog_arg *res;
					res = &(prog->args[i]);

					argi += 1;

					/* expect a value e.g. '-n 5' */
					if (prog->args[i].has_value == 1) {
						if (argv[argi] != NULL) {
							argval = argv[argi];
							argi += 1;
							argii = 1;
						} else {
							break;
						}
					}

					return res;
				}
			}

		/* --longarg=value */
		} else if (eql_sign > 2) {

			int longarg_len = eql_sign - 2; /* --foo=barbaz -> 3 */

			for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != NULL; i++) {
				if (tc_strneql(prog->args[i].longarg, &(argv[argi][tc_strlen("--")]), longarg_len) == 1) {
					struct prog_arg *res;

					res = &(prog->args[i]);

					/* expect a value e.g. '--foo=barbaz' */
					if (prog->args[i].has_value == 1) {
						argval = &(argv[argi][eql_sign + 1]);
						argi += 1;
						argii = 1;
					}


					return res;
				}
			}

		}

	} else { /* short arg */

		for (i = 0; prog->args[i].arg != '\0' && prog->args[i].longarg != NULL; i++) {

			if (prog->args[i].arg == argv[argi][argii]) {
				struct prog_arg *res;

				res = &(prog->args[i]);

				/* more to go? e.g. ls -al */
				if (argv[argi][argii + 1] == '\0') {
					argi += 1;
					argii = 1;
				} else {
					argii += 1;
				}

				/* expect a value e.g. '-n 5' */
				if (prog->args[i].has_value == 1) {
					if (argv[argi] != NULL) {
						argval = argv[argi];
						argi += 1;
						argii = 1;
					} else {
						break;
					}
				}
				return res;
			}
		}

	}

	fprintf(stderr, "Invalid argument: %s\n", argv[argi]);
	fprintf(stderr, "Try '%s --help' for more information\n", prog->program);

	exit(EXIT_FAILURE);
	return NULL; /* should not reach this line unless exit fails */
}
