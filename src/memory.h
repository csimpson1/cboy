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

#define WAVE_RAM_l   0xFF30 // Holds waveform data for sound channel 3
#define WAVE_RAM_H   0xFF3F

// Important registers I've known and loved
// Jump vectors for RST instruction
#define RST_0       0x0000
#define RST_8       0x0008
#define RST_10      0x0010
#define RST_18      0x0018
#define RST_20      0x0020
#define RST_28      0x0028
#define RST_30      0x0030
#define RST_38      0x0038

// Jump vectors for interrupts
#define INT_40       0x0040 // VBlank Interrupt
#define INT_48       0x0048 // STAT Interrupt
#define INT_50       0x0050 // Timer Interrupt
#define INT_58       0x0058 // Serial Interrupt
#define INT_60       0x0060

/*********************
 LCD Control Registers
**********************/

#define LCDC         0xFF40 // LCD Control Register
#define STAT         0xFF41 // Status register. Gives the status of the LCD screen
#define SCY          0xFF42 // Scroll y
#define SCX          0xFF43 // Scroll x
#define LY           0xFF33 // Current horizontal line. Can hold 0 -153. 144 - 153 indicated VBlank
#define LYC          0xFF45 // LY compare. When LYC=LY, flag is set in the STAT register and a STAT interrupt is requested
#define WY           0xFF4A // Window y position
#define WX           0xFF4B // Window x position

// LCD Monochrome Palettes
#define BGP          0xFF47 // Background Palette Data
#define OBP0         0xFF48 // Object Palette 0 Data
#define OBP1         0xFF49 // Onject Palette 1 Data

// LCD Color Palettes
#define BCPS         0xFF68 // Background Color Palette Specification
#define BCPD         0xFF69 // Background Color Palette Data
#define OCPS         0xFF6A // Object Color Palette Specification
#define OCPD         0xFF69 // Object Color Palette Data

// LCD OAM DMA Transfers
#define DMA          0xFF46 // DMA Transfer and Start Address

// LCD VRAM DMA Transfers (CGB)
#define HDMA1        0xFF51 // High DMA source
#define HDMA2        0xFF52 // Low DMA source
#define HDMA3        0xFF53 // New DMA Destination High
#define HDMA4        0xFF54 // New DMA Destination Low
#define HDMA5        0xFF55 // New DMA Length/Mode/Start

// VRAM Banks (CGB)
#define VBK          0xFF4F // Changes VRAM bank on CGB

/************************
 Sound Control Registers
*************************/

// Channel 1: Tone & Sweep
#define NR10         0xFF10 // Channel 1 Sweep register
#define NR11         0xFF11 // Channel 1 Sound length/Wav Pattern duty
#define NR12         0xFF12 // Channel 1 Volume Envelope
#define NR13         0xFF13 // Channel 1 Frequency Low. Read Only
#define NR14         0xFF14 // Channel 1 Frequency High. RW

// Channel 2 - Tone
#define NR21         0xFF16 // Channel 2 Sound Length/Wave Pattern duty
#define NR22         0xFF17 // Channel 2 Volume Envelope
#define NR23         0xFF18 // Channel 2 Frequency Low. Write Only
#define NR24         0xFF19 // Channel 2 Frequency High. RW

// Channel 3 - Wave
#define NR30         0xFF1A // Channel 3 Sound On/Off
#define NR31         0xFF1B // Channel 3 Sound Length
#define NR32         0xFF1C // Channel 3 Select output level
#define NR33         0xFF1D // Channel 3 Frequency Low. Write Only
#define NR34         0xFF1E // Channel 3 Frequency High. RW

// Channel 4 - Noise
#define NR41         0xFF20 // Channel 4 Sound Length
#define NR42         0xFF21 // Channel 4 Volume Envelope
#define NR43         0xFF22 // Channel 4 Polynomial Counter
#define NR44         0xFF23 // Channel 4 Counter/Consecutive

// Sound control registers
#define NR50         0xFF24 // Channel control/ ON-OFF / Volume
#define NR51         0xFF25 // Selection of Sound output terminal
#define NR52         0xFF26 // Sound on/off. Used to save power

/*************
 Joypad Input
**************/

#define JOYP         0xFF00 // Joypad RW

/*********************
 Serial Data Transfer
**********************/

#define SB           0xFF01 // Serial Data Transfer. Holds next byte to go out before a transfer
#define SC           0xFF02 // Serial Data Control

/**************************
 Timer & Divider Registers
***************************/

#define DIV          0xFF04 // Divider Register
#define TIMA         0xFF05 // Timer counter
#define TMA          0xFF06 // Timer modulo
#define TAC          0xFF07 // Timer Control

/********************
 Interrupt Registers
*********************/
#define INT_F        0xFF0F // Interrupt Flags
#define INT_EN_REG   0xFFFF // Interrupts enable registers


void init_mem(unsigned char *mem, int length);
unsigned char read_mem(unsigned char *mem, unsigned short address);
int write_mem(unsigned char *mem, unsigned short address, unsigned char val);
#endif // MEM_H_
