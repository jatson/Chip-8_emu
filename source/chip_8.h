#ifndef CHIP_8_H
#define CHIP_8_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>

#define DEBUG

#ifdef DEBUG
#define UNUSED(expr) do { (void)(expr); } while (0) /* just a macro to avoid 'unused' compiler warn during the develop phase */
#endif

#define MEMORY_SIZE     4096
#define PROG_MEM_OFFSET 512
#define SCREEN_SIZE     2048
#define REGISTER_SIZE   16
#define STACK_SIZE      16
#define KEYPAD_SIZE     16
#define FONTSET_SIZE    (16 * 5)

class chip_8
{
private:
    uint16_t    m_opcode;                   // 35 opcodes, 2 byte long, big-endian
    uint8_t     m_register[REGISTER_SIZE];  // 8-bit registers from V0 to VF. VF is carry flag
    uint16_t    m_I;                        // Index register. Value can be 0x000 to 0xFFF
    uint16_t    m_pc;                       // Program Counter. Value can be 0x000 to 0xFFF
    uint8_t     m_delayTimer;               // 60Hz, R/W timer for game events
    uint8_t     m_soundTimer;               // 60Hz timer to sound effects. Beep sound when 0 reached.

    /*
     * System memory map:
     * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
     * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
     * 0x200-0xFFF - Program ROM and work RAM
     */
    uint8_t     m_memory[MEMORY_SIZE]; // 4kB memory, interpreter at the first 512 byte

    /*
     * Graphics:
     * - 64x32 = 2048 pixels
     * - drawing done with XOR
     * - If pixel turned off, VF set to 1, else 0 (collision detection)
     * - monochrome, thus 64x32 byte array enough to store pixel state
     */
    uint8_t m_gfx[SCREEN_SIZE];  // 64x32px

    /*
     * Stack to store current location before a jump to certain address or subrutine call.
     * Anytime when a jump required the PC shall be stored before proceeding.
     * System has 16 levels of stack and to know which stack is currently used,
     * implementation of the stack pointer is needed.
     */
    uint16_t m_stack[STACK_SIZE];
    uint16_t m_sp;

    /*
     * Hex based keypad
     * Values from 0x0 to 0xF
     */
    uint8_t m_key[KEYPAD_SIZE];

    static const uint8_t m_fontset[FONTSET_SIZE];

    bool m_drawFlag; // Flag to indicate that the screen need to be updated

    void clearScreen();
    void draw(uint16_t x, uint16_t y, uint16_t height);

public:
    chip_8();
    virtual ~chip_8();

    void init();
    void mainCycle();
    bool loadGame(const char * fileName);
    void setPressedKey();

    bool drawFlag() const;
};

#endif // CHIP_8_H
