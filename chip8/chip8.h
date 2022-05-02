#ifndef CHIP8_H_
#define CHIP8_H_

#include <stdint.h>
#include <stdlib.h>

#include <string.h>

#define CHIP8_MEMSIZE 4096

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

#define CHIP8_FONTSIZE 0x50

// chip8_t structure
typedef struct {
    uint8_t mem[CHIP8_MEMSIZE];
    uint8_t vid[CHIP8_WIDTH * CHIP8_HEIGHT / 8];

    uint16_t pc;

    uint16_t reg_i;

    uint16_t stack[12];
    uint8_t stack_len;

    uint8_t tmr_delay;
    uint8_t tmr_sound;

    uint8_t reg_v[16];
} chip8_t;

// initialize chip8 emulator
int chip8_init(chip8_t *ch8);

// load rom
int chip8_loadrom(chip8_t *ch8, uint8_t *buf, uint16_t buf_sz);
int chip8_loadfont(chip8_t *ch8, uint8_t *buf);

// manage stack
int chip8_stack_push(chip8_t *ch8, uint16_t data);
int chip8_stack_pop(chip8_t *ch8, uint16_t data);

// update timers
int chip8_timer_tick(chip8_t *ch8);

// manage video memory
int chip8_vid_clear(chip8_t *ch);

int chip8_vid_read(chip8_t *ch8, uint8_t px, uint8_t py);
int chip8_vid_flip(chip8_t *ch8, uint8_t px, uint8_t py);

int chip8_vid_draw(chip8_t *ch8, uint16_t vx, uint16_t vy, uint16_t n);

int chip8_vid_pxix(uint8_t px, uint8_t py, uint16_t *index, uint16_t *shift);

// parse instruction
int chip8_instruction_read(chip8_t *ch, uint8_t *na, uint8_t *nx, uint8_t *ny, uint8_t *nn);
int chip8_instruction_tick(chip8_t *ch);

#endif