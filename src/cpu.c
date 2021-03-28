#include "cpu.h"


void init_registers(CPU *cpu){
    cpu->a = 0;
    cpu->f = 0;

    cpu->b = 0;
    cpu->c = 0;

    cpu->d = 0;
    cpu->e = 0;

    cpu->h = 0;
    cpu->l = 0;

    cpu->pc = 0;
    cpu->sp = 0;
}

//Byte operations for 16b registers

unsigned char get_high_byte(unsigned short *reg){
	unsigned char hb = *reg >> 8;
	return hb;
}

unsigned char get_low_byte(unsigned short *reg){
	unsigned char lb = *reg & 0xFF; //11111111
	return lb;

}

void set_high_byte(unsigned short *reg, unsigned char val){
	unsigned short valToSet = (val << 8);
	//Mask the top 7 bytes of the reg
	*reg = *reg & 0xFF; // 0000000011111111
	*reg = *reg | valToSet;
}

void set_low_byte(unsigned short *reg, unsigned char value){
	*reg = *reg & 0xFF00; // 1111111100000000
	*reg = *reg | value;
}

//Get and set operations for 16b registers
/*
    Will use the following shorthand for registers
    1: AF
    2: BC
    3: DE
    4: HL
    5: PC
    6: SP
*/
unsigned short get_16b_register(CPU *cpu, int reg){
    unsigned short regValue;

    switch(reg){
        case 1:{
            regValue = _get_8b_to_16b(&(cpu->a), &(cpu->f));
            break;
        }
        case 2:{
            regValue = _get_8b_to_16b(&(cpu->b), &(cpu->c));
            break;
        }
        case 3:{
            regValue = _get_8b_to_16b(&(cpu->d), &(cpu->e));
            break;
        }
        case 4:{
            regValue = _get_8b_to_16b(&(cpu->h), &(cpu->l));
            break;
        }
        case 5:{
            regValue = cpu->pc;
            break;
        }
        case 6:{
            regValue = cpu->sp;
            break;
        }

    }
    return regValue;
}

void set_16b_register(CPU *cpu, int reg, unsigned short val){
    unsigned short regValue;
    switch(reg){
        case 1:{
            _set_8b_to_16b(&(cpu->a), &(cpu->f), val);
            break;
        }
        case 2:{
            _set_8b_to_16b(&(cpu->b), &(cpu->c), val);
            break;
        }
        case 3:{
            _set_8b_to_16b(&(cpu->d), &(cpu->e), val);
            break;
        }
        case 4:{
            _set_8b_to_16b(&(cpu->h), &(cpu->l), val);
            break;
        }
        case 5:{
            cpu->pc = val;
            break;
        }
        case 6:{
            cpu->sp = val;
            break;
        }
    }

}

unsigned short _get_8b_to_16b(unsigned char *highByte, unsigned char *lowByte){
    //Take two 8b numbers from the registers, and interpret them as a single 16b number
    unsigned short highByteN = (unsigned short) (*highByte);
    unsigned short lowByteN = (unsigned short) (*lowByte);

    highByteN = (highByteN << 8) & 0xFF00; //1111111100000000
    lowByteN = lowByteN & 0xFF; // 11111111

    return (highByteN | lowByteN);
}

void _set_8b_to_16b(unsigned char *highByte, unsigned char *lowByte, unsigned short val){
    //Take a single 16b number, and store it as two 8b numbers in the  registers
    unsigned char highByteVal = ((unsigned char) (val >> 8) & 0xFF); //11111111
    unsigned char lowByteVal = ((unsigned char) val & 0xFF);
    *highByte = highByteVal;
    *lowByte = lowByteVal;

}



unsigned char swap_nibble(unsigned char *toSwap){
    unsigned char highNib = (*toSwap & 0xF0) >> 4;
    unsigned char lowNib = (*toSwap & 0x0F) <<4;

     return (lowNib | highNib);
}

unsigned char get_byte(CPU *cpu, unsigned char *mem){
    unsigned char byte = read_mem(mem, cpu->pc++);
    return byte;
}

void increment_timer(unsigned char *mem, unsigned char cycles){
    //Increment the timer register by the number of cycles taken by the operation
    write_mem(mem, TIMA, read_mem(mem, TIMA) + cycles);
}

