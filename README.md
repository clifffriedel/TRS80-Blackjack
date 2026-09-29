# TRS80-Blackjack

This is a blackjack game that I've written in C for the Tandy TRS-80 Model III.

## Installation

To install, simply clone this repo then run the CMD file in your TRS-80 Model III Emulator.  I use the following command to run it on TRS80gp in Linux:

`trs80gp -m3 -ld blackjack.cmd`

This should open a Model III emulation with 48K that runs LDOS and the program automatically.

## Compiling

I've written this program using the z88dk z80 cross compiler.  This is the line I use to compile the program and create the CMD file:

`zcc +trs80 -lndos -lm -create-app -subtype=disk blackjack.c -o blackjack`

This targets the TRS-80 series of computers and builds the command file needed to run the program in LDOS.  You will need to change the options to run it in TRSDOS.  

