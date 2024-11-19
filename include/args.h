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

#ifndef TEXT_TO_MORSE_ARGS_H
#define TEXT_TO_MORSE_ARGS_H

struct prog_arg {
	char arg;
	char *longarg;
	char *description;
	int has_value;
};

struct prog_example {
	char *description;
	char *command;
};

struct prog {
	char *program;
	char *version;
	char *package;
	char *description;
	char *copyright;
	char *license;
	char *author;
	char *usage;
	struct prog_arg *args;
	struct prog_example *examples;
};

#define PROG_ARG_VERBOSE { .arg = 'v', .longarg = "verbose", .description = "print verbose debugging details", .has_value = 0 }
#define PROG_ARG_SILENT { .arg = 's', .longarg = "silent", .description = "suppress output; yield exit status only", .has_value = 0 }
#define PROG_ARG_HELP { .arg = 'h', .longarg = "help", .description = "print help text", .has_value = 0 }
#define PROG_ARG_VERSION { .arg = 'V', .longarg = "version", .description = "print version and copyright info", .has_value = 0 }
#define PROG_ARG_END { .arg = '\0', .longarg = NULL, .description = NULL, .has_value = 0 }
#define PROG_EXAMPLE_END { .description = NULL, .command = NULL }

extern int argi;
extern char *argval;

struct prog_arg *args_process(struct prog *prog, int argc, char *argv[]);
void args_show_help(struct prog *prog);
void args_show_version(struct prog *prog);
void args_show_usage(struct prog *prog);

#endif