void flags_timer_post_inc(CPU *cpu, unsigned char *mem, unsigned char oldVal, unsigned char newVal){
    /*
    Handles the setting of flags after an increment instruction. Old val is the register value before incrementing
    */
    SET_ZF(cpu, (newVal == 0));
    SET_NF(cpu, 0);
    SET_HF(cpu, HC_CHECK(1, oldVal));

    increment_timer(mem, 4);
}

void flags_timer_post_dec(CPU *cpu, char *mem, unsigned char oldVal, unsigned char newVal){
    //Right now this is the same as flags post inc. Need to test this once the emu is in a better state
    SET_ZF(cpu, (newVal == 0));
    SET_NF(cpu, 0);
    // check how this works with a test rom.
    SET_HF(cpu, HC_CHECK(1, oldVal));

    increment_timer(mem, 4);
}

void interpret_opcode(CPU *cpu, unsigned char *mem, unsigned char opcode){
    switch(opcode){
        case 0x00:{ //NOP
            increment_timer(mem, 4);
            break;
        }

        case 0x01:{ //LD BC d16
            unsigned char low = get_byte(cpu, mem);
            unsigned char high = get_byte(cpu, mem);
            cpu->c = low;
            cpu->b = high;

            increment_timer(mem, 12);
            break;

        }

        case 0x02:{ //LD (BC) A
            unsigned char addr = get_16b_register(cpu, 2); //Get the value of BC
            write_mem(mem, addr, cpu->a);

            increment_timer(mem,8);
            break;

        }

        case 0x03:{ //INC BC
            unsigned short val = get_16b_register(cpu, 2);
            val ++;
            set_16b_register(cpu, 2, val);

            increment_timer(mem, 8);
            break;

        }


        case 0x06:{ //LD B, d8
            unsigned char data = get_byte(cpu, mem);
            cpu->b = data;

            increment_timer(mem, 8);
        }

        case 0x07:{ //RCLA

            increment_timer(mem, 4);
        }

        case 0x08:{ //LD (a16), SP
            //Low byte comes first, high byte afterwards. Shift the high byte up by 8 bits, and make sure everything
            //matches typewise
            unsigned short address = (((short) get_byte(cpu, mem)) | (short) get_byte(cpu,mem)) << 8 ;
            unsigned char lowByte = (unsigned char) (cpu->sp) & 0xff;
            unsigned char highByte = (unsigned char) ((cpu->sp) & 0xff00) >> 8;

            write_mem(mem, address, lowByte);
            write_mem(mem, address + 1, highByte);

            increment_timer(mem, 20);
        }

        case 0x09:{ //ADD HL, BC
            unsigned short hl = GET_HL(cpu);
            unsigned short bc = GET_BC(cpu);

            unsigned short newHl = hl + bc;
            SET_HL(cpu, newHl);

            SET_ZF(cpu, (newHl == 0));
            SET_HF(cpu, HC_CHECK(hl, bc));
            SET_CF(cpu, C_CHECK(hl, bc));

            increment_timer(mem, 8);
        }

        case 0x0A:{ //LD A, (BC)
            unsigned char data = read_mem(mem, GET_BC(cpu));
            cpu->a = data;

            increment_timer(mem, 8);
        }

        case 0x0B:{ //DEC BC
            unsigned short newBC = GET_BC(cpu) - 1;
            SET_BC(cpu, newBC);

            increment_timer(mem, 8);

        }

/*************************
 8 Bit Register Increments
**************************/

        case 0x04:{ //INC B
            unsigned char oldVal= cpu->b;
            unsigned char newVal= oldVal + 1;
            cpu->b = newVal;

            flags_timer_post_inc(cpu, mem, oldVal, newVal);

        }

        case 0x0C:{ //INC C
            unsigned char oldVal = cpu->c;
            unsigned char newVal = oldVal + 1;
            cpu->c = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);

        }

        case 0x14:{ //INC D
            unsigned char oldVal = cpu->d;
            unsigned char newVal = oldVal + 1;
            cpu->d = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);

        }

        case 0x1C:{ //INC E
            unsigned char oldVal = cpu->e;
            unsigned char newVal = oldVal + 1;
            cpu->e = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);
        }

        case 0x24:{ //INC H
            unsigned char oldVal = cpu->h;
            unsigned char newVal = oldVal + 1;
            cpu->h = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);
        }

        case 0x2C:{ //INC L
            unsigned char oldVal = cpu->l;
            unsigned char newVal = oldVal + 1;
            cpu->l = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);
        }

        case 0x34:{ //INC (HL)
            unsigned short address = GET_HL(cpu);
            unsigned char oldVal = read_mem(mem, address);
            unsigned char newVal = oldVal + 1;
            write_mem(mem, address, newVal);

            flags_timer_post_inc(cpu,mem, oldVal, newVal);
            //This op takes longer than other increments as memory needs to be accessed
            //Normal inc takes 4, this ones takes 12, so make up the difference
            increment_timer(mem, 8);
        }

        case 0x3C:{ //INC A
            unsigned char oldVal = cpu->a;
            unsigned char newVal = oldVal + 1;
            cpu->a = newVal;

            flags_timer_post_inc(cpu,mem, oldVal, newVal);
        }

