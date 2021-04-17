from collections import Counter as count
from functools import partial
import json
import mariadb
import os
import OpcodeCase
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
                            'AND' : partial(self._build_case_logical, '&'),
                            'OR'  : partial(self._build_case_logical, '|'),
                            'XOR' : partial(self._build_case_logical, '^'),
                            'RST' : self._build_case_rst,
                            'SRL' : partial(self._build_case_shift, 'srl'),
                            'SRA' : partial(self._build_case_shift, 'sra'),
                            'SLA' : partial(self._build_case_shift, 'sla'),
                            'RR'  : partial(self._build_case_rot, 'rr'),
                            'RRC' : partial(self._build_case_rot, 'rrc'),
                            'RL'  : partial(self._build_case_rot, 'rl'),
                            'RLC' : partial(self._build_case_rot, 'rlc'),
                            #RXXA cases are rotate cases specifically for the A register
                            #'RRA'  : partial(self._build_case_rot, 'rr'),
                            #'RRCA'  : partial(self._build_case_rot, 'rrc'),
                            #'RLA'  : partial(self._build_case_rot, 'rl'),
                            #'RLCA'  : partial(self._build_case_rot, 'rlc')
                            'CP'  : self._build_case_cp
                            }
        

        Strings to be used in the comments of each set of cases

        self.descriptions={
                            'SET' : "Bit setting operations",
                            'RES' : "Bit Resetting operations",
                            'LD'  : "Load operations",
                            'BIT' : "Bit compliment operations",
                            'SWAP': "Swap nibble operations",
                            'INC' : '8 / 16 bit increment operations',
                            'ADD' : '8 / 16 bit add operations',
                            'AND' : 'Bitwise AND operations',
                            'OR'  : 'Bitwise OR operations',
                            'XOR' : 'Bitwise XOR operations',
                            'RST' : 'RST perations',
                            'SRL' : 'SRL operations',
                            'SRA' : 'SRA operations',
                            'SLA' : 'SLA operations',
                            'RR'  : 'Rotate right operations',
                            'RRC' : 'Rotate right & carry operations',
                            'RL'  : 'Rotate left operations',
                            'RLC' : 'Rotate left & carry operations',
                            'RRA' : 'Rotate right operations for A register ',
                            'RRCA': 'Rotate right & carry operations for A register',
                            'RLA' : 'Rotate left operations for A register',
                            'RLCA': 'Rotate left & carry operations for A register',
                            'CP'  : 'Compare operations' 
                            
                            }
        
        """
    
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
    
    def disconnect(self):
        if self.cur:
            self.cur.close()
        if self.conn:
            self.conn.close()
    
    """
    Helper functions
    """
    
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
            return

                    
        
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
            return
        
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
            return
            
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
            return
        
        #Indent the lines of code in the case statement, and join them together into a single string    
        return self.prepare_case(case)
    
            

    
    def generic_case_builder(self, operations, prefixed=False):
        """
        This function creates a header file and a c file which will contain all of the case statements which we
        have defined for the class of operands specified.  This function handles boilerplate creation, and then defers
        creation of the actual cases to the create_cases function
        
        
        Will write to a directory called /src in the parent directory of the directory holding this script
        
        cboy -> src -> output
             \> scripts -> OpcodeGenerator.py
        
        """
        
        print("In Generic Case Builder")
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
            
            
            #When we actually start to create the cases, the indent level will be 2
            caseFactory = OpcodeCase.CaseFactory(2)
            for mnemonic in operations:
                
                print(mnemonic)
                self.indentLevel = 2
                
                #See if we have implemented the inside of the case statment before we make the case
                cases = caseFactory.get_cases(mnemonic)

                if cases:
                    lines += cases.create_cases()
                

            

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
        
        print("In Build Case Files")
        self.generic_case_builder(['SET', 'RES', 'BIT', 'SWAP', 'SRA', 'SRL', 'SLA', 'RR', 'RRC', 'RL', 'RLC'], prefixed=True)
        self.generic_case_builder(['LD', 'INC', 'ADD', 'AND', 'OR', 'XOR', 'RST', 'CP', 'JP', 'RET', 'JR'])
                    
                
            
            
            
        
        
            
            
            
if __name__ == '__main__':
    g = OpcodeGenerator()
    g.build_case_files()
    
    r = OpcodeReport.ReportCreator()
    r.output_base_rpt('opcodes.csv')
    r.output_summary_rpt('opcodes_summary.csv')
    print("Reports created")
    
