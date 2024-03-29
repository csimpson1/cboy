void interpret_prefixed_opcodes(CPU *cpu, unsigned char *mem, unsigned char opcode){
	switch(opcode){
		/***************************
		  SET: Bit Set operations
		***************************/
		case 0xC0:{ //SET 0 B
			set_bit_char(&(cpu->b), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC1:{ //SET 0 C
			set_bit_char(&(cpu->c), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC2:{ //SET 0 D
			set_bit_char(&(cpu->d), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC3:{ //SET 0 E
			set_bit_char(&(cpu->e), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC4:{ //SET 0 H
			set_bit_char(&(cpu->h), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC5:{ //SET 0 L
			set_bit_char(&(cpu->l), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC6:{ //SET 0 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 0, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xC7:{ //SET 0 A
			set_bit_char(&(cpu->a), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC8:{ //SET 1 B
			set_bit_char(&(cpu->b), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xC9:{ //SET 1 C
			set_bit_char(&(cpu->c), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xCA:{ //SET 1 D
			set_bit_char(&(cpu->d), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xCB:{ //SET 1 E
			set_bit_char(&(cpu->e), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xCC:{ //SET 1 H
			set_bit_char(&(cpu->h), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xCD:{ //SET 1 L
			set_bit_char(&(cpu->l), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xCE:{ //SET 1 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 1, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xCF:{ //SET 1 A
			set_bit_char(&(cpu->a), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD0:{ //SET 2 B
			set_bit_char(&(cpu->b), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD1:{ //SET 2 C
			set_bit_char(&(cpu->c), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD2:{ //SET 2 D
			set_bit_char(&(cpu->d), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD3:{ //SET 2 E
			set_bit_char(&(cpu->e), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD4:{ //SET 2 H
			set_bit_char(&(cpu->h), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD5:{ //SET 2 L
			set_bit_char(&(cpu->l), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD6:{ //SET 2 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 2, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xD7:{ //SET 2 A
			set_bit_char(&(cpu->a), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD8:{ //SET 3 B
			set_bit_char(&(cpu->b), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xD9:{ //SET 3 C
			set_bit_char(&(cpu->c), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xDA:{ //SET 3 D
			set_bit_char(&(cpu->d), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xDB:{ //SET 3 E
			set_bit_char(&(cpu->e), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xDC:{ //SET 3 H
			set_bit_char(&(cpu->h), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xDD:{ //SET 3 L
			set_bit_char(&(cpu->l), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xDE:{ //SET 3 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 3, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xDF:{ //SET 3 A
			set_bit_char(&(cpu->a), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE0:{ //SET 4 B
			set_bit_char(&(cpu->b), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE1:{ //SET 4 C
			set_bit_char(&(cpu->c), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE2:{ //SET 4 D
			set_bit_char(&(cpu->d), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE3:{ //SET 4 E
			set_bit_char(&(cpu->e), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE4:{ //SET 4 H
			set_bit_char(&(cpu->h), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE5:{ //SET 4 L
			set_bit_char(&(cpu->l), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE6:{ //SET 4 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 4, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xE7:{ //SET 4 A
			set_bit_char(&(cpu->a), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE8:{ //SET 5 B
			set_bit_char(&(cpu->b), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xE9:{ //SET 5 C
			set_bit_char(&(cpu->c), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xEA:{ //SET 5 D
			set_bit_char(&(cpu->d), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xEB:{ //SET 5 E
			set_bit_char(&(cpu->e), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xEC:{ //SET 5 H
			set_bit_char(&(cpu->h), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xED:{ //SET 5 L
			set_bit_char(&(cpu->l), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xEE:{ //SET 5 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 5, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xEF:{ //SET 5 A
			set_bit_char(&(cpu->a), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF0:{ //SET 6 B
			set_bit_char(&(cpu->b), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF1:{ //SET 6 C
			set_bit_char(&(cpu->c), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF2:{ //SET 6 D
			set_bit_char(&(cpu->d), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF3:{ //SET 6 E
			set_bit_char(&(cpu->e), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF4:{ //SET 6 H
			set_bit_char(&(cpu->h), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF5:{ //SET 6 L
			set_bit_char(&(cpu->l), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF6:{ //SET 6 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 6, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xF7:{ //SET 6 A
			set_bit_char(&(cpu->a), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF8:{ //SET 7 B
			set_bit_char(&(cpu->b), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xF9:{ //SET 7 C
			set_bit_char(&(cpu->c), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xFA:{ //SET 7 D
			set_bit_char(&(cpu->d), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xFB:{ //SET 7 E
			set_bit_char(&(cpu->e), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xFC:{ //SET 7 H
			set_bit_char(&(cpu->h), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xFD:{ //SET 7 L
			set_bit_char(&(cpu->l), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xFE:{ //SET 7 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 7, 1);
			increment_timer(mem, 16);
			break;
		}

		case 0xFF:{ //SET 7 A
			set_bit_char(&(cpu->a), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		/*****************************
		  RES: Bit Reset Operations
		*****************************/
		case 0x80:{ //RES 0 B
			set_bit_char(&(cpu->b), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x81:{ //RES 0 C
			set_bit_char(&(cpu->c), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x82:{ //RES 0 D
			set_bit_char(&(cpu->d), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x83:{ //RES 0 E
			set_bit_char(&(cpu->e), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x84:{ //RES 0 H
			set_bit_char(&(cpu->h), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x85:{ //RES 0 L
			set_bit_char(&(cpu->l), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x86:{ //RES 0 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 0, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0x87:{ //RES 0 A
			set_bit_char(&(cpu->a), 0, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x88:{ //RES 1 B
			set_bit_char(&(cpu->b), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x89:{ //RES 1 C
			set_bit_char(&(cpu->c), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x8A:{ //RES 1 D
			set_bit_char(&(cpu->d), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x8B:{ //RES 1 E
			set_bit_char(&(cpu->e), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x8C:{ //RES 1 H
			set_bit_char(&(cpu->h), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x8D:{ //RES 1 L
			set_bit_char(&(cpu->l), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x8E:{ //RES 1 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 1, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0x8F:{ //RES 1 A
			set_bit_char(&(cpu->a), 1, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x90:{ //RES 2 B
			set_bit_char(&(cpu->b), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x91:{ //RES 2 C
			set_bit_char(&(cpu->c), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x92:{ //RES 2 D
			set_bit_char(&(cpu->d), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x93:{ //RES 2 E
			set_bit_char(&(cpu->e), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x94:{ //RES 2 H
			set_bit_char(&(cpu->h), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x95:{ //RES 2 L
			set_bit_char(&(cpu->l), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x96:{ //RES 2 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 2, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0x97:{ //RES 2 A
			set_bit_char(&(cpu->a), 2, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x98:{ //RES 3 B
			set_bit_char(&(cpu->b), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x99:{ //RES 3 C
			set_bit_char(&(cpu->c), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x9A:{ //RES 3 D
			set_bit_char(&(cpu->d), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x9B:{ //RES 3 E
			set_bit_char(&(cpu->e), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x9C:{ //RES 3 H
			set_bit_char(&(cpu->h), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x9D:{ //RES 3 L
			set_bit_char(&(cpu->l), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0x9E:{ //RES 3 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 3, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0x9F:{ //RES 3 A
			set_bit_char(&(cpu->a), 3, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA0:{ //RES 4 B
			set_bit_char(&(cpu->b), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA1:{ //RES 4 C
			set_bit_char(&(cpu->c), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA2:{ //RES 4 D
			set_bit_char(&(cpu->d), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA3:{ //RES 4 E
			set_bit_char(&(cpu->e), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA4:{ //RES 4 H
			set_bit_char(&(cpu->h), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA5:{ //RES 4 L
			set_bit_char(&(cpu->l), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA6:{ //RES 4 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 4, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0xA7:{ //RES 4 A
			set_bit_char(&(cpu->a), 4, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA8:{ //RES 5 B
			set_bit_char(&(cpu->b), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xA9:{ //RES 5 C
			set_bit_char(&(cpu->c), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xAA:{ //RES 5 D
			set_bit_char(&(cpu->d), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xAB:{ //RES 5 E
			set_bit_char(&(cpu->e), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xAC:{ //RES 5 H
			set_bit_char(&(cpu->h), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xAD:{ //RES 5 L
			set_bit_char(&(cpu->l), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xAE:{ //RES 5 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 5, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0xAF:{ //RES 5 A
			set_bit_char(&(cpu->a), 5, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB0:{ //RES 6 B
			set_bit_char(&(cpu->b), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB1:{ //RES 6 C
			set_bit_char(&(cpu->c), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB2:{ //RES 6 D
			set_bit_char(&(cpu->d), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB3:{ //RES 6 E
			set_bit_char(&(cpu->e), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB4:{ //RES 6 H
			set_bit_char(&(cpu->h), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB5:{ //RES 6 L
			set_bit_char(&(cpu->l), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB6:{ //RES 6 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 6, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0xB7:{ //RES 6 A
			set_bit_char(&(cpu->a), 6, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB8:{ //RES 7 B
			set_bit_char(&(cpu->b), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xB9:{ //RES 7 C
			set_bit_char(&(cpu->c), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xBA:{ //RES 7 D
			set_bit_char(&(cpu->d), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xBB:{ //RES 7 E
			set_bit_char(&(cpu->e), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xBC:{ //RES 7 H
			set_bit_char(&(cpu->h), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xBD:{ //RES 7 L
			set_bit_char(&(cpu->l), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		case 0xBE:{ //RES 7 HL
			unsigned char addr = read_mem(mem, GET_HL(cpu));
			set_bit_char(&addr, 7, 0);
			increment_timer(mem, 16);
			break;
		}

		case 0xBF:{ //RES 7 A
			set_bit_char(&(cpu->a), 7, 1);
			increment_timer(mem, 8);
			break;
		}

		/**********************************
		  BIT: Bit compliment operations
		**********************************/
		case 0x40:{ //BIT 0 B
			unsigned char bit = get_bit((int) cpu -> b, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x41:{ //BIT 0 C
			unsigned char bit = get_bit((int) cpu -> c, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x42:{ //BIT 0 D
			unsigned char bit = get_bit((int) cpu -> d, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x43:{ //BIT 0 E
			unsigned char bit = get_bit((int) cpu -> e, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x44:{ //BIT 0 H
			unsigned char bit = get_bit((int) cpu -> h, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x45:{ //BIT 0 L
			unsigned char bit = get_bit((int) cpu -> l, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x46:{ //BIT 0 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 0));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x47:{ //BIT 0 A
			unsigned char bit = get_bit((int) cpu -> a, 0);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x48:{ //BIT 1 B
			unsigned char bit = get_bit((int) cpu -> b, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x49:{ //BIT 1 C
			unsigned char bit = get_bit((int) cpu -> c, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x4A:{ //BIT 1 D
			unsigned char bit = get_bit((int) cpu -> d, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x4B:{ //BIT 1 E
			unsigned char bit = get_bit((int) cpu -> e, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x4C:{ //BIT 1 H
			unsigned char bit = get_bit((int) cpu -> h, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x4D:{ //BIT 1 L
			unsigned char bit = get_bit((int) cpu -> l, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x4E:{ //BIT 1 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 1));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x4F:{ //BIT 1 A
			unsigned char bit = get_bit((int) cpu -> a, 1);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x50:{ //BIT 2 B
			unsigned char bit = get_bit((int) cpu -> b, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x51:{ //BIT 2 C
			unsigned char bit = get_bit((int) cpu -> c, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x52:{ //BIT 2 D
			unsigned char bit = get_bit((int) cpu -> d, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x53:{ //BIT 2 E
			unsigned char bit = get_bit((int) cpu -> e, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x54:{ //BIT 2 H
			unsigned char bit = get_bit((int) cpu -> h, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x55:{ //BIT 2 L
			unsigned char bit = get_bit((int) cpu -> l, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x56:{ //BIT 2 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 2));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x57:{ //BIT 2 A
			unsigned char bit = get_bit((int) cpu -> a, 2);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x58:{ //BIT 3 B
			unsigned char bit = get_bit((int) cpu -> b, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x59:{ //BIT 3 C
			unsigned char bit = get_bit((int) cpu -> c, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x5A:{ //BIT 3 D
			unsigned char bit = get_bit((int) cpu -> d, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x5B:{ //BIT 3 E
			unsigned char bit = get_bit((int) cpu -> e, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x5C:{ //BIT 3 H
			unsigned char bit = get_bit((int) cpu -> h, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x5D:{ //BIT 3 L
			unsigned char bit = get_bit((int) cpu -> l, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x5E:{ //BIT 3 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 3));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x5F:{ //BIT 3 A
			unsigned char bit = get_bit((int) cpu -> a, 3);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x60:{ //BIT 4 B
			unsigned char bit = get_bit((int) cpu -> b, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x61:{ //BIT 4 C
			unsigned char bit = get_bit((int) cpu -> c, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x62:{ //BIT 4 D
			unsigned char bit = get_bit((int) cpu -> d, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x63:{ //BIT 4 E
			unsigned char bit = get_bit((int) cpu -> e, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x64:{ //BIT 4 H
			unsigned char bit = get_bit((int) cpu -> h, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x65:{ //BIT 4 L
			unsigned char bit = get_bit((int) cpu -> l, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x66:{ //BIT 4 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 4));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x67:{ //BIT 4 A
			unsigned char bit = get_bit((int) cpu -> a, 4);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x68:{ //BIT 5 B
			unsigned char bit = get_bit((int) cpu -> b, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x69:{ //BIT 5 C
			unsigned char bit = get_bit((int) cpu -> c, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x6A:{ //BIT 5 D
			unsigned char bit = get_bit((int) cpu -> d, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x6B:{ //BIT 5 E
			unsigned char bit = get_bit((int) cpu -> e, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x6C:{ //BIT 5 H
			unsigned char bit = get_bit((int) cpu -> h, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x6D:{ //BIT 5 L
			unsigned char bit = get_bit((int) cpu -> l, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x6E:{ //BIT 5 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 5));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x6F:{ //BIT 5 A
			unsigned char bit = get_bit((int) cpu -> a, 5);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x70:{ //BIT 6 B
			unsigned char bit = get_bit((int) cpu -> b, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x71:{ //BIT 6 C
			unsigned char bit = get_bit((int) cpu -> c, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x72:{ //BIT 6 D
			unsigned char bit = get_bit((int) cpu -> d, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x73:{ //BIT 6 E
			unsigned char bit = get_bit((int) cpu -> e, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x74:{ //BIT 6 H
			unsigned char bit = get_bit((int) cpu -> h, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x75:{ //BIT 6 L
			unsigned char bit = get_bit((int) cpu -> l, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x76:{ //BIT 6 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 6));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x77:{ //BIT 6 A
			unsigned char bit = get_bit((int) cpu -> a, 6);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x78:{ //BIT 7 B
			unsigned char bit = get_bit((int) cpu -> b, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x79:{ //BIT 7 C
			unsigned char bit = get_bit((int) cpu -> c, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x7A:{ //BIT 7 D
			unsigned char bit = get_bit((int) cpu -> d, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x7B:{ //BIT 7 E
			unsigned char bit = get_bit((int) cpu -> e, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x7C:{ //BIT 7 H
			unsigned char bit = get_bit((int) cpu -> h, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x7D:{ //BIT 7 L
			unsigned char bit = get_bit((int) cpu -> l, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		case 0x7E:{ //BIT 7 HL
			unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), 7));
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 12);
			break;
		}

		case 0x7F:{ //BIT 7 A
			unsigned char bit = get_bit((int) cpu -> a, 7);
			unsigned char compliment = (bit == 0 : 1 : 0);
			SET_ZF(cpu, compliment);
			increment_timer(mem, 8);
			break;
		}

		/********************************
		  SWAP: Swap nibble operations
		********************************/
		case 0x30:{ //SWAP B 
			unsigned char toSwap = cpu->b;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->b = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x31:{ //SWAP C 
			unsigned char toSwap = cpu->c;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->c = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x32:{ //SWAP D 
			unsigned char toSwap = cpu->d;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->d = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x33:{ //SWAP E 
			unsigned char toSwap = cpu->e;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->e = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x34:{ //SWAP H 
			unsigned char toSwap = cpu->h;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->h = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x35:{ //SWAP L 
			unsigned char toSwap = cpu->l;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->l = swapped;
			increment_timer(mem, 8);
			break;
		}

		case 0x36:{ //SWAP HL 
			unsigned char toSwap = read_mem(mem, GET_HL(cpu));
			unsigned char swapped = swap_nibble(toSwap);
			write_mem(mem, GET_HL(cpu), swapped;
			increment_timer(mem, 16);
			break;
		}

		case 0x37:{ //SWAP A 
			unsigned char toSwap = cpu->a;
			unsigned char swapped = swap_nibble(toSwap);
			cpu->a = swapped;
			increment_timer(mem, 8);
			break;
		}

		/*****************************
		  SRA: SRA Shift operations
		*****************************/
		case 0x28:{ //SRA B 
			unsigned char toShift = cpu -> b;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> b = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x29:{ //SRA C 
			unsigned char toShift = cpu -> c;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> c = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x2A:{ //SRA D 
			unsigned char toShift = cpu -> d;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> d = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x2B:{ //SRA E 
			unsigned char toShift = cpu -> e;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> e = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x2C:{ //SRA H 
			unsigned char toShift = cpu -> h;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> h = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x2D:{ //SRA L 
			unsigned char toShift = cpu -> l;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> l = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x2E:{ //SRA HL 
			unsigned char toShift = read_mem(mem, GET_HL(cpu));
			unsigned char shifted = sra(toShift, cpu);
			write_mem(mem, GET_HL(cpu), shifted);
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x2F:{ //SRA A 
			unsigned char toShift = cpu -> a;
			unsigned char shifted = sra(toShift, cpu);
			cpu -> a = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		/*****************************
		  SRL: SRL Shift operations
		*****************************/
		case 0x38:{ //SRL B 
			unsigned char toShift = cpu -> b;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> b = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x39:{ //SRL C 
			unsigned char toShift = cpu -> c;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> c = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x3A:{ //SRL D 
			unsigned char toShift = cpu -> d;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> d = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x3B:{ //SRL E 
			unsigned char toShift = cpu -> e;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> e = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x3C:{ //SRL H 
			unsigned char toShift = cpu -> h;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> h = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x3D:{ //SRL L 
			unsigned char toShift = cpu -> l;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> l = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x3E:{ //SRL HL 
			unsigned char toShift = read_mem(mem, GET_HL(cpu));
			unsigned char shifted = srl(toShift, cpu);
			write_mem(mem, GET_HL(cpu), shifted);
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x3F:{ //SRL A 
			unsigned char toShift = cpu -> a;
			unsigned char shifted = srl(toShift, cpu);
			cpu -> a = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		/*****************************
		  SLA: SLA Shift operations
		*****************************/
		case 0x20:{ //SLA B 
			unsigned char toShift = cpu -> b;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> b = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x21:{ //SLA C 
			unsigned char toShift = cpu -> c;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> c = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x22:{ //SLA D 
			unsigned char toShift = cpu -> d;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> d = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x23:{ //SLA E 
			unsigned char toShift = cpu -> e;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> e = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x24:{ //SLA H 
			unsigned char toShift = cpu -> h;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> h = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x25:{ //SLA L 
			unsigned char toShift = cpu -> l;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> l = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x26:{ //SLA HL 
			unsigned char toShift = read_mem(mem, GET_HL(cpu));
			unsigned char shifted = sla(toShift, cpu);
			write_mem(mem, GET_HL(cpu), shifted);
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x27:{ //SLA A 
			unsigned char toShift = cpu -> a;
			unsigned char shifted = sla(toShift, cpu);
			cpu -> a = shifted;
			SET_ZF(cpu, (shifted == 0));
			increment_timer(mem, 8);
			break;
		}

		/************
		  RR: None
		************/
		case 0x18:{ //RR B 
			unsigned char toRotate = rot_right(cpu -> b, cpu);
			cpu -> b = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x19:{ //RR C 
			unsigned char toRotate = rot_right(cpu -> c, cpu);
			cpu -> c = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x1A:{ //RR D 
			unsigned char toRotate = rot_right(cpu -> d, cpu);
			cpu -> d = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x1B:{ //RR E 
			unsigned char toRotate = rot_right(cpu -> e, cpu);
			cpu -> e = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x1C:{ //RR H 
			unsigned char toRotate = rot_right(cpu -> h, cpu);
			cpu -> h = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x1D:{ //RR L 
			unsigned char toRotate = rot_right(cpu -> l, cpu);
			cpu -> l = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x1E:{ //RR HL 
			unsigned short address = GET_HL(cpu);
			unsigned char toRotate = read_mem(cpu, address);
			write_mem(cpu, address);
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x1F:{ //RR A 
			unsigned char toRotate = rot_right(cpu -> a, cpu);
			cpu -> a = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		/*************
		  RRC: None
		*************/
		case 0x08:{ //RRC B 
			unsigned char toRotate = rot_left_carry(cpu -> b, cpu);
			cpu -> b = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x09:{ //RRC C 
			unsigned char toRotate = rot_left_carry(cpu -> c, cpu);
			cpu -> c = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x0A:{ //RRC D 
			unsigned char toRotate = rot_left_carry(cpu -> d, cpu);
			cpu -> d = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x0B:{ //RRC E 
			unsigned char toRotate = rot_left_carry(cpu -> e, cpu);
			cpu -> e = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x0C:{ //RRC H 
			unsigned char toRotate = rot_left_carry(cpu -> h, cpu);
			cpu -> h = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x0D:{ //RRC L 
			unsigned char toRotate = rot_left_carry(cpu -> l, cpu);
			cpu -> l = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x0E:{ //RRC HL 
			unsigned short address = GET_HL(cpu);
			unsigned char toRotate = read_mem(cpu, address);
			write_mem(cpu, address);
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x0F:{ //RRC A 
			unsigned char toRotate = rot_left_carry(cpu -> a, cpu);
			cpu -> a = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		/************
		  RL: None
		************/
		case 0x10:{ //RL B 
			unsigned char toRotate = rot_left(cpu -> b, cpu);
			cpu -> b = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x11:{ //RL C 
			unsigned char toRotate = rot_left(cpu -> c, cpu);
			cpu -> c = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x12:{ //RL D 
			unsigned char toRotate = rot_left(cpu -> d, cpu);
			cpu -> d = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x13:{ //RL E 
			unsigned char toRotate = rot_left(cpu -> e, cpu);
			cpu -> e = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x14:{ //RL H 
			unsigned char toRotate = rot_left(cpu -> h, cpu);
			cpu -> h = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x15:{ //RL L 
			unsigned char toRotate = rot_left(cpu -> l, cpu);
			cpu -> l = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x16:{ //RL HL 
			unsigned short address = GET_HL(cpu);
			unsigned char toRotate = read_mem(cpu, address);
			write_mem(cpu, address);
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x17:{ //RL A 
			unsigned char toRotate = rot_left(cpu -> a, cpu);
			cpu -> a = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		/*************
		  RLC: None
		*************/
		case 0x00:{ //RLC B 
			unsigned char toRotate = rot_right_carry(cpu -> b, cpu);
			cpu -> b = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x01:{ //RLC C 
			unsigned char toRotate = rot_right_carry(cpu -> c, cpu);
			cpu -> c = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x02:{ //RLC D 
			unsigned char toRotate = rot_right_carry(cpu -> d, cpu);
			cpu -> d = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x03:{ //RLC E 
			unsigned char toRotate = rot_right_carry(cpu -> e, cpu);
			cpu -> e = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x04:{ //RLC H 
			unsigned char toRotate = rot_right_carry(cpu -> h, cpu);
			cpu -> h = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x05:{ //RLC L 
			unsigned char toRotate = rot_right_carry(cpu -> l, cpu);
			cpu -> l = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

		case 0x06:{ //RLC HL 
			unsigned short address = GET_HL(cpu);
			unsigned char toRotate = read_mem(cpu, address);
			write_mem(cpu, address);
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 16);
			break;
		}

		case 0x07:{ //RLC A 
			unsigned char toRotate = rot_right_carry(cpu -> a, cpu);
			cpu -> a = toRotate;
			SET_ZF(cpu, (toRotate == 0));
			increment_timer(mem, 8);
			break;
		}

	}
}
