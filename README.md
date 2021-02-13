# README

A chip-8 emulator in C (C11)

![pong demo image](https://en.wikipedia.org/wiki/CHIP-8#/media/File:Space_intercept.png)


## Table of Contents

- [Spec](#spec)
- [TODO](#todo)
- [Requirements](#requirements)
- [Buiding](#building)
- [Keymapping](#keymapping)
- [Support](#support)
- [Contributing](#contributing)
- [License](#license)

## Spec
- 64x32 resolution display (1 bit per pixel)
- 4K RAM (`0x0 ~ 0xfff`)
  - `0x0 ~ 0x1ff`: reserved for system
  - `0x200 ~ 0xe8f`: free RAM
  - `0xea0 ~ 0xeff`: reserved for stack, internal use and other variables
  - `0xf00 ~ 0xfff`: reserved for display
- 8-bit delay and sound timer
- 16 8-bit registers `V0` ~ `VF`
- A 16-bit program counter (`PC`)
- A 16-bit stack pointer (`SP`) (only 8 used, 16 for future implementations)
- One 16-bit address register `I`
- All instructions are 2 bytes long and are stored most-significant-byte first.
- [Complete instruction set](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set)

## TODO
- Currently not all ROMs are supported.
- Sound support is incomplete.
- Better logging.

## Requirements

You'll need the lastest version of `meson` and `ninja-build` to compile.
Also the SDL2 libray (called libsdl2-dev in Debian-based systems).

## Building and running

To build using meson, once inside the project dir type:
```sh
mkdir builddir
cd builddir/
meson compile
./chipoch
```

## Keymapping

The default ROM (Pong) uses `1` and `Q` for player 1 and `R` and `4` for player 2. And `Esc` exits the game.

```text
1 2 3 C        1 2 3 4
4 5 6 D   ->   Q W E R
7 8 9 E        A S D F
A 0 B F        Z X C V
```


## Support

Please [open an issue](https://github.com/gnudiego/chipoch/issues/new) for support.

## Contributing

Please contribute using [Github Flow](https://guides.github.com/introduction/flow/). Create a branch, add commits, and [open a pull request](https://github.com/gnudiego/chipoch/compare/).

## License

    CHIP-8: A multiplatform CHIP-8 emulator done in SDL
    Copyright © 2020 Diego Marfil <diegomrepo@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

See COPYING for the entire contents of the license.
