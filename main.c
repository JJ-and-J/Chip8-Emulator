#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "chip8/chip8.h"
#include "chip8/chip8_font.h"

uint8_t *loadrom(char *romname, uint16_t *size);
void dispprint(chip8_t *ch);

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "usage: %s [romfile.ch8]\n", argv[0]);
        return 1;
    }

    uint16_t romsz;
    uint8_t *romdata = loadrom(argv[1], &romsz);

    if(!romdata) {
        fprintf(stderr, "failed to load rom\n");
        return 1;
    }

    fprintf(stdout, "romsz: 0x%04x\n", romsz);

    chip8_t ch8;
    chip8_init(&ch8);

    chip8_loadfont(&ch8, chip8_font);
    chip8_loadrom(&ch8, romdata, romsz);

    for(int ix = 0; ix < 1000000; ++ ix) {
        chip8_instruction_tick(&ch8);
    }

    dispprint(&ch8);

    return 0;
}

uint8_t *loadrom(char *romname, uint16_t *romsz) {
    if(!romname || !romsz) return NULL;

    FILE *f = fopen(romname, "rb");
    if(!f) return NULL;

    fseek(f, 0, SEEK_END);
    *romsz = (uint16_t) ftell(f);

    uint8_t *prog = malloc(*romsz);
    if(!prog) return NULL;

    fseek(f, 0, SEEK_SET);
    fread(prog, *romsz, 1, f);

    return prog;
}

void dispprint(chip8_t *ch) {
    for(int y = 0; y < CHIP8_HEIGHT; ++ y) {
        for(int x = 0; x < CHIP8_WIDTH; ++ x) {
            fprintf(stdout, "%c", chip8_vid_read(ch, x, y) ? '@' : '_');
        }
        fprintf(stdout, "\n");
    }

    fflush(stdout);
}
