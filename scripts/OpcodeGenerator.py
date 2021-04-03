from collections import Counter as count
from functools import partial
import json
import mariadb
import os
import OpcodeReport
import requests
import sys

class OpcodeGenerator:
    def __init__(self):
        self.cur = self.connect()
        self.indentLevel = 0;
        
        self.queries = {
                        'mnemonicQuery' : """
                                            select distinct code,cycles,bytes 
                                            from opcodes_v 
                                            where mnemonic=?; 
                                            """,
                        'operandQuery' : """
                                            select 
                                            operand_name,
                                            op_immediate,
                                            op_action
                                            from opcodes_v
                                            where code =?
                                            and op_order =?;
                                            """
                        }
        """
        Given a mnemonic, define how you want to create the cases for that mnemomic.
        The mnemonic here will also be used in the output filename for the cases
        """
        self.case_builder={
                            #Set and reset are basically the same, with a change in the value. 
                            #Using partial evaluation to save some code
                            'SET' : partial(self._build_case_set_reset, 1),
                            'RES' : partial(self._build_case_set_reset, 0),
                            'LD'  : self._build_case_ld,
                            'BIT' : self._build_case_bit,
                            'SWAP': self._build_case_swap,
                            'INC' : self._build_case_inc,
                            'ADD' : self._build_case_add,
                            'AND' : self._build_case_and
                            }
        
        """
        Strings to be used in the comments of each set of cases
        """
        self.descriptions={
                            'SET' : "Bit setting operations",
                            'RES' : "Bit Resetting operations",
                            'LD'  : "Load operations",
                            'BIT' : "Bit compliment operations",
                            'SWAP': "Swap nibble operations",
                            'INC' : '8 / 16 bit increment operations',
                            'ADD' : '8 / 16 bit add operations',
                            'AND' : 'Bitwise AND operations'
                            }
        
        
    
    def connect(self):
        try:
            self.conn = mariadb.connect(
                user='root',
                host='localhost',
                port=3306,
                database='gameboy_opcodes'
                )
            
        except mariadb.Error as e:
            print(f"Error connecting to MariaDB Platform: {e}")
            sys.exit(-1)
        
        cur = self.conn.cursor()
        print("Connected to DB!")
        return cur

    def indent_string(self, string):
        #Return a string with a specified number of tabs appended to the start
        indent=''
        for i in range(self.indentLevel):
            indent += '\t'
            
        return indent + string
    
    def cleanup(self, fName):
        #Removes a filename from the directory. Mainly used when testing this script so we don't append to a garbage file
        
        if os.path.exists(fName):
            print(f'Removing {fName}')
            os.remove(fName)
        
        else:
            print(f'No file {fName}, skipping cleanup')
            
    def print_case_error(self, var, params):
            print(f'Error determining code for {var} variable')
            print(params)
    
    def _build_case_and(self, tgt, src, bytes):
        """
        The AND operation takes the bitwise AND of some 8b value and the accumulator,
        and stores that value in the accumulator. 
        
         REG  ^ A -> A
        (REG) ^ A -> A
          d8  ^ A -> A
        """
        tgtName = tgt[0]
        case = []
        
        if tgtName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L']:
            case.append(f'unsigned char tgt = cpu -> {tgtName.lower()};\n')
        
        elif tgtName == 'HL':
            case.append('unsigned char tgt = read_mem(mem, GET_HL(cpu);\n')
        
        elif tgtName == 'd8':
            case.append('unsigned char tgt = get_byte(cpu, mem);\n')
        
        else:
            self.print_case_error('src', src)
            
        case.append('cpu -> a = cpu -> a & tgt;\n')
        
        #flags
        case.append('SET_ZF(cpu, (cpu -> a == 0));\n')
        case.append('SET_NF(cpu, 0);\n')
        case.append('SET_HF(cpu, 0);\n')
        case.append('SET_CF(cpu, 0);\n')
        
        case = map(self.indent_string, case)
        return ''.join(case)
        
        
    
    def _build_case_add(self, tgt, src, bytes):
        """
        There are a few kinds of add operations
        
        16B       + HL -> HL
        8b        + A  -> A
        8b Memory + A  -> A
        8b Imm    + A  -> A
        S8 Imm    + SP -> SP
        
        Flags for anything with an unsigned 8b operand are the same, Z, 0 ,H, C
        
        For S8 Imm, flags are 0,0,H,C
        
        For 16b registers, the flags are -, 0, H, C
        
        Where we have signed data, I think it is best to do away with shorts and chars.
        Everthing needs to be cast to an INT to make sure that 1) The data type can actually hold 
        whatever value we're storing and 2) We can actually perform a subtraction if needed.
        
        
        """
        
        tgtName = tgt[0]
        srcName = src[0]
        srcImm = src[1]
        
        case = []
        
        #8 bit add
        if srcName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L'] or (srcName == 'HL' and srcImm == 0) or (srcName == 'd8' and tgtName == 'A'):
            #Reading data from a memory address
            if srcName == 'HL' and srcImm == 0:
                case.append('unsigned char src = read_mem(mem, GET_HL(cpu));\n')
            
            #Reading immediate data from the stream
            elif(srcName == 'd8' and tgtName == 'A'):
                case.append('unsigned char src = get_byte(cpu, mem);\n')
            
            #Reading data from a register    
            else:
                case.append(f'unsigned char src = cpu -> {srcName.lower()};\n')
                
            case.append(f'unsigned char tgt = cpu -> {tgtName.lower()};\n')
            case.append(f'unsigned char result = src + tgt;\n')
            case.append(f'cpu->{tgtName.lower()} = result;\n')
            
            #Set flags
            case.append('SET_ZF(cpu, (result == 0));\n')
            case.append('SET_NF(cpu, 0);\n')
            case.append('SET_HF(cpu, HC_CHECK(src, tgt));\n')
            case.append('SET_CF(cpu, C_CHECK(src, tgt, result));\n')
            
        #16 bit add    
        elif srcName in ['BC', 'DE', 'EF', 'HL', 'SP']:
            case.append(f'unsigned short src = GET_{srcName}(cpu);\n')
            case.append(f'unsigned short tgt = GET_{tgtName}(cpu);\n')
            case.append('unsigned short result = src + tgt;\n')
            case.append(f'SET_{tgtName}(cpu, result);\n')
            
            #Set flags
            case.append('SET_NF(cpu,0);\n')
            #Checking if a carry occurs from bit 11 to 12
            case.append('SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));\n')
            case.append('SET_CF(cpu, C_CHECK(src, tgt, result));\n')
            
        #Signed addition to the stack pointer
        elif srcName == 'r8':
            case.append('unsigned char src = get_byte(cpu, mem);\n')
            case.append('int  srcInt = (int) src;\n')
            case.append(f'unsigned short tgt = GET_{tgtName}(cpu);\n')
            case.append('int tgtInt = (int) tgt;\n')
            #??
            case.append('unsigned short result = (unsigned short)(srcInt + tgtInt);\n')
            case.append(f'SET_{tgtName}(cpu, result);\n')
            
            #set flags
            #??
            case.append('SET_HF(cpu, HC_CHECK(src, tgt);\n')
            #This needs to be tested with a test ROM, not sure on the behavior here
            case.append('SET_CF(cpu, src, tgt, result);\n')
        
        else:
            self.print_case_error('src', src)
            
        
        case = map(self.indent_string, case)
        return ''.join(case)
            
            
        
        
    
    def _build_case_inc(self, tgt, src, bytes):
        """
        The increment operation adds 1 to a target register. There's two flavors:
        1) We're incrementing an 8 bit register, either in the CPU, or in memory
        2) We're incrementing a 16 bit register in the CPU
        
        The 16b operation is simpler, it sets no flags. Flags across all 8bit operations 
        are the same as well, so this is done in a helper function
        """
        
        tgtName = tgt[0]
        tgtImmediate = tgt[1]
        case = []
        
        #We're incrementing an 8 bit registers
        if tgtName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char toInc = cpu -> {tgtName.lower()};\n')
            case.append('unsigned char incremented = toInc + 1;\n')
            case.append(f'cpu -> {tgtName.lower()};\n')
            self._build_flags_inc(case)
        
        #Incrementing an 8 bit register in memory    
        elif tgtName =='HL' and tgtImmediate == 0:
            case.append(f'unsigned char toInc = read_mem(mem, GET_HL(cpu));\n')
            case.append('unsigned char incremented = toInc + 1;\n')
            case.append(f'write_mem(mem, GET_HL(cpu), incremented);\n')
            self._build_flags_inc(case)
            
        elif tgtName in ['BC', 'DE', 'HL', 'SP']:
            case.append(f'unsigned short toInc = GET_{tgtName}(cpu);\n')
            case.append('unsigned short incremented = toInc + 1;\n')
            case.append(f'SET_{tgtName}(cpu, incremented);\n')
        else:
            self.print_case_error('tgt', tgt)
            
        case = map(self.indent_string, case)
        return ''.join(case)
            
    def _build_flags_inc(self, case):
        case.append('SET_ZF(cpu, (incremented == 0));\n')
        case.append('SET_NF(cpu, 0);\n')
        case.append('SET_HF(cpu, HC_CHECK(toInc, 0x1));\n')

    def _build_case_swap(self, tgt, src, bytes):
        tgtName = tgt[0]
        case = []
        
        
        if tgtName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char toSwap = cpu->{tgtName.lower()};\n')
            case.append ('unsigned char swapped = swap_nibble(toSwap);\n')
            case.append(f'cpu->{tgtName.lower()} = swapped;\n')
        
        elif tgtName =='HL':
            case.append('unsigned char toSwap = read_mem(mem, GET_HL(cpu));\n')
            case.append('unsigned char swapped = swap_nibble(toSwap);\n')
            case.append('write_mem(mem, GET_HL(cpu), swapped;\n')
        
        else:
            self.print_case_error('tgt', tgt)
        
        case = map(self.indent_string, case)
        
        return "".join(case)
    def _build_case_bit(self, tgt, src, bytes):
        """
        Creates case statements for the BIT operation. This operation takes the compliment of a given bit from a register, and stores that value into the Z flag of the
        cpu. The src registers can be any of the 8 bit registers in the CPU, or from the value of an address pointed to by the HL register
        """
        case = []
        srcName = src[0]
        tgtName = tgt[0]
        
        
        #Get registers directly from the CPU
        if srcName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char bit = get_bit((int) cpu -> {srcName.lower()}, {tgtName});\n')
        
        #Memory addresses are read from mem    
        elif srcName == 'HL':
            case.append(f'unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), {tgtName}));\n')
            
        else:
            print('Error determining code for source variable')
            print(src)
            sys.exit(-1)
            
        #Just checking the tgt name, to make sure it is a valid bit position    
        if tgtName in [f'{i}' for i in range(8)]:
            pass
        else:
            self.print_case_error('tgt', tgt)
        
            
            
        case.append('unsigned char compliment = (bit == 0 : 1 : 0);\n')
        case.append('SET_ZF(cpu, compliment);\n')
        
        caseIndented = map(self.indent_string, case)
        return "".join(caseIndented)
        
                
    
    def _build_case_ld(self, tgt, src, bytes):
        """
        Just a wrapper for the two other case builders, uses the size of the operation to decide what's what 
        """
        if bytes < 3:
            return self._build_case_ld_8b(tgt, src)
        
        else:
            return self._build_case_ld_16b(tgt, src)
    
    def _build_case_ld_8b(self, tgt, src):
        """
        Creates case statements for 8 bit load operations.
        
        These operations follow a simple pattern. The first operand is 
        the target location for the load operation, and the second is the source.
        There's four forms that a load operation can take (SRC -> TGT)
        
        1) REG  -> REG
        2) IMM  -> REG
        3) ADDR -> REG
        4) REG  -> ADDR
        
        For each of the SRC and TGT types, these are then handled using c code 
        specific to the implementation of the emulator
        
        There are specific cases where there is an action taken on one of the operands as well
        such as incrementing or decrementing 
        
        """
        
        
        #TODO: need to handle the special case of 0xF8,
        # where a signed operation is handled
        if tgt[0] == 'SP' or src[0] == 'SP':
            print("Skipping case  for now")
            return
        
    
        
        """
        Start by determining how to access the src operand.
        """
        
        srcString = ""
       
        
        srcName = src[0]
        
        #TODO: Maybe instead of calling indent string, we can store all the strings in a list, and then just run a map command, and join it into the final string?
         #8b immediate data is read from the stream
        if srcName == 'd8':
            srcString += self.indent_string('unsigned char src = get_byte(cpu, mem);\n')
        
        #8b registers need to be read from the cpu struct itself
        elif srcName in ['A','F','B','C','D','E','H','L']:
            srcString += self.indent_string(f'unsigned char src =  cpu->{srcName.lower()};\n')
        
        #addresses are stored in one of 3 registers, and need to be accessed using the memory module    
        elif srcName in ['BC', 'DE', 'HL']:
            srcString += self.indent_string(f'unsigned char src =  read_mem(mem, GET_{srcName}(cpu));\n')
        
        
        else:
            self.print_case_error('src', src)

                    
        
        #Dealing with actions. Custom actions are only taken on the 16b registers, so 
        #we only need to consider actions on them
        srcAction = src[2]
        srcActionString = ""
        if srcAction:
            srcActionString = self.indent_string(f'SET_{srcName}(cpu,src {srcAction} 1);\n')
        
        srcString = srcString + srcActionString
        
        """
        Check how to store the source operand into the target operand
        """
        
        tgtName = tgt[0]
        
        
        #If the target is a register, we need to acceess it via the cpu struct
        if tgtName in ['A','F','B','C','D','E','H','L']:
            tgtString = f'cpu->{tgtName.lower()} = src;\n'
            
        #If the target is a mem address, the write_mem function needs to be used
        elif tgtName in ['BC', 'DE', 'HL']:
            tgtString = f'write_mem(mem, GET_{tgtName}(cpu), src);\n'
            
        else:
            self.print_case_error('tgt', tgt)
        
        #Dealing with actions. Same case as for the src variable. We only need to use the 16b macro to increment 
        tgtAction = tgt[2]
        tgtActionString = ""
        if tgtAction:
            tgtActionString = self.indent_string(f'SET_{tgtName}(cpu, GET_{tgtName}(cpu) {tgtAction} 1);\n')
        
        #action string is already properly indented, so we just need to add an indent to the start of the string
        tgtString = self.indent_string(tgtString + tgtActionString)
        
        #Smash everything together and return
        return srcString + tgtString
        
    def _build_case_ld_16b(self, tgt, src):
        """
        In the case of 16b registers, we have the following forms
        
        (SRC -> TGT)
        
        1) IMM -> REG
        2) SP -> ADDR, ADDR + 0x1
        3) A -> ADDR
        4) ADDR -> A 
        
        Explanations:
        
        1) Take the two bytes of data following the opcode, combine into a short,
        and load this into whatever target is specified
        
        2 Split the SP into high and low bytes. Get the next two bytes following 
        the opcode, and combine into a short. This is the address of where to 
        store the low byte of SP. The high byte will be stored at 0x(ADDR) + 1
        
        3) Get the next two bytes after the operand, and combine into a short. 
        Store the contents of the accumulator register at this address
        
        4) Get the next two bytes after the operand, and combine into a short. 
        Store the contents of this memory address into the accumulator
        
        """
        
        case = []
        srcName = src[0]
        
        #16b immediate data needs to be read from the stream one byte at a time
        if srcName == 'd16':
            case.append('unsigned char srcLowByte =  get_byte(cpu, mem);\n')
            case.append('unsigned char srcHighByte = get_byte(cpu, mem);\n')
            case.append('unsigned short src = _get_8b_to_16b(&srcHighByte, &srcLowByte);\n')
           
        #16b address data also needs to be read one byte at a time    
        elif srcName == 'a16':
            case.append('unsigned char srcLowByte =  get_byte(cpu, mem);\n')
            case.append('unsigned char srcHighByte = get_byte(cpu, mem);\n')
            case.append('unsigned short address = _get_8b_to_16b(&srcHighByte, &srcLowByte);\n')
            case.append('unsigned short src = read_mem(mem, address);\n')
        #Really only used for one opcode, 0x08 which is a bit of a special case    
        elif srcName =='SP':
            case.append('unsigned char spLowByte = get_low_byte &(cpu -> sp);\n')
            case.append('unsigned char spHighByte = get_high_byte(&(cpu -> sp);\n')
        
        elif srcName == 'A':
            case.append('unsigned char src = cpu -> a;\n')
        
        else:
            self.print_case_error('tgt', tgt)
            
        #No actions are needed for these loads, so we skip this part
        
        tgtName = tgt[0]
        #Need to use the SET macro to properly set the 16b register representations
        if tgtName in ['BC', 'DE', 'HL', 'SP']:
            case.append(f'SET_{tgtName}(cpu, src);\n')
        
        #If the target is memory, we need to assemble the address    
        elif tgtName == 'a16':
            case.append('unsigned char addrLow = get_byte(cpu, mem);\n')
            case.append('unsigned char addrHigh = get_byte(cpu, mem);\n')
            case.append('unsigned short addr = _get_8b_to_16b(&addrLow, &addrHigh);\n')
            
            #0x08 needs to be handled differently than other cases where the target is memory
            if srcName == 'SP':
                case.append('writemem(mem, addr, spLowByte);\n')
                case.append('writemem(mem, addr + 0x1, spHighByte);\n')
                
            else:
                case.append('writemem(mem, addr, src;\n')
        
        elif tgtName == 'A':
            case.append("cpu -> a = src;\n")
            
            
        else:
            self.print_case_error('tgt', tgt)
        
        #Indent the lines of code in the case statement, and join them together into a single string    
        caseIndented = map(self.indent_string, case)
        return "".join(caseIndented)
    
    def _build_case_set_reset(self, val, tgt, src, bytes):
        """
        The set/ reset functions are taking a bit of some register, and setting it to 1 or 0. The first 'operand' (not really passed as one, but represented as
        such in the dataset) is the bit, the second is the register. There's two main patterns
        
        bit -> register
        bit -> memory location

        """
        
        bit = tgt[0]
        reg = src[0]
        
        
        case = ""
        
        if reg in ['A','F','B','C','D','E','H','L']:
            case = self.indent_string(f"set_bit_char(&(cpu->{reg.lower()}), {bit}, 1);\n")
            
        elif reg == "HL":
            command1 = self.indent_string("unsigned char addr = read_mem(mem, GET_HL(cpu));\n")
            command2 = self.indent_string(f'set_bit_char(&addr, {bit}, {val});\n')
            case = command1 + command2
        
        else:
            self.print_case_error("src and tgt", src)
            print(tgt)
        
        return case
            
    def create_cases(self, mnemonic, indentLevel = 2):
        """
        Creates the case statements for a given mnemonic. High level this consists of
        
        1) Running two queries to get information on all operations with a particular mnemonic
        2) Creating a comment, outlining that we are starting a new section of cases
        3) Creating the case statement. Every case statement has 
            * A declaration case VAL {
            
            * Variable creations and assignments. This is deferred to a function specified 
              by each mnemonic, which may wrap others depending on how complicated it is to implement
              each mnenonic
              
            * A statement to increment the timer by some number of cycles
            
            * A break statement
            
            * A closing bracket
            
        Variable creation and assignment functions are found in self.case_builder. Descriptions for comments
        are in self.descriptions.
        
        To implement a new case, you need to create a function that will build the variable creation and assignment statements
        link this with the mnemonic in self.case_builder, create a description of the new cas linked with the mnemonic
        in self.descriptions
        
        Some tricks
        
        The function reference in self.case_builder needs to have the following signature
        
        fn(self, [optional args], src, tgt, bytes)
        where
        [optional args]: Not optional in the traditional python sense. These are positional arguments that are evaluated at the time
        the function is specified in the case_builder dict. 
        
        An example of this is the set/reset operations. These behave exactly the same, except for the value the bit is set to.
        Instead of duplicating the functions, the bit is specified using partial function evaluation, and the appropriate evaluation
        is specified for each mnemonic.
        
        src: A tuple consisting of operand_name, op_immediate, op_action of the source operand
        
        tgt: A tuple consisting of operand_name, op_immediate, op_action of the target operand
        
        bytes: the size of the operand in bytes

        """
        self.indentLevel = indentLevel
        #If the file already exists, clean it up.
        #fName = f'{mnemonic}_cases.txt'
        #self.cleanup(fName)
        
        #Execute the query that corresponds to this mnemonic
        self.cur.execute(self.queries['mnemonicQuery'], (mnemonic,))
        results = self.cur.fetchall()
        
        lines = []
        
        #Creating the comment to append each section of cases:
        comment = []
        commentString = f'  {mnemonic}: {self.descriptions[mnemonic]}\n'
        dots = "*" * (len(commentString) + 1)
        
        comment.append('/' + dots + '\n')
        comment.append(commentString)
        comment.append(dots +  '/' + '\n')
        
        lines += list(map(self.indent_string, comment))
        
        for row in results:
            code = row[0]
            cycles = row[1]
            bytes = row[2]
            
            #TODO: How should this be handled in cases where there's not 2 operands?
            self.cur.execute(self.queries['operandQuery'], (code, 1))
            tgt = self.cur.fetchone()
            self.cur.execute(self.queries['operandQuery'], (code, 2))
            src = self.cur.fetchone()
            
            
            #prefixed opcodes will be dealt with in a separate function, it's assumed we've already ready the first CB byte
            #so strip it
            if '0xCB' in code:
                code = code[0:2] + code[4:]
            
            #Call the function that will build the case statement for this particular operation
            #with open(fName, 'a') as f:
            
                

            srcName = src[0] if src is not None else None
            tgtName = tgt[0] if tgt is not None else None
            print(f'Creating case for {code} {tgtName} {srcName}')    
            
            #First see if we can generate a case for this code. 
            #Need to increment and decrement the indent level so we match that of the inside of the case
            self.indentLevel += 1
            caseStr = self.case_builder[mnemonic](tgt, src, bytes)
            self.indentLevel -= 1

            
            if caseStr:
                
                #If there's no source or target we need to provide some value, so provide a null one

                
                
                lines.append(self.indent_string(f"case {code}:{{ //{mnemonic} {tgtName} {srcName}\n"))
                #Inside the case staetment so indent
                
                
                #write whatever the case statement has determined will be the code in this particular instance
                #Case statement should be properly indented if it exists
                lines.append(caseStr)
                self.indentLevel += 1
                #Update the cycles, break out and close the case
                lines.append(self.indent_string(f'increment_timer(mem, {cycles});\n'))
                lines.append(self.indent_string('break;\n'))
                
                #closing case statement
                self.indentLevel -= 1
                lines.append(self.indent_string("}\n\n"))
                
                
            else:
                print(f'Skipping case {code} as nothing returned by case builder function')
                
        return lines
    
    def generic_case_builder(self, operations, prefixed=False):
        """
        This function creates a header file and a c file which will contain all of the case statements which we
        have defined for the class of operands specified.  This function handles boilerplate creation, and then defers
        creation of the actual cases to the create_cases function
        
        
        Will write to a directory called /src in the parent directory of the directory holding this script
        
        cboy -> src -> output
             \> scripts -> OpcodeGenerator.py
        
        """
        
        
        
        #File name decisions
        modifier = '' if not prefixed else 'prefixed_'
        fName = OpcodeReport.get_file_path(f'{modifier}opcodes.c')
        headerName =  OpcodeReport.get_file_path(f'{modifier}opcodes.h')
        
        self.cleanup(headerName)
        
        #Write the header file
        with open(headerName, 'w') as h:
            h.write(f'void interpret_{modifier}opcodes(CPU *cpu, unsigned char *mem, unsigned char opcode);\n')
        
        self.cleanup(fName)
            
        #Build the c file containing the function
        with open(fName, 'w') as f:
            
            self.indentLevel = 0
            lines = []
    
            lines.append(self.indent_string(f'void interpret_{modifier}opcodes(CPU *cpu, unsigned char *mem, unsigned char opcode){{\n'))
            
            self.indentLevel = 1
            
            lines.append(self.indent_string('switch(opcode){\n'))
            
            self.indentLevel = 2
            
            
            for mnemonic in operations:

                lines += self.create_cases(mnemonic)
                
                #print(lines)
            

            self.indentLevel = 1
            lines.append(self.indent_string('}\n'))
            
            self.indentLevel = 0
            lines.append(self.indent_string('}\n'))
            f.write("".join(lines))
            
    
    def build_case_files(self):
        """
        Wrapper for the generic case builder. For the GB there are two main classes of opcodes,
        prefixed and unprefixed. Here we specify which is which, and pass along to the case builder
        """
        self.generic_case_builder(['SET', 'RES', 'BIT', 'SWAP'], prefixed=True)
        self.generic_case_builder(['LD', 'INC', 'ADD', 'AND'])
                    
                
            
            
            
        
        
            
            
            
if __name__ == '__main__':
    g = OpcodeGenerator()
    g.build_case_files()
    
    r = OpcodeReport.ReportCreator()
    r.output_base_rpt('opcodes.csv')
    r.output_summary_rpt('opcodes_summary.csv')
    print("Reports created")
    
