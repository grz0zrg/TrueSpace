
# TrueSpace by The Orz
### 256 bytes linux intro for Lovebyte sizecoding demoparty 2021

this doesn't use any libraries (not even the standard C library) and use two syscalls (open / mmap) to output to the Linux framebuffer device (fbdev), this also doesn't use shell / compression stub so it is a 'pure' standalone 256 bytes ELF

prototyped first with my [software graphics library](https://github.com/grz0zrg/fbg)

![screenshot](truespace_400x225.png?raw=true "Screenshot")

requirements:
* 32 bits /dev/fb0 (framebuffer) with supported resolution
* high quality: the framebuffer should be set to display resolution

how to run:
* switch to console with Ctrl+Alt+F2 (Ctrl+Alt+F1 to switch back to X)
* check /dev/fb0 is writable to you (or launch with sudo to bypass this step)
* check /dev/fb0 current resolution with fbset tool
* `clear` before running the program (not mandatory)
* run the appropriate binary

permission issue: add current user to either 'tty' or 'video' group or use 'sudo'

change graphical terminal resolution:
* check supported resolution with "vbeinfo" in GRUB command prompt
* "GRUB_GFXMODE=WxH" in /etc/default/grub (where W/H is a value)
* sudo update-grub

didn't test resolutions others than 1920x1080 so may have issues.

the goal was to test doing something midly interesting in 256b in C on Linux without compression stub / shell script, it use CORDIC based algorithm to output some kind of additive background stars

must be run at night with big display for best contrast ! (it is rather meh as a screenshot and couldn't capture fb0) here is a [phone video](https://www.youtube.com/watch?v=Zr8sXXdpvts)

with compression stub + truncation this goes to 239 bytes, the ELF header is roughly taking half the size, without the header the program code is ~80 bytes

code by grz built with GCC v7.5.0

greetings to all sizecoders

12/03/2021
