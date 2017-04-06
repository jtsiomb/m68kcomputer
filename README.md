Homebrew 68k computer project 1
===============================

![photo](http://nuclear.mutantstargoat.com/hw/m68kcomp1/img/m68kcomp1_test2-thumb.jpg)

About
-----
The purpose of this project is to design a minimal but useful homebrew computer,
based on the Motorola 68010 16/32-bit processor.

The current hardware design is broken into a main processor board, and an I/O
and expansion board with an AVR microcontroller playing the role of UART, timer,
and RTC all in one.

Directory structure:

  * `hw` - hardware: kicad files and pdf schematics.
  * `fw_iolite` - firmware for the I/O controller.
  * `monitor` - ROM monitor program.

License
-------
Copyright (C) 2017 John Tsiombikas <nuclear@member.fsf.org>

You are free to use, reproduce, modify, redistribute any parts of this project,
provided you make any derivative work you release, freely available under the
same terms.

More specifically the hardware parts of this project are released under the
Creative Commons Attribution Share-Alike license (see `LICENSE.hw`
for details), and the software/firmware parts are released under the GNU General
Public License v3, or later (see `LICENSE.sw` for details).