/*************************
 8 Bit Register Decrements
**************************/
//TODO: Test out how the half cary flag/ carry flags are set

        case 0x05:{ //DEC B
            unsigned char oldVal= cpu->b;
            // hmm
            unsigned char newVal= oldVal - 1;
            cpu->b = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);

        }

        case 0x0D:{ //DEC C
            unsigned char oldVal= cpu->c;
            unsigned char newVal= oldVal - 1;
            cpu->c = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

        case 0x15:{ //DEC D
            unsigned char oldVal= cpu->d;
            unsigned char newVal= oldVal - 1;
            cpu->d = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

        case 0x1D:{ //DEC E
            unsigned char oldVal= cpu->e;
            unsigned char newVal= oldVal - 1;
            cpu->e = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

        case 0x25:{ //DEC H
            unsigned char oldVal= cpu->h;
            unsigned char newVal= oldVal - 1;
            cpu->h = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

        case 0x2D:{ //DEC L
            unsigned char oldVal= cpu->l;
            unsigned char newVal= oldVal - 1;
            cpu->l = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

        case 0x35:{ //DEC (HL)
            unsigned short address = GET_HL(cpu);
            unsigned char oldVal= read_mem(mem, address);
            unsigned char newVal= oldVal - 1;
            write_mem(mem, address, newVal);

            flags_timer_post_dec(cpu, mem, oldVal, newVal);
            //Becase we access memory, decerement is slower.
        }

        case 0x3D:{ //DEC A
            unsigned char oldVal= cpu->a;
            unsigned char newVal= oldVal - 1;
            cpu->a = newVal;
            flags_timer_post_dec(cpu, mem, oldVal, newVal);
        }

       /************************
         8 bit Load Instructions
        *************************/
		case 0x02:{ // LD BC, A
			unsigned char src = cpu->a;
			write_mem(cpu, GET_BC(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x06:{ // LD B, d8
			unsigned char src = get_byte(cpu);
			cpu->b = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x0A:{ // LD A, BC
			unsigned char src = read_mem(mem, GET_BC(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x0E:{ // LD C, d8
			unsigned char src = get_byte(cpu);
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x12:{ // LD DE, A
			unsigned char src = cpu->a;
			write_mem(cpu, GET_DE(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x16:{ // LD D, d8
			unsigned char src = get_byte(cpu);
			cpu->d = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x1A:{ // LD A, DE
			unsigned char src = read_mem(mem, GET_DE(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x1E:{ // LD E, d8
			unsigned char src = get_byte(cpu);
			cpu->e = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x22:{ // LD HL, A
			unsigned char src = cpu->a;
			write_mem(cpu, GET_HL(cpu), src);
			SET_HL(cpu, GET_HL(cpu) + 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x26:{ // LD H, d8
			unsigned char src = get_byte(cpu);
			cpu->h = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x2A:{ // LD A, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			SET_HL(cpu,src + 1);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x2E:{ // LD L, d8
			unsigned char src = get_byte(cpu);
			cpu->l = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x32:{ // LD HL, A
			unsigned char src = cpu->a;
			write_mem(cpu, GET_HL(cpu), src);
			SET_HL(cpu, GET_HL(cpu) - 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x36:{ // LD HL, d8
			unsigned char src = get_byte(cpu);
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 12);
			break;
		}

		case 0x3A:{ // LD A, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			SET_HL(cpu,src - 1);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x3E:{ // LD A, d8
			unsigned char src = get_byte(cpu);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x40:{ // LD B, B
			unsigned char src = cpu->b;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x41:{ // LD B, C
			unsigned char src = cpu->c;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x42:{ // LD B, D
			unsigned char src = cpu->d;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x43:{ // LD B, E
			unsigned char src = cpu->e;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x44:{ // LD B, H
			unsigned char src = cpu->h;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x45:{ // LD B, L
			unsigned char src = cpu->l;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x46:{ // LD B, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->b = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x47:{ // LD B, A
			unsigned char src = cpu->a;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x48:{ // LD C, B
			unsigned char src = cpu->b;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x49:{ // LD C, C
			unsigned char src = cpu->c;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4A:{ // LD C, D
			unsigned char src = cpu->d;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4B:{ // LD C, E
			unsigned char src = cpu->e;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4C:{ // LD C, H
			unsigned char src = cpu->h;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4D:{ // LD C, L
			unsigned char src = cpu->l;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4E:{ // LD C, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x4F:{ // LD C, A
			unsigned char src = cpu->a;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x50:{ // LD D, B
			unsigned char src = cpu->b;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x51:{ // LD D, C
			unsigned char src = cpu->c;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x52:{ // LD D, D
			unsigned char src = cpu->d;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x53:{ // LD D, E
			unsigned char src = cpu->e;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x54:{ // LD D, H
			unsigned char src = cpu->h;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x55:{ // LD D, L
			unsigned char src = cpu->l;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x56:{ // LD D, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->d = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x57:{ // LD D, A
			unsigned char src = cpu->a;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x58:{ // LD E, B
			unsigned char src = cpu->b;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x59:{ // LD E, C
			unsigned char src = cpu->c;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5A:{ // LD E, D
			unsigned char src = cpu->d;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5B:{ // LD E, E
			unsigned char src = cpu->e;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5C:{ // LD E, H
			unsigned char src = cpu->h;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5D:{ // LD E, L
			unsigned char src = cpu->l;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5E:{ // LD E, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->e = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x5F:{ // LD E, A
			unsigned char src = cpu->a;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x60:{ // LD H, B
			unsigned char src = cpu->b;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x61:{ // LD H, C
			unsigned char src = cpu->c;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x62:{ // LD H, D
			unsigned char src = cpu->d;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x63:{ // LD H, E
			unsigned char src = cpu->e;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x64:{ // LD H, H
			unsigned char src = cpu->h;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x65:{ // LD H, L
			unsigned char src = cpu->l;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x66:{ // LD H, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->h = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x67:{ // LD H, A
			unsigned char src = cpu->a;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x68:{ // LD L, B
			unsigned char src = cpu->b;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x69:{ // LD L, C
			unsigned char src = cpu->c;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6A:{ // LD L, D
			unsigned char src = cpu->d;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6B:{ // LD L, E
			unsigned char src = cpu->e;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6C:{ // LD L, H
			unsigned char src = cpu->h;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6D:{ // LD L, L
			unsigned char src = cpu->l;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6E:{ // LD L, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->l = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x6F:{ // LD L, A
			unsigned char src = cpu->a;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x70:{ // LD HL, B
			unsigned char src = cpu->b;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x71:{ // LD HL, C
			unsigned char src = cpu->c;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x72:{ // LD HL, D
			unsigned char src = cpu->d;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x73:{ // LD HL, E
			unsigned char src = cpu->e;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x74:{ // LD HL, H
			unsigned char src = cpu->h;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x75:{ // LD HL, L
			unsigned char src = cpu->l;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x77:{ // LD HL, A
			unsigned char src = cpu->a;
			write_mem(cpu, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x78:{ // LD A, B
			unsigned char src = cpu->b;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x79:{ // LD A, C
			unsigned char src = cpu->c;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7A:{ // LD A, D
			unsigned char src = cpu->d;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7B:{ // LD A, E
			unsigned char src = cpu->e;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7C:{ // LD A, H
			unsigned char src = cpu->h;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7D:{ // LD A, L
			unsigned char src = cpu->l;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7E:{ // LD A, HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x7F:{ // LD A, A
			unsigned char src = cpu->a;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0xE2:{ // LD C, A
			unsigned char src = cpu->a;
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0xF2:{ // LD A, C
			unsigned char src = cpu->c;
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}



    }

}

void interpret_extended_opcode(CPU *cpu, unsigned char *mem, unsigned char opcode){

}



