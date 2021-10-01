# MorOS
Moros1138's Useless Kernel

## Why?

Why not?  I've played around with a few bootsector programs in the past, but never anything that involved going into 32-bit protected mode. The exercise is to see if I can learn what I need to know to make a port of my GetHome game, but as a kernel. Not for any practical reason other than fun and education.

Intially, the code and methodoloy will be heavily borrowed from/influenced by the guides and resources available for all of the internet at [The OSDev Wiki](https://wiki.osdev.org/Bare_Bones)

## DISCLAIMER!

This is a work-in-progress and is not currently in a working state.

The code or methodology contained herein should not be considered useful advice. This repository exists for the entertainment of the folks on the OLC discord and anybody else who stumbles on it.

## But I want to build it!!!

Go for it. You will need to build a cross-compiler by following the guide at [The OSDev Wiki](https://wiki.osdev.org/GCC_Cross-Compiler). I have added the toolchain to my path.

You'll also need to have gnu make, grub, xorriso, mtools, and qemu-system-i386 and their dependencies installed in order to effectively use my makefile.

## But what about building on *fill in the blank* platform that's not Linux...

Sorry, I'm a Linux daily driver and I don't have the time nor desire to go around figuring out how to do all this on an OS I don't use. However, the guide I am following does seem to have sections dedicated to doing all of this on Windows and MacOS as well. I suggest following the guide yourself to get a working build environment!
