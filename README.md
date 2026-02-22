# Toy Kernel (x86, Multiboot, C)
![Linux](/images/icons8-linux-48.png)

## Overview
This project is a minimal toy operating system kernel written in C with
a small amount of x86 assembly. It is designed to boot using GRUB via
the Multiboot specification and run inside QEMU.

The goal of this project is educational; to understand the fundamentals
of operating system development, including:
- Boot process and Multiboot
- Linking and memory layout
- Stack setup
- Freestanding C environment
- Direct hardware memory access (VGA text buffer)

It is not intended to be a production operating system.

## Architecture
Target: x86 (32-bit protected mode)
Bootloader: GRUB (Multiboot-compliant)
Language: C (freestanding) + Assembly
Emulator: QEMU

GRUB loads the kernel into memory and transfers control to the `_start`
symbol defined in `boot.s`. The assembly entry point sets up the stack
and calls `kernel_main()` in C.

The kernel writes directly to VGA memory at `0xB8000` to display text.

Here's the project structure:
```bash
boot.s
kernel.c
idt.c
idt.h
linker.ld    # linker script
iso/
    boot/
        grub/
            grub.cfg
Makefile
README.md
```

## Requirements
You must have:
- x86_64-elf-gcc
- x86_64-elf-binutils
- nasm
- qemu-system-x86_64
- GRUB tools (`grub-mkrescue`)

The kernel must be built in freestanding mode.

## Build Instructions
Build the kernel:
```bash
make
```
This will:
- compile `boot.s` and `kernel.c`
- link them using `linker.ld`
- create a GRUB-bootable ISO image

## Run
Launch in QEMU:
```bash
make run
```
or
```bash
qemu-system-x86_64 -cdrom myos.iso
```

If successful, the screen should display:
`Kernel loaded successfully.`

## What This Project Demonstrates
- How control transfers from bootloaders to the kernel
- How to define an entry point manually
- How to work without a standard library
- How linking and memory placement affect execution
- How C behaves in a freestanding environment

