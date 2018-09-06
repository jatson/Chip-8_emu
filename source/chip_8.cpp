#include "chip_8.h"

/*
 * Font set - Explanation of working by example:
 *
 * DEC   HEX    BIN         RESULT    DEC   HEX    BIN         RESULT
 * 240   0xF0   1111 0000    ****     240   0xF0   1111 0000    ****
 * 144   0x90   1001 0000    *  *      16   0x10   0001 0000       *
 * 144   0x90   1001 0000    *  *      32   0x20   0010 0000      *
 * 144   0x90   1001 0000    *  *      64   0x40   0100 0000     *
 * 240   0xF0   1111 0000    ****      64   0x40   0100 0000     *
 *
 */
const uint8_t chip_8::m_fontset[FONTSET_SIZE] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

chip_8::chip_8()
{
    init();
}

chip_8::~chip_8()
{

}

/* init registers and memory */
void chip_8::init()
{
    m_pc     = PROG_MEM_OFFSET; /* PC starts here */
    m_opcode = 0;
    m_I      = 0;
    m_sp     = 0;

    for (int i = 0; i < REGISTER_SIZE; ++i) m_register[i] = 0;
    for (int i = 0; i < MEMORY_SIZE; ++i) m_memory[i] = 0;
    for (int i = 0; i < SCREEN_SIZE; ++i) m_gfx[i] = 0;
    for (int i = 0; i < STACK_SIZE; ++i) m_stack[i] = 0;
    for (int i = 0; i < KEYPAD_SIZE; ++i) m_key[i] = 0;

    m_delayTimer = 0;
    m_soundTimer = 0;

    m_drawFlag = true;

    /* loading font set to memory */
    for (int i = 0; i < FONTSET_SIZE; ++i) m_memory[i] = m_fontset[i];
}

void chip_8::mainCycle()
{
    // fetch Opcode
    m_opcode = ((m_memory[m_pc] << 8) | m_memory[m_pc + 1]);

    // decode Opcode
    switch (m_opcode & 0xF000) {
    case 0x0000:
        switch (m_opcode & 0x000F) {
        case 0x0000:

            break;
        case 0x000E:
            break;
        default:
            // unknown opcode
            break;
        }
        break;
    case 0x1000:
        break;
    case 0x2000:
        break;
    case 0x3000:
        break;
    case 0x4000:
        break;
    case 0x5000:
        break;
    case 0x6000:
        break;
    case 0x7000:
        break;
    case 0x8000:
        switch (m_opcode & 0x000F)
        {
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
            break;
        case 0x4:
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
            break;
        case 0xE:
            break;
        default:
            // unknown opcode
            break;
        }

        break;
    case 0x9000:
        break;
    case 0xA000:
        break;
    case 0xB000:
        break;
    case 0xC000:
        break;
    case 0xD000:
        break;
    case 0xE000:
        switch (m_opcode & 0x00FF)
        {
        case 0x9E:
            break;
        case 0xA1:
            break;
        default:
            // unknown opcode
            break;
        }
    case 0xF000:
        switch (m_opcode & 0x00FF)
        {
        case 0x07:
            break;
        case 0x0A:
            break;
        case 0x15:
            break;
        case 0x18:
            break;
        case 0x1E:
            break;
        case 0x29:
            break;
        case 0x33:
            break;
        case 0x55:
            break;
        case 0x65:
            break;
        default:
            // unknown opcode
            break;
        }
    default:
        // unknown opcode
        break;
    }
    // execute Opcode
    // update timers
}

void chip_8::loadGame(const char *fileName)
{
    // Open file
    std::streampos fileSize;
    std::vector<uint8_t> fileData(fileSize);
    std::ifstream file(fileName, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    file.read((char*) &fileData[0], fileSize);

#warning "do something with the read data"
}

void chip_8::setPressedKey()
{

}

bool chip_8::drawFlag() const
{
    return m_drawFlag;
}
