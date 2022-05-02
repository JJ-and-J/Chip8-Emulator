#include "chip8.h"

// initialize chip8 emulator
int chip8_init(chip8_t *ch8) {
    if(!ch8) {
        return 1;
    }

    ch8->pc = 0;

    chip8_vid_clear(ch8);

    memset(ch8->mem, 0, sizeof(ch8->mem));

    memset(ch8->stack, 0, sizeof(ch8->stack));
    memset(ch8->reg_v, 0, sizeof(ch8->reg_v));

    return 0;
}

// load rom
int chip8_loadrom(chip8_t *ch8, uint8_t *buf, uint16_t buf_sz) {
    if(!ch8 || !buf) {
        return 1;
    }

    memcpy(&ch8->mem[0x200], buf, buf_sz);

    return 0;
}

int chip8_loadfont(chip8_t *ch8, uint8_t *buf) {
    if(!ch8 || !buf) {
        return 1;
    }

    memcpy(&ch8->mem[0x50], buf, CHIP8_FONTSIZE);

    return 0;
}

// manage stack
int chip8_stack_push(chip8_t *ch8, uint16_t data) {
    if(!ch8 || ch8->stack_len >= sizeof(ch8->stack_len)) {
        return 1;
    }

    ch8->stack[ch8->stack_len] = data;
    ++ ch8->stack_len;

    return 0;
}

int chip8_stack_pop(chip8_t *ch8, uint16_t data) {
    if(!ch8 || ch8->stack_len <= 0) {
        return -1;
    }

    -- ch8->stack_len;

    return ch8->stack[ch8->stack_len];
}

// update timers
int chip8_timer_tick(chip8_t *ch8) {
    if(!ch8) {
        return 1;
    }

    if(ch8->tmr_delay) {
        -- ch8->tmr_delay;
    }

    if(ch8->tmr_sound) {
        -- ch8->tmr_sound;
    }

    return 0;
}

// manage video memory
int chip8_vid_clear(chip8_t *ch) {
    memset(ch->vid, 0, sizeof(ch->vid));

    return 0;
}

int chip8_vid_read(chip8_t *ch8, uint8_t px, uint8_t py) {
    if(!ch8) {
        return -1;
    }

    uint16_t index, shift;

    if(chip8_vid_pxix(px, py, &index, &shift)) {
        return -1;
    }

    return (ch8->vid[index] >> shift) & 1;
}

int chip8_vid_flip(chip8_t *ch8, uint8_t px, uint8_t py) {
    if(!ch8) {
        return 1;
    }

    uint16_t index, shift;

    if(chip8_vid_pxix(px, py, &index, &shift)) {
        return 1;
    }

    ch8->vid[index] ^= 1 << shift;

    return 0;
}

int chip8_vid_draw(chip8_t *ch8, uint16_t vx, uint16_t vy, uint16_t n) {
    if(!ch8) {
        return 1;
    }

    uint8_t px = ch8->reg_v[vx];
    uint8_t py = ch8->reg_v[vy];

    uint16_t i = ch8->reg_i;

    ch8->reg_v[0xf] = 0;

    for(uint8_t y = 0; y < n; ++ y) {
        uint8_t row = ch8->mem[i + y];

        for(uint8_t x = 0; row; ++ x) {
            if(row & 1) {
                chip8_vid_flip(ch8, px + 8 - x, py + y);
                ch8->reg_v[0xf] = 1;
            }

            row >>= 1;
        }
    }

    return 0;
}

int chip8_vid_pxix(uint8_t px, uint8_t py, uint16_t *index, uint16_t *shift) {
    if(px > CHIP8_WIDTH || py > CHIP8_HEIGHT) {
        return 1;
    }

    if(index) {
        *index = (px + py * CHIP8_WIDTH) / 8;
    }

    if(shift) {
        *shift = 7 - (px % 8);
    }

    return 0;
}

// parse instruction
int chip8_instruction_read(chip8_t *ch, uint8_t *na, uint8_t *nx, uint8_t *ny, uint8_t *nn) {
    if(!ch) {
        return 1;
    }

    uint8_t b0 = ch->mem[ch->pc    ];
    uint8_t b1 = ch->mem[ch->pc + 1];

    if(na) {
        *na = (b0 >> 4) & 0xf;
    }

    if(nx) {
        *nx = (b0     ) & 0xf;
    }

    if(ny) {
        *ny = (b1 >> 4) & 0xf;
    }

    if(nn) {
        *nn = (b1     ) & 0xf;
    }

    return 0;
}

int chip8_instruction_tick(chip8_t *ch) {
    if(!ch) {
        return 1;
    }

    uint8_t na, nx, ny, nn;
    chip8_instruction_read(ch, &na, &nx, &ny, &nn);

    // currently rather incomplete
    switch(na) {
        case 0x0: {
            // TODO:
            chip8_vid_clear(ch);
            break;
        }

        case 0x1: {
            // TODO:
            ch->pc = ((((nx << 4) | ny) << 4) | nn) - 2;
            break;
        }

        case 0x6: {
            // TODO:
            ch->reg_v[nx] = (ny << 4) | nn;
            break;
        }

        case 0x7: {
            // TODO:
            ch->reg_v[nx] += (ny << 4) | nn;
            break;
        }

        case 0xa: {
            // TODO:
            ch->reg_i = (((nx << 4) | ny) << 4) | nn;
            break;
        }

        case 0xd: {
            // TODO:
            chip8_vid_draw(ch, nx, ny, nn);
            break;
        }
    }

    ch->pc += 2;

    return 0;
}
