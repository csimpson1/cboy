from functools import partialmethod
import mariadb
import sys

class CaseFactory:
    
    def __init__(self, indentLevel):
        self.indentLevel = indentLevel
        
        self.mapping={
            'SWAP': SwapCase,
            'BIT' : BitCase,
            'INC' : IncCase,
            'ADD' : AddCase,
            'OR'  : OrCase,
            'AND' : AndCase,
            'XOR' : XorCase,
            'RST' : RstCase,
            'SRA' : SraCase,
            'SRL' : SrlCase,
            'SLA' : SlaCase,
            'RR'  : RrCase,
            'RRC' : RrcCase,
            'RL'  : RlCase,
            'RLC' : RlcCase,
            'CP'  : CpCase,
            'SET' : SetCase,
            'RES' : ResetCase,
            'JP'  : JumpCase,
            'RET' : RetCase
            
            }
        
    def get_cases(self, mnemonic):
        #case here is a constructor function. If it's implemented, pass the arguments necessary
        #for instantiation, and return an instance of the class specified by the mnemonic
        case = self.mapping.get(mnemonic)
        if case:
            return case(self.indentLevel)
    
    
"""
CASE CREATION METHODS
"""
class OpcodeCase:
    
    def __init__(self, mnemonic, prefixed, indentLevel):
        self.cur = None
        self.indentLevel = indentLevel;
        self.prefixed = prefixed
        
        """
        The mnemonic query must always return a code as the first element of a row in the result set. 
        Otherwise though, this query can be overwritten. One caveat is that the default cycles function 
        assumes that cycles will be the second element in a row in the result set.You can change 
        this in the query, but will then need to change the _create_cycles function as well
        """        
        self.queries = {
                        'mnemonicQuery' : """
                                            select distinct code,cycles 
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
        
        self.commentString = None
        self.mnemonic = mnemonic
        self.mnemonicProperties = None
        self.srcProperties =  None
        self.tgtProperties = None
        self.code = None #This will be populated after the mnemonic query is run in the _create_cases function
        self.srcName = None
        self.tgtName = None
        self.lines = []
        
    
    """
    Helper functions
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
        return cur
    
    def disconnect(self):
        if self.cur:
            self.cur.close()
        if self.conn:
            self.conn.close()

    def indent_string(self, string, indentLevel = None):
        
        """
        Return a string with a specified number of tabs appended to the start
        If no indent is specified in the function, default to to the indent
        level specified by the function. Otherwise, perform the custom indent
        """
        indent=''
        
        #Default value is really the one specified by this object
        if not indentLevel:
            indentLevel = self.indentLevel
        
        for i in range(indentLevel):
            indent += '\t'
            
        return indent + string
    
    
    def print_case_error(self, var, params):
            print(f'Error determining code for {var} variable')
            print(params)
            
    def prepare_case(self, acase):
        """
        Helper function for proper indentation of case statements
        """
        case = map(self.indent_string, acase)
        return ''.join(case)
    
    """
    Case building functions
    """
    
    
    def _create_cycles(self):
        lines = []
        if self.mnemonicProperties:
            cycles = self.mnemonicProperties[1]
            lines.append(f'increment_timer(mem, {cycles});\n')
            
        lines.append('break;\n')
        
        return lines 
    
    def create_cases(self):
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
        
        fn(self, [optional args], src, tgt,)
        where
        [optional args]: Not optional in the traditional python sense. These are positional arguments that are evaluated at the time
        the function is specified in the case_builder dict. 
        
        An example of this is the set/reset operations. These behave exactly the same, except for the value the bit is set to.
        Instead of duplicating the functions, the bit is specified using partial function evaluation, and the appropriate evaluation
        is specified for each mnemonic.
        
        src: A tuple consisting of operand_name, op_immediate, op_action of the source operand
        
        tgt: A tuple consisting of operand_name, op_immediate, op_action of the target operand
        

        """
        #Get the DB connection
        self.cur = self.connect()
        
        #Creating the comment to append each section of cases:
        self._set_comment_string()
        self._set_mnemonic_info()
        
        
        for row in self.mnemonicProperties:
            self.mnemonicProperties = row
            self.code = row[0]
            self.srcProperties = None
            self.tgtProperties = None

            self._set_src_tgt_info()
            
            #First see if we can generate a case for this code. 

            self.indentLevel += 1
            caseStr = self._case_builder()
            self.indentLevel -= 1
            
            #prefixed opcodes will be dealt with in a separate function, it's assumed we've already ready the first CB byte
            #so strip it. However, the prefix is present in the DB, so we need to strip it after the case has been created
            if '0xCB' in self.code:
                self.code = self.code[0:2] + self.code[4:]
            
            
            if caseStr:
                self._set_case_contents(caseStr)
                
            else:
                print(f'Skipping case {self.code} as nothing returned by case builder function')
        
        #Close the connection and return the case statement
        self.disconnect()
        return ''.join(self.lines)
                
    
    def _set_mnemonic_info(self):
        self.cur.execute(self.queries['mnemonicQuery'], (self.mnemonic,))
        self.mnemonicProperties = self.cur.fetchall()
        
            
           
    def _set_src_tgt_info(self):
        """
        Handles the setting of src and tgt properties for a given opcode. If you want to modify how these properties are set, for example
        only getting the src properties, overwrite this function in a subclass
        """
        
        #1 specifies the target of an operation, 2 the source. Of course, this doesn't necessarily hold for all operations
        #but it's common enough that I called the variables this
        self.cur.execute(self.queries['operandQuery'], (self.code, 1))
        self.tgtProperties = self.cur.fetchone()
        self.cur.execute(self.queries['operandQuery'], (self.code, 2))
        self.srcProperties = self.cur.fetchone()
        
        self.srcName = self.srcProperties[0] if self.srcProperties is not None else ''
        self.tgtName = self.tgtProperties[0] if self.tgtProperties is not None else ''
        
    def _set_comment_string(self):
        comment = []
        commentString = f'  {self.mnemonic}: {self.commentString}\n'
        dots = "*" * (len(commentString) + 1)
        
        comment.append('/' + dots + '\n')
        comment.append(commentString)
        comment.append(dots +  '/' + '\n')
        
        self.lines += list(map(self.indent_string, comment))
        
    def _set_case_contents(self, caseStr):
        

        
        print(f'Creating case for {self.code} {self.tgtName} {self.srcName}')
        
        self.lines.append(self.indent_string(f"case {self.code}:{{ //{self.mnemonic} {self.tgtName} {self.srcName}\n"))
        self.lines.append(caseStr)
        self.lines.append(self.indent_string("}\n\n"))
        
    def _case_builder(self):
        
        """
        Wrapper for the mnemonic specific implementation method. Handles which functions are called in creating the case statement.
        If you want to call a different function based on some parameter, or change how the cycles are set, override this function
        in your child class
        """
        
        case = []
        
        case += self._create_case()
        case += self._create_cycles()
        
        return self.prepare_case(case)
            
    def _create_case(self):
        """
        Where the contents of a case statement are actually generated. This needs to be overwritten in every subclass
        """
        pass
    
class SwapCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('SWAP', True, indentLevel)
        self.commentString = "Swap nibble operations"
        
    def _create_case(self):
        case = []
        
        
        if self.tgtName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char toSwap = cpu->{self.tgtName.lower()};\n')
            case.append ('unsigned char swapped = swap_nibble(toSwap);\n')
            case.append(f'cpu->{self.tgtName.lower()} = swapped;\n')
        
        elif self.tgtName =='HL':
            case.append('unsigned char toSwap = read_mem(mem, GET_HL(cpu));\n')
            case.append('unsigned char swapped = swap_nibble(toSwap);\n')
            case.append('write_mem(mem, GET_HL(cpu), swapped;\n')
        
        else:
            self.print_case_error('tgt',self.tgtProperties)
            return
        
        return case
    
class BitCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('BIT', True, indentLevel)
        self.commentString = "Bit compliment operations"
        
    def _create_case(self):
        """
        Creates case statements for the BIT operation. This operation takes the compliment of a given bit from a register, and stores that value into the Z flag of the
        cpu. The src registers can be any of the 8 bit registers in the CPU, or from the value of an address pointed to by the HL register
        """
        case = []
        #srcName = src[0]
        #tgtName = tgt[0]
        
        
        #Get registers directly from the CPU
        if self.srcName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char bit = get_bit((int) cpu -> {self.srcName.lower()}, {self.tgtName});\n')
        
        #Memory addresses are read from mem    
        elif self.srcName == 'HL':
            case.append(f'unsigned char bit = get_bit((int) read_mem(cpu, GET_HL(CPU), {self.tgtName}));\n')
            
        else:
            self.print_case_error('src', self.srcProperties)
            return
            
        #Just checking the tgt name, to make sure it is a valid bit position    
        if self.tgtName in [f'{i}' for i in range(8)]:
            pass
        else:
            self.print_case_error('tgt', self.tgtProperties)
            return
        
            
            
        case.append('unsigned char compliment = (bit == 0 : 1 : 0);\n')
        case.append('SET_ZF(cpu, compliment);\n')
        
        
        return case

class IncCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('INC', False, indentLevel)
        self.commentString = '8 / 16 bit increment operations'
        
    def _create_case(self):
        """
        The increment operation adds 1 to a target register. There's two flavors:
        1) We're incrementing an 8 bit register, either in the CPU, or in memory
        2) We're incrementing a 16 bit register in the CPU
        
        The 16b operation is simpler, it sets no flags. Flags across all 8bit operations 
        are the same as well, so this is done in a helper function
        """
        

        tgtImmediate = self.tgtProperties[1]
        case = []
        
        #We're incrementing an 8 bit registers
        if self.tgtName in ['A', 'B', 'C', 'D', 'E', 'H', 'L']:
            case.append(f'unsigned char toInc = cpu -> {self.tgtName.lower()};\n')
            case.append('unsigned char incremented = toInc + 1;\n')
            case.append(f'cpu -> {self.tgtName.lower()};\n')
            self._build_flags_inc(case)
        
        #Incrementing an 8 bit register in memory    
        elif self.tgtName =='HL' and tgtImmediate == 0:
            case.append(f'unsigned char toInc = read_mem(mem, GET_HL(cpu));\n')
            case.append('unsigned char incremented = toInc + 1;\n')
            case.append(f'write_mem(mem, GET_HL(cpu), incremented);\n')
            self._build_flags_inc(case)
            
        elif self.tgtName in ['BC', 'DE', 'HL', 'SP']:
            case.append(f'unsigned short toInc = GET_{self.tgtName}(cpu);\n')
            case.append('unsigned short incremented = toInc + 1;\n')
            case.append(f'SET_{self.tgtName}(cpu, incremented);\n')
        else:
            self.print_case_error('tgt', self.tgtProperties)
            return
            
        
        return case
            
    def _build_flags_inc(self, case):
        case.append('SET_ZF(cpu, (incremented == 0));\n')
        case.append('SET_NF(cpu, 0);\n')
        case.append('SET_HF(cpu, HC_CHECK(toInc, 0x1));\n')
        
class AddCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('ADD', False, indentLevel)
        self.commentString = '8 / 16 bit add operations'
    
    def _create_case(self):
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
        
   
        srcImm = self.srcProperties[1]
        
        case = []
        
        #8 bit add
        if self.srcName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L'] or (self.srcName == 'HL' and srcImm == 0) or (self.srcName == 'd8' and self.tgtName == 'A'):
            #Reading data from a memory address
            if self.srcName == 'HL' and srcImm == 0:
                case.append('unsigned char src = read_mem(mem, GET_HL(cpu));\n')
            
            #Reading immediate data from the stream
            elif(self.srcName == 'd8' and self.tgtName == 'A'):
                case.append('unsigned char src = get_byte(cpu, mem);\n')
            
            #Reading data from a register    
            else:
                case.append(f'unsigned char src = cpu -> {self.srcName.lower()};\n')
                
            case.append(f'unsigned char tgt = cpu -> {self.tgtName.lower()};\n')
            case.append(f'unsigned char result = src + tgt;\n')
            case.append(f'cpu->{self.tgtName.lower()} = result;\n')
            
            #Set flags
            case.append('SET_ZF(cpu, (result == 0));\n')
            case.append('SET_NF(cpu, 0);\n')
            case.append('SET_HF(cpu, HC_CHECK(src, tgt));\n')
            case.append('SET_CF(cpu, C_CHECK(src, tgt, result));\n')
            
        #16 bit add    
        elif self.srcName in ['BC', 'DE', 'EF', 'HL', 'SP']:
            case.append(f'unsigned short src = GET_{self.srcName}(cpu);\n')
            case.append(f'unsigned short tgt = GET_{self.tgtName}(cpu);\n')
            case.append('unsigned short result = src + tgt;\n')
            case.append(f'SET_{self.tgtName}(cpu, result);\n')
            
            #Set flags
            case.append('SET_NF(cpu,0);\n')
            #Checking if a carry occurs from bit 11 to 12
            case.append('SET_HF(cpu, HC_CHECK_16B_ADD(src, tgt));\n')
            case.append('SET_CF(cpu, C_CHECK(src, tgt, result));\n')
            
        #Signed addition to the stack pointer
        elif self.srcName == 'r8':
            case.append('unsigned char src = get_byte(cpu, mem);\n')
            case.append('int  srcInt = (int) src;\n')
            case.append(f'unsigned short tgt = GET_{self.tgtName}(cpu);\n')
            case.append('int tgtInt = (int) tgt;\n')
            #??
            case.append('unsigned short result = (unsigned short)(srcInt + tgtInt);\n')
            case.append(f'SET_{self.tgtName}(cpu, result);\n')
            
            #set flags
            #??
            case.append('SET_HF(cpu, HC_CHECK(src, tgt);\n')
            #This needs to be tested with a test ROM, not sure on the behavior here
            case.append('SET_CF(cpu, src, tgt, result);\n')
        
        else:
            self.print_case_error('src', self.srcProperties)
            return
            
        
        
        return case
  
class LogicalCase(OpcodeCase):
    
    """
    LogicalCase is not meant to be called directly by the OpcodeGenerator. Instead, it acts as a parent class
    for the classes corresponding to the OR/XOR/AND operations. The logical operator is filled in in the child
    classes
    """
    def __init__(self, mnemonic, indentLevel):
        super().__init__(mnemonic, False, indentLevel)
    
    def _create_case(self, operand):
        """
        The  operation takes the bitwise logical operation * of some 8b value and the accumulator,
        and stores that value in the accumulator. 
        
         REG  * A -> A
        (REG) * A -> A
          d8  * A -> A
        """
        case = []
        
        if self.tgtName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L']:
            case.append(f'unsigned char tgt = cpu -> {self.tgtName.lower()};\n')
        
        elif self.tgtName == 'HL':
            case.append('unsigned char tgt = read_mem(mem, GET_HL(cpu);\n')
        
        elif self.tgtName == 'd8':
            case.append('unsigned char tgt = get_byte(cpu, mem);\n')
        
        else:
            self.print_case_error('src', self.srcProperties)
            return
            
        case.append(f'cpu -> a = cpu -> a {operand} tgt;\n')
        
        #flags
        case.append('SET_ZF(cpu, (cpu -> a == 0));\n')
        case.append('SET_NF(cpu, 0);\n')
        case.append('SET_HF(cpu, 1);\n') if operand == '&' else case.append('SET_HF(cpu, 0);\n')
        case.append('SET_CF(cpu, 0);\n')

        
        
        return case

class OrCase(LogicalCase):
    
    def __init__(self, indentLevel):
        super().__init__('OR', indentLevel)
        self.commentString = 'Bitwise OR operations'
        #Override the create cases function using partial function evaluation to supply the bitwise or operator |
    
    def _create_case(self):
        return super()._create_case('|')

class XorCase(LogicalCase):
    
    def __init__(self, indentLevel):
        super().__init__('XOR', indentLevel)
        self.commentString = 'Bitwise XOR operations'
        #Override the create cases function using partial function evaluation to supply the bitwise or operator |
    
    def _create_case(self):
        return super()._create_case('^')    

class AndCase(LogicalCase):

    def __init__(self, indentLevel):
        super().__init__('AND', indentLevel)
        self.commentString = 'Bitwise AND operations'
        #Override the create cases function using partial function evaluation to supply the bitwise or operator |
    
    def _create_case(self):
        return super()._create_case('&') 
    
class RstCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('RST', False, indentLevel)
        self.commentString = 'Bit Resetting operations'
    
    def _create_case(self):
        """
        The reset operation pushes the current value of the SP onto the stack and
        then jumps to an RST location in memory.
        
        The push to the stack works like this. First, SP is decremented by one, and
        the high order byte of the PC is loaded to the location specified by the SP. SP is
        decremented by 1 again, and this time the low order byte of the PC is stored.
        
        Depending on the operand, we'll load the PC with one of 8 values
        
        """
        
        #The target is formatted as something like 10H, H signifying hex
        #Just leave that part off, the number is what's important
        tgtName = self.tgtName[0][0:2]
        case = []
        #Push the PC to the stack
        case.append('cpu -> sp = cpu -> cp - 1;\n')
        case.append('unsigned char highByte = get_high_byte(&(cpu->pc));\n')
        case.append('write_mem(mem, cpu->sp, highByte);\n')
        case.append('cpu -> sp = cpu -> cp - 1;\n')
        case.append('unsigned char lowByte = get_low_byte(&(cpu->pc));\n')
        case.append('write_mem(mem, cpu->sp, lowByte);\n')
        
        #Now load the jump vector into the PC
        case.append('set_high_byte(&(pc -> sp), (char) 0x00);\n')
        case.append(f'set_low_byte(&(pc -> sp), (char) 0x{tgtName});\n')
        
        
        return case

class ShiftCase(OpcodeCase):
    """
    ShiftCase is not meant to be created directly by the OpcodeGenerator. Instead, it acts as a parent class for classes
    which correspond to specific shift operations. In the C codebase we are generating cases for, there are specific functions that 
    take care of the different shifts. The child cases specify these functions.
    """
    
    def __init__(self, mnemonic, indentLevel):
        super().__init__(mnemonic, True, indentLevel)
        
    def _create_case(self, op):
        """
        The SLA, SRA, and SRR operations shift a register either to the left or right. The operations
        ending in A do not change the terminal bit ie
        
        SRA 10101010 -> 11010101, CY => 0
        SRL 10101010 -> 01010101, CY => 0
        
        These commands can either be applied to the 8b registers in the CPU, or to a register in memory
        
        """
        
        case = []
        
        if self.tgtName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L']:
            case.append(f'unsigned char toShift = cpu -> {self.tgtName.lower()};\n')
            case.append(f'cpu -> {self.tgtName.lower()} = shifted;\n')
        
        elif self.tgtName == 'HL':
            case.append(f'unsigned char toShift = read_mem(mem, GET_HL(cpu));\n')
            case.append('write_mem(mem, GET_HL(cpu), shifted);\n')
            
        else:
            self.print_case_error('tgt', self.tgtProperties)
            return
        
        #This line is common to both cases. Rather than create a new if/else,
        #handle all lines with a tgt dependency first, and then insert this one
        #which is common
        case.insert(1,f'unsigned char shifted = {op}(toShift, cpu);\n')
        
        #Flags. 
        #No need to take care of the flag operations which are dependent on a 
        #specific bit. These are handled by the respective operation
        case.append('SET_ZF(cpu, (shifted == 0));\n')
        
        
        return case

class SraCase(ShiftCase):
    
    def __init__(self, indentLevel):
        super().__init__('SRA', indentLevel)
        self.commentString = 'SRA Shift operations'
        
    def _create_case(self):
        return super()._create_case('sra')

class SrlCase(ShiftCase):
    
    def __init__(self, indentLevel):
        super().__init__('SRL', indentLevel)
        self.commentString = 'SRL Shift operations'
        
    def _create_case(self):
        return super()._create_case('srl')

class SlaCase(ShiftCase):
    
    def __init__(self, indentLevel):
        super().__init__('SLA', indentLevel)
        self.commentString = 'SLA Shift operations'
        
    def _create_case(self):
        return super()._create_case('sla')
    
class RotCase(OpcodeCase):
    def __init__(self, mnemonic, indentLevel):
        super().__init__(mnemonic, True, indentLevel)
        
    def _create_case(self, op):
        """
        The rotate operations act like a shift operation on a circular structure. Like the operation 
        x -> x +- 1 mod(8). Operations that are suffixed with c store the value shifted out of the 
        register and back in in the C flag. Those that are not do not have this behavior. 
        
        There are two sorts of targets, either a register in the CPU, or a register specified by a 
        memory address
        """
        
        case=[]
        """
        This dictionary contains the actual names of the functions that are used in the C code. 
        Names that are not the mnemonics are used to improve the readability of the code
        """
        opTranslate={
                        'rr': "rot_right",
                        'rl': "rot_left",
                        'rlc': "rot_right_carry",
                        'rrc': "rot_left_carry"
            }
        

        if self.tgtName in ['A', 'B', 'C', 'D', 'E', 'F', 'H', 'L']:
            
            case.append(f'unsigned char toRotate = {opTranslate[op]}(cpu -> {self.tgtName.lower()}, cpu);\n')
            case.append(f'cpu -> {self.tgtName.lower()} = toRotate;\n')
            
        elif self.tgtName == 'HL':
            case.append('unsigned short address = GET_HL(cpu);\n')
            case.append(f'unsigned char toRotate = read_mem(cpu, address);\n')
            case.append(f'write_mem(cpu, address);\n')
        
        else:
            self.print_case_error('tgt', self.tgtProperties)
            return
        
        #Set the Z flag
        case.append("SET_ZF(cpu, (toRotate == 0));\n")
        
        
        return case
    
class RrCase(RotCase):
    
    def __init__(self, indentLevel):
        super().__init__('RR', indentLevel)
        commentString = 'Rotate Right operations'
        
    def _create_case(self):
        return super()._create_case('rr')

class RrcCase(RotCase):

    def __init__(self, indentLevel):
        super().__init__('RRC', indentLevel)
        commentString = 'Rotate Right & Carry operations'
        
    def _create_case(self):
        return super()._create_case('rrc')

class RlCase(RotCase):

    def __init__(self, indentLevel):
        super().__init__('RL', indentLevel)
        commentString = 'Rotate Left operations'
        
    def _create_case(self):
        return super()._create_case('rl')

class RlcCase(RotCase):
    
    def __init__(self, indentLevel):
        super().__init__('RLC', indentLevel)
        commentString = 'Rotate Left & Carry operations'
        
    def _create_case(self):
        return super()._create_case('rlc')

class CpCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('CP', False, indentLevel)
        commentString = 'Compare operations'
        
    
    def _create_case(self):
        """
        The cp or compare operation checks to see if a register, immediate, or value at a memory address
        is equal to the value in A. If so, it sets the zero flag (think:subtracting the value of A and whatever).
        The half carry  and carry flags are set as if a subtraction was done, but the value of A is not changed.
        """
        
        case=[]
        
        if self.tgtName in ['B', 'C', 'D', 'E', 'F', 'H', 'L']:
            case.append(f'unsigned char subtrahend = cpu -> {self.tgtName.lower()};\n')
            
        elif self.tgtName == 'A':
            #No need to actually do the subtraction here, A-A is always 0. 
            return self._get_flags_cp_a(case)
        
        elif self.tgtName == 'HL':
            case.append('unsigned char subtrahend = read_mem(mem, GET_HL(cpu));\n')
        
        elif self.tgtName == 'd8':
            case.append('unsigned char subtrahend = get_byte(cpu, mem);\n')
        
        case.append('unsigned char result = cpu -> a - subtrahend;\n')    
        #Set the flags
        case.append("SET_ZF(cpu, (result == 0));\n")
        case.append("SET_NF(cpu, 1);\n")
        case.append("SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), subtrahend));\n")
        case.append("SET_CF(cpu, C_CHECK_SUB((cpu->a), subtrahend, result));\n")
        
        
        return case
    
    def _get_flags_cp_a(self, case):
        # We know the result will be 0, and the subtrahend is A so setting flags is easy
        case.append("SET_ZF(cpu, 0);\n")
        case.append("SET_NF(cpu, 1);\n")
        case.append("SET_HF(cpu, HC_CHECK_8B_SUB((cpu->a), (cpu->a)));\n")
        case.append("SET_CF(cpu, 0);\n")
        
        
        return case

class SetResetCase(OpcodeCase):
    """
    The SetResetCase is not meant to be instantiated by the CaseFactory directly. Instead it splits into
    two subclasses, SetCase and ResetCase, depending on if we change a given bit to 1 or 0 respectively.
    """
    
    def __init__(self, mnemonic, indentLevel):
        super().__init__(mnemonic, True, indentLevel)
        
    
    def _create_case(self, val):
        """
        The set/ reset functions are taking a bit of some register, and setting it to 1 or 0. The first 'operand' (not really passed as one, but represented as
        such in the dataset) is the bit, the second is the register. There's two main patterns
        
        bit -> register
        bit -> memory location

        """
        
        bit = self.tgtProperties[0]
        reg = self.srcProperties[0]
        
        
        case = []
        
        if reg in ['A','F','B','C','D','E','H','L']:
            case.append(f"set_bit_char(&(cpu->{reg.lower()}), {bit}, 1);\n")
            
        elif reg == "HL":
            case.append("unsigned char addr = read_mem(mem, GET_HL(cpu));\n")
            case.append(f'set_bit_char(&addr, {bit}, {val});\n')
        
        else:
            self.print_case_error("src and tgt", self.srcProperties)
            print(self.tgtProperties)
            return
        
        return case
    
class SetCase(SetResetCase):
    
    def __init__(self, indentLevel):
        super().__init__('SET', indentLevel)
        self.commentString = 'Bit Set operations'
        
    def _create_case(self):
        return super()._create_case(1)
    
class ResetCase(SetResetCase):
    
    def __init__(self, indentLevel):
        super().__init__('RES', indentLevel)
        self.commentString = 'Bit Reset Operations'
        
    def _create_case(self):
        return super()._create_case(0)      
"""
class LdCase(OpcodeCase):
    def __init__(self, indentLevel):
        super().__init__('LD', False, indentLevel)
        self.commentString = 'LD operations'
