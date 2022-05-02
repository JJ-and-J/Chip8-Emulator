# Chip8-Emulator
Custom (and hopefully portable) Chip8 Emulator written in C
 
# Compiling
Should compile fine with `gcc -O3 -o ch8 main.c chip8/chip8.c`
 
# Usage
Run the binary with a ROMFile as a path (`./ch8 [romfile.ch8]`). The output will be printed to `stdout`.
 
At the emulator's current (rather incomplete) stage, not many ROMs run. Try [IBM Logo.ch8](https://github.com/loktar00/chip8/blob/master/roms/IBM%20Logo.ch8) or something simple.
 
# Sources
- [wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [tobiasvl's guide (for jumping off point)](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
