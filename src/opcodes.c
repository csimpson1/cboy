void interpret_opcodes(CPU *cpu, unsigned char *mem, unsigned char opcode){
	switch(opcode){
		/****************************************
		  INC: 8 / 16 bit increment operations
		****************************************/
		case 0x03:{ //INC BC 
			unsigned short toInc = GET_BC(cpu);
			unsigned short incremented = toInc + 1;
			SET_BC(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x04:{ //INC B 
			unsigned char toInc = cpu -> b;
			unsigned char incremented = toInc + 1;
			cpu -> b;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x0C:{ //INC C 
			unsigned char toInc = cpu -> c;
			unsigned char incremented = toInc + 1;
			cpu -> c;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x13:{ //INC DE 
			unsigned short toInc = GET_DE(cpu);
			unsigned short incremented = toInc + 1;
			SET_DE(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x14:{ //INC D 
			unsigned char toInc = cpu -> d;
			unsigned char incremented = toInc + 1;
			cpu -> d;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x1C:{ //INC E 
			unsigned char toInc = cpu -> e;
			unsigned char incremented = toInc + 1;
			cpu -> e;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x23:{ //INC HL 
			unsigned short toInc = GET_HL(cpu);
			unsigned short incremented = toInc + 1;
			SET_HL(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x24:{ //INC H 
			unsigned char toInc = cpu -> h;
			unsigned char incremented = toInc + 1;
			cpu -> h;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x2C:{ //INC L 
			unsigned char toInc = cpu -> l;
			unsigned char incremented = toInc + 1;
			cpu -> l;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		case 0x33:{ //INC SP 
			unsigned short toInc = GET_SP(cpu);
			unsigned short incremented = toInc + 1;
			SET_SP(cpu, incremented);
			increment_timer(mem, 8);
			break;
		}

		case 0x34:{ //INC HL 
			unsigned char toInc = read_mem(mem, GET_HL(cpu));
			unsigned char incremented = toInc + 1;
			write_mem(mem, GET_HL(cpu), incremented);
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 12);
			break;
		}

		case 0x3C:{ //INC A 
			unsigned char toInc = cpu -> a;
			unsigned char incremented = toInc + 1;
			cpu -> a;
			SET_ZF(cpu, (incremented == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(toInc, 0x1));
			increment_timer(mem, 4);
			break;
		}

		/**********************************
		  ADD: 8 / 16 bit add operations
		**********************************/
		case 0x09:{ //ADD HL BC
			unsigned short src = GET_BC(cpu);
			unsigned short tgt = GET_HL(cpu);
			unsigned short result = src + tgt;
			SET_HL(cpu, result);
			SET_NF(cpu,0);
			SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0x19:{ //ADD HL DE
			unsigned short src = GET_DE(cpu);
			unsigned short tgt = GET_HL(cpu);
			unsigned short result = src + tgt;
			SET_HL(cpu, result);
			SET_NF(cpu,0);
			SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0x29:{ //ADD HL HL
			unsigned short src = GET_HL(cpu);
			unsigned short tgt = GET_HL(cpu);
			unsigned short result = src + tgt;
			SET_HL(cpu, result);
			SET_NF(cpu,0);
			SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0x39:{ //ADD HL SP
			unsigned short src = GET_SP(cpu);
			unsigned short tgt = GET_HL(cpu);
			unsigned short result = src + tgt;
			SET_HL(cpu, result);
			SET_NF(cpu,0);
			SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0x80:{ //ADD A B
			unsigned char src = cpu -> b;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x81:{ //ADD A C
			unsigned char src = cpu -> c;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x82:{ //ADD A D
			unsigned char src = cpu -> d;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x83:{ //ADD A E
			unsigned char src = cpu -> e;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x84:{ //ADD A H
			unsigned char src = cpu -> h;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x85:{ //ADD A L
			unsigned char src = cpu -> l;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0x86:{ //ADD A HL
			unsigned char src = read_mem(mem, GET_HL(cpu));
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0x87:{ //ADD A A
			unsigned char src = cpu -> a;
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xC6:{ //ADD A d8
			unsigned char src = get_byte(cpu, mem);
			unsigned char tgt = cpu -> a;
			unsigned char result = src + tgt;
			cpu->a = result;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, HC_CHECK(src, tgt));
			SET_CF(cpu, C_CHECK(src, tgt, result));
			increment_timer(mem, 8);
			break;
		}

		case 0xE8:{ //ADD SP r8
			unsigned char src = get_byte(cpu, mem);
			int  srcInt = (int) src;
			unsigned short tgt = GET_SP(cpu);
			int tgtInt = (int) tgt;
			unsigned short result = (unsigned short)(srcInt + tgtInt);
			SET_SP(cpu, result);
			SET_HF(cpu, HC_CHECK(src, tgt);
			SET_CF(cpu, src, tgt, result);
			increment_timer(mem, 16);
			break;
		}

		/*******************************
		  AND: Bitwise AND operations
		*******************************/
		case 0xA0:{ //AND B 
			unsigned char tgt = cpu -> b;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA1:{ //AND C 
			unsigned char tgt = cpu -> c;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA2:{ //AND D 
			unsigned char tgt = cpu -> d;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA3:{ //AND E 
			unsigned char tgt = cpu -> e;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA4:{ //AND H 
			unsigned char tgt = cpu -> h;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA5:{ //AND L 
			unsigned char tgt = cpu -> l;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA6:{ //AND HL 
			unsigned char tgt = read_mem(mem, GET_HL(cpu);
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		case 0xA7:{ //AND A 
			unsigned char tgt = cpu -> a;
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xE6:{ //AND d8 
			unsigned char tgt = get_byte(cpu, mem);
			cpu -> a = cpu -> a & tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 1);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		/*****************************
		  OR: Bitwise OR operations
		*****************************/
		case 0xB0:{ //OR B 
			unsigned char tgt = cpu -> b;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB1:{ //OR C 
			unsigned char tgt = cpu -> c;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB2:{ //OR D 
			unsigned char tgt = cpu -> d;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB3:{ //OR E 
			unsigned char tgt = cpu -> e;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB4:{ //OR H 
			unsigned char tgt = cpu -> h;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB5:{ //OR L 
			unsigned char tgt = cpu -> l;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xB6:{ //OR HL 
			unsigned char tgt = read_mem(mem, GET_HL(cpu);
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		case 0xB7:{ //OR A 
			unsigned char tgt = cpu -> a;
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xF6:{ //OR d8 
			unsigned char tgt = get_byte(cpu, mem);
			cpu -> a = cpu -> a | tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		/*******************************
		  XOR: Bitwise XOR operations
		*******************************/
		case 0xA8:{ //XOR B 
			unsigned char tgt = cpu -> b;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xA9:{ //XOR C 
			unsigned char tgt = cpu -> c;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xAA:{ //XOR D 
			unsigned char tgt = cpu -> d;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xAB:{ //XOR E 
			unsigned char tgt = cpu -> e;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xAC:{ //XOR H 
			unsigned char tgt = cpu -> h;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xAD:{ //XOR L 
			unsigned char tgt = cpu -> l;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xAE:{ //XOR HL 
			unsigned char tgt = read_mem(mem, GET_HL(cpu);
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		case 0xAF:{ //XOR A 
			unsigned char tgt = cpu -> a;
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xEE:{ //XOR d8 
			unsigned char tgt = get_byte(cpu, mem);
			cpu -> a = cpu -> a ^ tgt;
			SET_ZF(cpu, (cpu -> a == 0));
			SET_NF(cpu, 0);
			SET_HF(cpu, 0);
			SET_CF(cpu, 0);
			increment_timer(mem, 8);
			break;
		}

		/*********************************
		  RST: Bit Resetting operations
		*********************************/
		case 0xC7:{ //RST 00H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x0);
			increment_timer(mem, 16);
			break;
		}

		case 0xCF:{ //RST 08H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x0);
			increment_timer(mem, 16);
			break;
		}

		case 0xD7:{ //RST 10H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x1);
			increment_timer(mem, 16);
			break;
		}

		case 0xDF:{ //RST 18H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x1);
			increment_timer(mem, 16);
			break;
		}

		case 0xE7:{ //RST 20H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x2);
			increment_timer(mem, 16);
			break;
		}

		case 0xEF:{ //RST 28H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x2);
			increment_timer(mem, 16);
			break;
		}

		case 0xF7:{ //RST 30H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x3);
			increment_timer(mem, 16);
			break;
		}

		case 0xFF:{ //RST 38H 
			cpu -> sp = cpu -> cp - 1;
			unsigned char highByte = get_high_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, highByte);
			cpu -> sp = cpu -> cp - 1;
			unsigned char lowByte = get_low_byte(&(cpu->pc));
			write_mem(mem, cpu->sp, lowByte);
			set_high_byte(&(pc -> sp), (char) 0x00);
			set_low_byte(&(pc -> sp), (char) 0x3);
			increment_timer(mem, 16);
			break;
		}

		/************
		  CP: None
		************/
		case 0xB8:{ //CP B 
			unsigned char subtrahend = cpu -> b;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xB9:{ //CP C 
			unsigned char subtrahend = cpu -> c;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xBA:{ //CP D 
			unsigned char subtrahend = cpu -> d;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xBB:{ //CP E 
			unsigned char subtrahend = cpu -> e;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xBC:{ //CP H 
			unsigned char subtrahend = cpu -> h;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xBD:{ //CP L 
			unsigned char subtrahend = cpu -> l;
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 4);
			break;
		}

		case 0xBE:{ //CP HL 
			unsigned char subtrahend = read_mem(mem, GET_HL(cpu));
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 8);
			break;
		}

		case 0xBF:{ //CP A 
			SET_ZF(cpu, 0);
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), (cpu->a)));
			SET_CF(cpu, 0);
			increment_timer(mem, 4);
			break;
		}

		case 0xFE:{ //CP d8 
			unsigned char subtrahend = get_byte(cpu, mem);
			unsigned char result = cpu -> a - subtrahend;
			SET_ZF(cpu, (result == 0));
			SET_NF(cpu, 1);
			SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));
			SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));
			increment_timer(mem, 8);
			break;
		}

		/***********************
		  JP: Jump Operations
		***********************/
		case 0xC2:{ //JP NZ a16
			unsigned char lowByte = get_byte(cpu, mem);
			unsigned char highByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&highByte, &lowByte);
			if GET_ZF(cpu) == 0:{
							SET_PC(cpu, address);
							increment_timer(mem, 12);
			}

			else {
							increment_timer(mem, 16);
			}

			break;
		}

		case 0xC3:{ //JP a16 
			unsigned char lowByte = get_byte(cpu, mem);
			unsigned char highByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&highByte, &lowByte);
			SET_PC(cpu, address);
			increment_timer(mem, 16);
			break;
		}

		case 0xCA:{ //JP Z a16
			unsigned char lowByte = get_byte(cpu, mem);
			unsigned char highByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&highByte, &lowByte);
			if GET_ZF(cpu) == 1:{
							SET_PC(cpu, address);
							increment_timer(mem, 12);
			}

			else {
							increment_timer(mem, 16);
			}

			break;
		}

		case 0xD2:{ //JP NC a16
			unsigned char lowByte = get_byte(cpu, mem);
			unsigned char highByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&highByte, &lowByte);
			if GET_CF(cpu) == 0:{
							SET_PC(cpu, address);
							increment_timer(mem, 12);
			}

			else {
							increment_timer(mem, 16);
			}

			break;
		}

		case 0xDA:{ //JP C a16
			unsigned char lowByte = get_byte(cpu, mem);
			unsigned char highByte = get_byte(cpu, mem);
			unsigned short address = _get_8b_to_16b(&highByte, &lowByte);
			if GET_CF(cpu) == 1:{
							SET_PC(cpu, address);
							increment_timer(mem, 12);
			}

			else {
							increment_timer(mem, 16);
			}

			break;
		}

		case 0xE9:{ //JP HL 
			SET_PC(cpu, GET_HL(cpu));
			increment_timer(mem, 4);
			break;
		}

	}
}
