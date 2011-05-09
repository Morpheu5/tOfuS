tOfuS
=====

A general purpose open source kernel and operating system with a special
interest for music production and multimedia applications.

It's released under GPLv3, check it out at http://www.opensource.org/licenses/gpl-3.0

Get more information at http://tofus.morpheu5.net

Installation
------------
You should only need to check that the configuration meets your testing
environment and run `make`. If all goes well, a file called `tofus` should
appear right there. You can copy it wherever you like and add it to your
multiboot-compliant bootloader. I like GNU GRUB, hence here's the relevant
entry:

    title tOfuS
    root (hdX,Y)          # read GRUB's manual, it's not that hard :-)
    kernel /path/to/tofus

If you're testing tOfuS with QEMU (or other similar tools) you may need to
reinstall GRUB every time you update the kernel image--as far as I can tell,
something is getting cached and you need to flush it.