"""

class JumpCase(OpcodeCase):
    
    def __init__(self, indentLevel):

        super().__init__('JP', False, indentLevel)
        self.commentString = "Jump Operations"
        
        self.queries['mnemonicQuery'] = """
                                        select distinct code,cycles,conditional_cycles 
                                        from opcodes_v 
                                        where mnemonic=?; 
                                        """
        
    def _case_builder(self):
        """ 
        Overrides the _case_builder function from the OpcodeCase class. Flags are set conditionally, so will
        be set as a part of the _create_case() function. 
        """
        case = []
        case += self._create_case()
        
        return self.prepare_case(case)
    
    def _create_case(self):
        """
        The JP or jump command loads a value into the PC if a certain condition is met (one operation is
        actually an unconditional jump). If the condition fails, then the next operation following the JP
        instruction is executed.
        
        One of the more interesting cases, as we need to consider that the cycles are different 
        here depending on if the JP is executed or not.
        """
        
        case = []
        
        if self.tgtName == 'a16':
            case += self._get_address()
            case.append('SET_PC(cpu, address);\n')
            case += self._create_cycles()
            return case
        
        elif self.tgtName == 'HL':
            case.append('SET_PC(cpu, GET_HL(cpu));\n')
            case += self._create_cycles()
            return case
            
        elif self.tgtName in ['NZ', 'Z', 'NC', 'C']:
            case += self._get_address()
            case += self._jp_conditional()
            return case
            
        
        else:
            self.print_case_error("src and tgt", self.srcProperties)
            print(self.tgtProperties)
            return
        
    def _get_address(self):
        statements = []
        statements.append("unsigned char lowByte = get_byte(cpu, mem);\n")
        statements.append("unsigned char highByte = get_byte(cpu, mem);\n")
        statements.append("unsigned short address = _get_8b_to_16b(&highByte, &lowByte);\n")
        return statements
    
    def _jp_conditional(self):
        
        case = []
        conditionalCycles = self.mnemonicProperties[2]
        cycles = self.mnemonicProperties[1]
        
        if 'N' in self.tgtName:
            #We need to check to see if the corresponding flag is 0
            flag = self.tgtName[1:]
            case.append(f'if GET_{flag}F(cpu) == 0:{{\n')
            
        else:
            flag = self.tgtName[0:1]
            case.append(f'if GET_{flag}F(cpu) == 1:{{\n')
            
        self.indentLevel += 1
        
        case.append(self.indent_string('SET_PC(cpu, address);\n'))
        case.append(self.indent_string(f'increment_timer(mem, {conditionalCycles});\n'))
        
        self.indentLevel -= 1
        case.append('}\n\n')
        case.append('else {\n')
        
        self.indentLevel += 1
        
        case.append(self.indent_string(f'increment_timer(mem, {cycles});\n'))
        
        self.indentLevel -= 1
        case.append('}\n\n')
        case.append('break;\n')
        
        
        return case
        
class RetCase(OpcodeCase):
    
    def __init__(self, indentLevel):
        super().__init__('RET', False, indentLevel)
        self.commentString = "Return Operations"
        self.queries['mnemonicQuery'] = """
                                select distinct code,cycles,conditional_cycles 
                                from opcodes_v 
                                where mnemonic=?; 
                                """
    
    def _case_builder(self):
        
        """
        Override the method in Opcode case to skip the cycle creation method. This will be handled in the create_case method.
        Wrapper for the mnemonic specific implementation method. Handles which functions are called in creating the case statement.
        If you want to call a different function based on some parameter, or change how the cycles are set, override this function
        in your child class
        """
        
        case = []
        
        case += self._create_case()
        
        return self.prepare_case(case)
        
        
    def _create_case(self):
        """
        The RET functions check for the value of a given flag, and pop the previous value of the program counter
        off of the stack if the flag condition is true. The exact condition will be loaded into the tgtName
        """
        case = []
        
        """
        #Get the address tp return to
        case.append('unsigned char lowByte = read_mem(mem, cpu->sp);\n')
        case.append('SET_SP(cpu, (GET_SP(cpu) + 1));\n')
        case.append('unsigned char highByte = read_mem(mem, cpu->sp);\n')
        case.append('SET_SP(cpu, (GET_SP(cpu) + 1));\n')
        case.append('unsigned short address = get_8b_to_16b(highByte, lowByte);\n')
        """
        
        if self.tgtName in ['Z', 'NZ', 'C', 'NC']:
            case += self._create_conditional()
            return case
            
        elif not self.tgtName:
            #We have an unconditional return
            case += self._get_address_from_stack()
            case += self._load_address_set_cycles()
            case.append('break;\n')
            return case
        
        else:
            self.print_case_error("tgt", self.tgtProperties)
            return                        
    
    def _create_conditional(self):
        
        #If our tgtName starts with N, we are checking to see if a flag is 0 and will action accordingly.
        #Otherwise, our tgtName specifies that we are checking to see if a flag is set to 1
        

        case = []
        
        if self.tgtName[0] == 'N':
            value = '0'
            flag = self.tgtName[1]
        
        else:
            value = '1'
            flag = self.tgtName[0]
        
        case.append(f'if(GET_{flag}F(cpu) == {value}){{\n')
        
        #We want to indent the inside of the IF block one tab further than the rest of the case
        case += map(lambda string: self.indent_string(string,indentLevel=1), self._get_address_from_stack())
        case += map(lambda string: self.indent_string(string,indentLevel=1), self._load_address_set_cycles())
        
        
        case.append('}\n')
        case.append('else{\n')
        
        #Again, want to set off the default case
        case += map(lambda string: self.indent_string(string,indentLevel=1), self._dont_load_address_set_cycles())
        
        case.append('}\n')
        case.append('break;\n')
        
        return case
    
    def _get_address_from_stack(self):
        
        """Create the portion of the case statement to load the address from the stack"""  
        
        case = []
        
        case.append('unsigned char lowByte = read_mem(mem, cpu->sp);\n')
        case.append('SET_SP(cpu, (GET_SP(cpu) + 1));\n')
        case.append('unsigned char highByte = read_mem(mem, cpu->sp);\n')
        case.append('SET_SP(cpu, (GET_SP(cpu) + 1));\n')
        case.append('unsigned short address = get_8b_to_16b(highByte, lowByte);\n')
        
        return case
             
    def _load_address_set_cycles(self):
        
        """
        Create the subcase statement that corresponds to loading the address into the PC
        register, and set the cycles accordingly
        """
        
        case = []
        conditionalCycles = self.mnemonicProperties[1]
        
        case.append('SET_PC(cpu, address);\n')
        case.append(f'increment_timer(mem, {conditionalCycles});\n')
        
        return case  
        
    def _dont_load_address_set_cycles(self):
        
        case = []
        cycles = self.mnemonicProperties[2]
        case.append(f'increment_timer(mem, {cycles});\n')
        
        return case
        
if __name__ == '__main__':
    case = SwapCase(2)
    lines = case.create_cases()
    print(lines)
    
    print()
    
    case = BitCase(2) 
    lines = case.create_cases()
    print(lines)
    
    case = OrCase(2)
    lines = case.create_cases()
    print(lines)      
        