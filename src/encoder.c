 /*
    text-to-morse -- converts text into a morse code audio file
    Copyright (C) 2007-2009  Josh Coalson
    Copyright (C) 2011-2024  Xiph.Org Foundation
    Copyright (C) 2024       Thomas Cort

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

/*
 * This file is based on example code from the flac repository:
 * https://github.com/xiph/flac/blob/master/examples/c/encode/file/main.c
 * Retrieved 2024-11-19 / Commit de8bbb0
 *
 * Modified for single channel, no progress, no tagging, and more.
 *
 * Upgraded from GPLv2+ to GPLv3+ on advice of licensing@fsf.org
 */

#include "encoder.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <FLAC/metadata.h>
#include <FLAC/stream_encoder.h>

#define READSIZE (1024)

static FLAC__byte buffer[READSIZE * (BPS/8) * CHANNELS];
static FLAC__int32 pcm[READSIZE * CHANNELS];

int encoder_encode(char *filepath, int16_t *result, size_t result_len, size_t total_samples) {

	FILE *fin = NULL;
	FLAC__bool ok = true;
	FLAC__StreamEncoder *encoder = 0;
	FLAC__StreamEncoderInitStatus init_status;

	fin = fmemopen(result, sizeof(int16_t) * result_len, "r");
	if (fin == NULL) {
		fprintf(stderr, "ERROR: could not read resulting audio\n");
		exit(EXIT_FAILURE);
	}

	if ((encoder = FLAC__stream_encoder_new()) == NULL) {
		fprintf(stderr, "ERROR: allocating encoder\n");
		exit(EXIT_FAILURE);
	}

        ok &= FLAC__stream_encoder_set_verify(encoder, VERIFY ? true : false);
        ok &= FLAC__stream_encoder_set_compression_level(encoder, COMPRESSION_LEVEL);
        ok &= FLAC__stream_encoder_set_channels(encoder, CHANNELS);
        ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, BPS);
        ok &= FLAC__stream_encoder_set_sample_rate(encoder, SAMPLE_RATE);
        ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);

        /* initialize encoder */
        if (ok) {
                init_status = FLAC__stream_encoder_init_file(encoder, filepath, /*progress_callback*/NULL, /*client_data=*/NULL);
                if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
                        fprintf(stderr, "ERROR: initializing encoder: %s\n", FLAC__StreamEncoderInitStatusString[init_status]);
                        ok = false;
                }
        }


        /* read blocks of samples from WAVE file and feed to encoder */
        if (ok) {
                size_t left = (size_t)total_samples;
                while(ok && left) {
                        size_t need = (left>READSIZE? (size_t)READSIZE : (size_t)left);
			size_t count;

			count = fread(buffer, CHANNELS*(BPS/8), need, fin);
                        if(count != need) {
                                fprintf(stderr, "ERROR: reading from buffer result_len=%lu read=%lu need=%lu left=%lu size_per_elem=%d\n", result_len, count, need, left, CHANNELS*(BPS/8));
                                ok = false;
                        } else {
                                /* convert the packed little-endian 16-bit PCM samples from WAVE into an interleaved FLAC__int32 buffer for libFLAC */
                                size_t i;
                                for (i = 0; i < need * CHANNELS; i++) {
					pcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2*i+1] << 8) | (FLAC__int16)buffer[2*i]);
                                }
                                /* feed samples to encoder */
                                ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, need);
                        }
                        left -= need;
                }
        }

        ok &= FLAC__stream_encoder_finish(encoder);

        FLAC__stream_encoder_delete(encoder);
        fclose(fin);

	return ok ? 0 : -1;
}
