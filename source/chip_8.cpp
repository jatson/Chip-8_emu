#include "chip_8.h"

chip_8::chip_8()
{
    srand(time(NULL));
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

void chip_8::clearScreen()
{
    for (int i = 0; i < SCREEN_SIZE; ++i) m_gfx[i] = 0;
    m_drawFlag = true;
}

void chip_8::draw(uint16_t x, uint16_t y, uint16_t height)
{
    uint16_t pixel;
    for (int yline = 0; yline < height; yline++) // height
    {
        pixel = m_memory[m_I + yline];

        for(int xline = 0; xline < 8; xline++) // width --> 8 pixel
        {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(m_gfx[(x + xline + ((y + yline) * 64))] == 1)
                {
                    m_register[0xF] = 1;
                }
                m_gfx[(x + xline + ((y + yline) * 64))] ^= 1;
            }
        }
    }
    m_drawFlag = true;
}



void chip_8::mainCycle()
{
    uint16_t x, y, height;
    bool keyPressed = false;

    // fetch Opcode
    m_opcode = ((m_memory[m_pc] << 8) | m_memory[m_pc + 1]);

    // decode Opcode
    switch (m_opcode & 0xF000)
    {
    case 0x0000:
        switch (m_opcode & 0x000F)
        {
        case 0x0: // 00E0 - clears the screen
            clearScreen();
            m_pc += 2;
            break;

        case 0xE: // 00EE - Returns from a subroutine.
            --m_sp;  // this is return.. thus stack pointer need to be decreased.
            m_pc = m_stack[m_sp];
            m_pc +=2;
            break;

        default:
            // unknown opcode
            break;
        }
        break;

    case 0x1000: // 1NNN - 	Jumps to address NNN.
        m_pc = (m_opcode & 0x0FFF);
        break;

    case 0x2000: // 2NNN - Calls subroutine at NNN.
        m_stack[m_sp] = m_pc;
        ++m_sp;

        m_pc = (m_opcode & 0x0FFF);
        break;

    case 0x3000: // 3XNN - Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
        if(m_register[(m_opcode & 0x0F00) >> 8] == (m_opcode & 0x00FF)) m_pc += 4;
        else m_pc += 2;

        break;

    case 0x4000:
        if(m_register[(m_opcode & 0x0F00) >> 8] != (m_opcode & 0x00FF)) m_pc += 4;
        else m_pc += 2;

        break;

    case 0x5000:
        if(m_register[(m_opcode & 0x0F00) >> 8] == m_register[(m_opcode & 0x00F0) >> 4]) m_pc += 4;
        else m_pc += 2;

        break;

    case 0x6000:
        m_register[(m_opcode & 0x0F00) >> 8] = (m_opcode & 0x00FF);
        m_pc += 2;

        break;

    case 0x7000:
        m_register[(m_opcode & 0x0F00) >> 8] += (m_opcode & 0x00FF);
        m_pc += 2;

        break;

    case 0x8000:
        switch (m_opcode & 0x000F)
        {
        case 0x0:
            m_register[(m_opcode & 0x0F00) >> 8] = m_register[(m_opcode & 0x00F0) >> 4];
            m_pc += 2;

            break;

        case 0x1:
            m_register[(m_opcode & 0x0F00) >> 8] = (m_register[(m_opcode & 0x0F00) >> 8] | m_register[(m_opcode & 0x00F0) >> 4]);
            m_pc += 2;

            break;

        case 0x2:
            m_register[(m_opcode & 0x0F00) >> 8] = (m_register[(m_opcode & 0x0F00) >> 8] & m_register[(m_opcode & 0x00F0) >> 4]);
            m_pc += 2;

            break;

        case 0x3:
            m_register[(m_opcode & 0x0F00) >> 8] = (m_register[(m_opcode & 0x0F00) >> 8] ^ m_register[(m_opcode & 0x00F0) >> 4]);
            m_pc += 2;

            break;

        case 0x4:
            if(m_register[(m_opcode & 0x00F0) >> 4] > (0xFF - m_register[(m_opcode & 0x0F00) >> 8])) m_register[0xF] = 1;
            else m_register[0xF] = 0;
            m_register[(m_opcode & 0x0F00) >> 8] += m_register[(m_opcode & 0x00F0) >> 4];
            m_pc += 2;

            break;

        case 0x5:
            if(m_register[(m_opcode & 0x00F0) >> 4] > m_register[(m_opcode & 0x0F00) >> 8]) m_register[0xF] = 0;
            else m_register[0xF] = 1;
            m_register[(m_opcode & 0x0F00) >> 8] -= m_register[(m_opcode & 0x00F0) >> 4];
            m_pc += 2;

            break;

        case 0x6:
            m_register[0xF] = m_register[(m_opcode & 0x0F00) >> 8] & 0x1;
            m_register[(m_opcode & 0x0F00) >> 8] >>= 1;
            m_pc += 2;

            break;

        case 0x7:
            if(m_register[(m_opcode & 0x00F0) >> 4] > m_register[(m_opcode & 0x0F00) >> 8]) m_register[0xF] = 0;
            else m_register[0xF] = 1;
            m_register[(m_opcode & 0x0F00) >> 8] = m_register[(m_opcode & 0x00F0) >> 4] - m_register[(m_opcode & 0x0F00) >> 8];
            m_pc += 2;

            break;

        case 0xE:
            m_register[0xF] = m_register[(m_opcode & 0x0F00) >> 8] >> 7;
            m_register[(m_opcode & 0x0F00) >> 8] <<= 1;
            m_pc += 2;

            break;
        default:
            // unknown opcode
            break;
        }
        break;

    case 0x9000:
        if(m_register[(m_opcode & 0x0F00) >> 8] != m_register[(m_opcode & 0x00F0) >> 4]) m_pc += 4;
        else m_pc += 2;

        break;

    case 0xA000:
        m_I = (m_opcode & 0x0FFF);
        m_pc += 2;

        break;

    case 0xB000:
        m_pc = m_register[0] + (m_opcode & 0x0FFF);

        break;

    case 0xC000:
        m_register[(m_opcode & 0x0F00) >> 8] = ((rand() % 0xFF) & (m_opcode & 0xFF));
        m_pc += 2;

        break;

    case 0xD000:
        x = m_register[(m_opcode & 0x0F00) >> 8];
        y = m_register[(m_opcode & 0x00F0) >> 4];
        height = (m_opcode & 0x000F);
        draw(x, y, height);

        break;

    case 0xE000:
        switch (m_opcode & 0x00FF)
        {
        case 0x9E:
            if(m_key[m_register[(m_opcode & 0x0F00) >> 8]] != 0) m_pc +=4;
            else m_pc += 2;

            break;

        case 0xA1:
            if(m_key[m_register[(m_opcode & 0x0F00) >> 8]] == 0) m_pc +=4;
            else m_pc += 2;

            break;

        default:
            // unknown opcode
            break;
        }
    case 0xF000:
        switch (m_opcode & 0x00FF)
        {
        case 0x07:
            m_register[(m_opcode & 0x0F00) >> 8] = m_delayTimer;
            m_pc += 2;

            break;

        case 0x0A:
            for (int i = 0; i < 0xF; ++i)
            {
                if(m_key[i] != 0)
                {
                    m_register[(m_opcode & 0x0F00) >> 8] = i;
                    keyPressed = true;
                }
            }

            if(keyPressed == false) return;

            m_pc += 2;

            break;

        case 0x15:
            m_delayTimer = m_register[(m_opcode & 0x0F00) >> 8];
            m_pc += 2;
            break;

        case 0x18:
            m_soundTimer = m_register[(m_opcode & 0x0F00) >> 8];
            m_pc += 2;
            break;

        case 0x1E:
            m_I += m_register[(m_opcode & 0x0F00) >> 8];
            m_pc += 2;

            break;

        case 0x29:
            m_I = m_register[(m_opcode & 0x0F00) >> 8] * 0x5;
            m_pc += 2;

            break;

        case 0x33:
            m_memory[m_I]     =  m_register[(m_opcode & 0x0F00) >> 8] / 100;
            m_memory[m_I + 1] = (m_register[(m_opcode & 0x0F00) >> 8] / 10) % 10;
            m_memory[m_I + 2] = (m_register[(m_opcode & 0x0F00) >> 8] % 100) % 10;

            m_pc += 2;
            break;

        case 0x55:
            for (int i = 0; i <= ((m_opcode & 0x0F00) >> 8); ++i) m_memory[m_I + i] = m_register[i];
            m_I += ((m_opcode & 0x0F00) >> 8) + 1;

            m_pc += 2;
            break;

        case 0x65:
            for (int i = 0; i <= ((m_opcode & 0x0F00) >> 8); ++i) m_register[i] = m_memory[m_I + i];
            m_I += ((m_opcode & 0x0F00) >> 8) + 1;

            m_pc += 2;
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
