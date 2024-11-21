# text-to-morse

Converts text into a Morse code audio file.

## Description

Accepts a text file as input and outputs an audio file in FLAC format
containing the equivalent Morse code. The tone's frequency is customizable
as is the speed. There is waveform shaping to prevent clicks. The FLAC
format allows for the same lossless audio as WAV but with compression.

## Requirements

* C compiler and standard build tools (make, sh, ...).
* [cmake](https://cmake.org/)
* [libFLAC](https://github.com/xiph/flac)

## Installation

Installing in your home directory:

```
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=${HOME} ..
make
make install
```

Add the following to `${HOME}/.profile`:

```
export PATH=${HOME}/bin:${PATH}
```

## Usage

Convert the text of `hello.txt` into a FLAC audio file name `hello.flac`:

```
text-to-morse hello.txt hello.flac
```

## Audio Quality

Various combinations of bits per sample and sample rates were tried.
The best audio quality with the smallest file sizes turned out to be
a single audio channel with 16-bit samples at a sample rate of 8 kHz.

## Sample Encoding Time and Memory Usage, Audio Duration and File Size

An English text containing 46,000 characters comprising 8,400 words rendered
at 18 words per minute:

- Render Time: 0.074 seconds
- Encoding Time: 6.692 seconds
- Memory Usage: 223 MB
- Audio Duration: 7 hours 45 minutes
- Audio File Size: 63 MB

## License

SPDX-License-Identifier: GPL-3.0-or-later

See the file `COPYING` for the full license. Each file in the repository should
include its own license information.
