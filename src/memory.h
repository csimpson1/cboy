#ifndef MEM_H_
#define MEM_H_

// Memory map boundaries
#define MEM_SIZE 0xFFFF
#define ROM_BANK_0_L 0x0000 //ROM bank 0. Contains boot program on startup, otherwise from cart
#define ROM_BANK_0_H 0x3FFF
#define ROM_BANK_N_L 0x4000 //ROM bank 1-n. From cart, switchable via memory mapper
#define ROM_BANK_N_H 0x7FFF
#define VRAM_L       0x8000 //Video RAM
#define VRAM_H       0x9FFF
#define EXT_RAM_L    0xA000 //External Ram. From cart, used for saved data. Switchable
#define EXT_RAM_H    0xBFFF
#define WRAM_0_L     0xC000 //Work RAM 0
#define WRAM_0_H     0xCFFF
#define WRAM_N_L     0xD000 //Work RAM 1-n. Switchable
#define WRAM_N_H     0xDFFF
#define ECHO_RAM_L   0xE000 //Echo RAM. Copy of C000-DDFF. Prohibited
#define ECHO_RAM_H   0xFDFF
#define OAM_L        0xFE00 //Object Attribute Memory
#define OAM_H        0xFE9F
#define NA_L         0xFEA0 //Prohibited
#define NA_H         0xFEFF
#define IO_REG_L     0xFF00 //IO registers
#define IO_REG_H     0xFF7F
#define HRAM_L       0xFF80 //High Ram. Fast
#define HRAM_H       0xFFFE

// Memory map subdivisions
#define HEADER_L     0x0100 // Cartridge header
#define HEADER_H     0x014F

// Important registers I've known and loved
// Jump vectors for RST instruction
#define RST_0        0x0000 s
#define RST_8        0x0008
#define RST_10       0x0010
#define RST_18       0x0018
#define RST_20       0x0020
#define RST_28       0x0028
#define RST_30       0x0030
#define RST_38       0x0038

// Jump vectors for interrupts
#define INT_40       0x0040 // VBlank Interrupt
#define INT_48       0x0048 // STAT Interrupt
#define INT_50       0x0050
#define INT_58       0x0058
#define INT_60       0x0060

#define LCDC         0xFF40 // LCD Control Register
#define SCY          0xFF42 // Scroll y
#define SCX          0xFF43 // Scroll x
#define LY           0xFF33 // Current horizontal line. Can hold 0 -153. 144 - 153 indicated VBlank
#define LYC          0xFF45 // LY compare. When LYC=LY, flag is set in the STAT register and a STAT interrupt is requested
#define WY           0xFF4A // Window y position
#define WX           0xFF4B // Window x position

#define BGP          0xFF47 // Background Palette Data




#define INT_EN_REG   0xFFFF //Interrupts enable registers


void init_mem(unsigned char *mem, int length);
unsigned char read_mem(unsigned char *mem, unsigned short address);
int write_mem(unsigned char *mem, unsigned short address, unsigned char val);
#endif // MEM_H_
