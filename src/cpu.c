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



        case 0x0E:{

        }

        case 0x0F:{

        }


    }

}

void interpret_extended_opcode(CPU *cpu, unsigned char *mem, unsigned char opcode){

}



