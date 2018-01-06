# bas-2.5-pw
2018 extended version of BAS 2.4 ANSI BASIC interpreter by Michael Haardt - moira.de (cpmtools fame)

This file is supplimentary to the BAS project, last edited: Jan 6 2018

Here **pw** means version 2.5 (and later) _with_ _extensions_ (bas-2.\*pw == bas-2.5-pw). BAS v2.5 contains only _generic_ additions, changes, bugfixes an extensions.  Language extensions like those found in MSX-BASIC, Amstrad Locomotive BASIC, BBC BASIC or Sinclair BASIC on the Spectrum, are developed here.

# binaries
All binary objects present in the repo are compiled on a RPi2 with gcc-6.1.0, on a customised (mostly console only) SD card image of PipaOS 5.0 with minimal X-Windows (for Chromium Browser on VT8).

# additions
See README. Some additions have been deemed _generic_ for modern day useful purposes, even thought they are not part of the ANSI BASIC standard, and may have been added to some variants of BASIC. They have therfore already been added to **BAS-2.5**. They are:
  * MERGE - merge program source code (LOAD without internal NEW)
  * CHAIN - continue execution, with existing variables, another program (RUN without internal NEW)

# bugs
All those found in **BAS-2.5**. At the moment, **CHAIN** keeps the internal stack, which may (?) include defined functions, and does include **gosub** branches and **for**/**next** loops, so make sure you CHAIN execution is clean, otherwise you may get unexpected results.

# future
It may chance its name, but wont change its origins, **BAS-2.5-pw** aims to create the least resistance possible when importing ANSI BASIC supersets, like MSX-BASIC, Amstrad Locomotive BASIC, BBC BASIC or Sinclair BASIC from the Spectrum.

At some point graphics commands need be added (based on the framebuffer device code I have already developed), and then sound (I have an inline AY chip core). However, at the moment most extensions are first proto-typed (as being usable) via command line scripts, some of which are then converted to programs written in C/C++, all of which can be used via the _SHELL_ statement.

Eventually, it is to be used as a shell replacement after _init_ has completed, capable of running on (or near) bare bones systems, like the Raspberry Pi. However, many considerations need to be taken into account before that happens.

It may or may not become the basis for the project https://github.com/paulwratt/ShellBASIC-shbasic

OK, that's enough for now.
