#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdint.h>

class chip_8
{
private:
    uint16_t    m_opcode;       // 35 opcodes, 2 byte long, big-endian
    uint8_t     m_register[16]; // 8-bit registers from V0 to VF. VF is carry flag
    uint8_t     m_I;            // Index register. Value can be 0x000 to 0xFFF
    uint8_t     m_pc;           // Program Counter. Value can be 0x000 to 0xFFF
    uint8_t     m_delayTimer;   // 60Hz, R/W timer for game events
    uint8_t     m_soundTimer;   // 60Hz timer to sound effects. If !0, beep.

    /*
     * System memory map:
     * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
     * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
     * 0x200-0xFFF - Program ROM and work RAM
     */
    uint8_t     m_memory[4096]; // 4kB memory, interpreter at the first 512 byte

    /*
     * Graphics:
     * - 64x32 = 2048 pixels
     * - drawing done with XOR
     * - If pixel turned off, VF set to 1, else 0 (collision detection)
     * - monochrome, thus 64x32 byte array enough to store pixel state
     */
    uint8_t m_gfx[2048];  // 64x32px

public:
    chip_8();
};

#endif // CHIP_8_H
