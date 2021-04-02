void interpret_opcodes(CPU *cpu, unsigned char *mem, unsigned char opcode){
	switch(opcode){
		/***********************
		  LD: Load operations
		***********************/
		case 0x01:{ //LD BC d16
			unsigned char srcLowByte =  get_byte(cpu, mem);
			unsigned char srcHighByte = get_byte(cpu, mem);
			unsigned short src = _get_8b_to_16b(&srcHighByte, &srcLowByte);
			SET_BC(cpu, src);
			increment_timer(mem, 12);
			break;
		}

		case 0x02:{ //LD BC A
			unsigned char src =  cpu->a;
			write_mem(mem, GET_BC(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x06:{ //LD B d8
			unsigned char src = get_byte(cpu, mem);
			cpu->b = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x08:{ //LD a16 SP
			unsigned char spLowByte = get_low_byte &(cpu -> sp);
			unsigned char spHighByte = get_high_byte(&(cpu -> sp);
			unsigned char addrLow = get_byte(cpu, mem);
			unsigned char addrHigh = get_byte(cpu, mem);
			unsigned short addr = _get_8b_to_16b(&addrLow, &addrHigh);
			writemem(mem, addr, spLowByte);
			writemem(mem, addr + 0x1, spHighByte);
			increment_timer(mem, 20);
			break;
		}

		case 0x0A:{ //LD A BC
			unsigned char src =  read_mem(mem, GET_BC(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x0E:{ //LD C d8
			unsigned char src = get_byte(cpu, mem);
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x11:{ //LD DE d16
			unsigned char srcLowByte =  get_byte(cpu, mem);
			unsigned char srcHighByte = get_byte(cpu, mem);
			unsigned short src = _get_8b_to_16b(&srcHighByte, &srcLowByte);
			SET_DE(cpu, src);
			increment_timer(mem, 12);
			break;
		}

		case 0x12:{ //LD DE A
			unsigned char src =  cpu->a;
			write_mem(mem, GET_DE(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x16:{ //LD D d8
			unsigned char src = get_byte(cpu, mem);
			cpu->d = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x1A:{ //LD A DE
			unsigned char src =  read_mem(mem, GET_DE(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x1E:{ //LD E d8
			unsigned char src = get_byte(cpu, mem);
			cpu->e = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x21:{ //LD HL d16
			unsigned char srcLowByte =  get_byte(cpu, mem);
			unsigned char srcHighByte = get_byte(cpu, mem);
			unsigned short src = _get_8b_to_16b(&srcHighByte, &srcLowByte);
			SET_HL(cpu, src);
			increment_timer(mem, 12);
			break;
		}

		case 0x22:{ //LD HL A
			unsigned char src =  cpu->a;
			write_mem(mem, GET_HL(cpu), src);
			SET_HL(cpu, GET_HL(cpu) + 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x26:{ //LD H d8
			unsigned char src = get_byte(cpu, mem);
			cpu->h = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x2A:{ //LD A HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			SET_HL(cpu,src + 1);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x2E:{ //LD L d8
			unsigned char src = get_byte(cpu, mem);
			cpu->l = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x31:{ //LD SP d16
			unsigned char srcLowByte =  get_byte(cpu, mem);
			unsigned char srcHighByte = get_byte(cpu, mem);
			unsigned short src = _get_8b_to_16b(&srcHighByte, &srcLowByte);
			SET_SP(cpu, src);
			increment_timer(mem, 12);
			break;
		}

		case 0x32:{ //LD HL A
			unsigned char src =  cpu->a;
			write_mem(mem, GET_HL(cpu), src);
			SET_HL(cpu, GET_HL(cpu) - 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x36:{ //LD HL d8
			unsigned char src = get_byte(cpu, mem);
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 12);
			break;
		}

		case 0x3A:{ //LD A HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			SET_HL(cpu,src - 1);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x3E:{ //LD A d8
			unsigned char src = get_byte(cpu, mem);
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x40:{ //LD B B
			unsigned char src =  cpu->b;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x41:{ //LD B C
			unsigned char src =  cpu->c;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x42:{ //LD B D
			unsigned char src =  cpu->d;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x43:{ //LD B E
			unsigned char src =  cpu->e;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x44:{ //LD B H
			unsigned char src =  cpu->h;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x45:{ //LD B L
			unsigned char src =  cpu->l;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x46:{ //LD B HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->b = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x47:{ //LD B A
			unsigned char src =  cpu->a;
			cpu->b = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x48:{ //LD C B
			unsigned char src =  cpu->b;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x49:{ //LD C C
			unsigned char src =  cpu->c;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4A:{ //LD C D
			unsigned char src =  cpu->d;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4B:{ //LD C E
			unsigned char src =  cpu->e;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4C:{ //LD C H
			unsigned char src =  cpu->h;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4D:{ //LD C L
			unsigned char src =  cpu->l;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x4E:{ //LD C HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x4F:{ //LD C A
			unsigned char src =  cpu->a;
			cpu->c = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x50:{ //LD D B
			unsigned char src =  cpu->b;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x51:{ //LD D C
			unsigned char src =  cpu->c;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x52:{ //LD D D
			unsigned char src =  cpu->d;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x53:{ //LD D E
			unsigned char src =  cpu->e;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x54:{ //LD D H
			unsigned char src =  cpu->h;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x55:{ //LD D L
			unsigned char src =  cpu->l;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x56:{ //LD D HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->d = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x57:{ //LD D A
			unsigned char src =  cpu->a;
			cpu->d = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x58:{ //LD E B
			unsigned char src =  cpu->b;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x59:{ //LD E C
			unsigned char src =  cpu->c;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5A:{ //LD E D
			unsigned char src =  cpu->d;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5B:{ //LD E E
			unsigned char src =  cpu->e;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5C:{ //LD E H
			unsigned char src =  cpu->h;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5D:{ //LD E L
			unsigned char src =  cpu->l;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x5E:{ //LD E HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->e = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x5F:{ //LD E A
			unsigned char src =  cpu->a;
			cpu->e = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x60:{ //LD H B
			unsigned char src =  cpu->b;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x61:{ //LD H C
			unsigned char src =  cpu->c;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x62:{ //LD H D
			unsigned char src =  cpu->d;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x63:{ //LD H E
			unsigned char src =  cpu->e;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x64:{ //LD H H
			unsigned char src =  cpu->h;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x65:{ //LD H L
			unsigned char src =  cpu->l;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x66:{ //LD H HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->h = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x67:{ //LD H A
			unsigned char src =  cpu->a;
			cpu->h = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x68:{ //LD L B
			unsigned char src =  cpu->b;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x69:{ //LD L C
			unsigned char src =  cpu->c;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6A:{ //LD L D
			unsigned char src =  cpu->d;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6B:{ //LD L E
			unsigned char src =  cpu->e;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6C:{ //LD L H
			unsigned char src =  cpu->h;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6D:{ //LD L L
			unsigned char src =  cpu->l;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x6E:{ //LD L HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->l = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x6F:{ //LD L A
			unsigned char src =  cpu->a;
			cpu->l = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x70:{ //LD HL B
			unsigned char src =  cpu->b;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x71:{ //LD HL C
			unsigned char src =  cpu->c;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x72:{ //LD HL D
			unsigned char src =  cpu->d;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x73:{ //LD HL E
			unsigned char src =  cpu->e;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x74:{ //LD HL H
			unsigned char src =  cpu->h;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x75:{ //LD HL L
			unsigned char src =  cpu->l;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x77:{ //LD HL A
			unsigned char src =  cpu->a;
			write_mem(mem, GET_HL(cpu), src);
			increment_timer(mem, 8);
			break;
		}

		case 0x78:{ //LD A B
			unsigned char src =  cpu->b;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x79:{ //LD A C
			unsigned char src =  cpu->c;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7A:{ //LD A D
			unsigned char src =  cpu->d;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7B:{ //LD A E
			unsigned char src =  cpu->e;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7C:{ //LD A H
			unsigned char src =  cpu->h;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7D:{ //LD A L
			unsigned char src =  cpu->l;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0x7E:{ //LD A HL
			unsigned char src =  read_mem(mem, GET_HL(cpu));
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0x7F:{ //LD A A
			unsigned char src =  cpu->a;
			cpu->a = src;
			increment_timer(mem, 4);
			break;
		}

		case 0xE2:{ //LD C A
			unsigned char src =  cpu->a;
			cpu->c = src;
			increment_timer(mem, 8);
			break;
		}

		case 0xEA:{ //LD a16 A
			unsigned char src = cpu -> a;
			unsigned char addrLow = get_byte(cpu, mem);
			unsigned char addrHigh = get_byte(cpu, mem);
			unsigned short addr = _get_8b_to_16b(&addrLow, &addrHigh);
			writemem(mem, addr, src;
			increment_timer(mem, 16);
			break;
		}

		case 0xF2:{ //LD A C
			unsigned char src =  cpu->c;
			cpu->a = src;
			increment_timer(mem, 8);
			break;
		}

		case 0xFA:{ //LD A a16
			unsigned char srcLowByte =  get_byte(cpu, mem);
			unsigned char srcHighByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&srcHighByte, &srcLowByte);
			unsigned short src = read_mem(mem, address);
			cpu -> a = src;
			increment_timer(mem, 16);
			break;
		}

		/****************************************
		  INC: 8 / 16 bit increment operations
		****************************************/
		case 0x03:{ //INC BC None
			unsigned short toInc = GET_BC(cpu);
			unsigned short incremented = toInc + 1;
			SET_BC(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x04:{ //INC B None
			unsigned char toInc = cpu -> b;
			unsigned char incremented = toInc + 1;
			cpu -> b;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x0C:{ //INC C None
			unsigned char toInc = cpu -> c;
			unsigned char incremented = toInc + 1;
			cpu -> c;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x13:{ //INC DE None
			unsigned short toInc = GET_DE(cpu);
			unsigned short incremented = toInc + 1;
			SET_DE(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x14:{ //INC D None
			unsigned char toInc = cpu -> d;
			unsigned char incremented = toInc + 1;
			cpu -> d;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x1C:{ //INC E None
			unsigned char toInc = cpu -> e;
			unsigned char incremented = toInc + 1;
			cpu -> e;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x23:{ //INC HL None
			unsigned short toInc = GET_HL(cpu);
			unsigned short incremented = toInc + 1;
			SET_HL(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x24:{ //INC H None
			unsigned char toInc = cpu -> h;
			unsigned char incremented = toInc + 1;
			cpu -> h;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x2C:{ //INC L None
			unsigned char toInc = cpu -> l;
			unsigned char incremented = toInc + 1;
			cpu -> l;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x33:{ //INC SP None
			unsigned short toInc = GET_SP(cpu);
			unsigned short incremented = toInc + 1;
			SET_SP(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x34:{ //INC HL None
			unsigned char toInc = read_mem(mem, GET_HL(cpu));
			unsigned char incremented = toInc + 1;
			write_mem(mem, GET_HL(cpu), incremented);
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 12);
			break;
		}

		case 0x3C:{ //INC A None
			unsigned char toInc = cpu -> a;
			unsigned char incremented = toInc + 1;
			cpu -> a;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

	}
}
