from collections import Counter as count
import json
import mariadb
import os
import requests
import sys

class OpcodeGenerator:
    def __init__(self):
        self.cur = self.connect()
        self.indentLevel = 0;
        
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
        
        self.case_builder={
                            'SET' : self._build_case_set
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
            
            
    
    def get_ld_src(self, codeParams):
        srcString = 'unsigned char src = '
        
        #Immediate data is read from the stream
        src = codeParams['src']
        srcAction = codeParams['srcAction']
        
        if src == 'd8':
            srcString += 'get_byte(cpu, mem);\n'
        
        #registers need to be read from the cpu struct itself
        elif src in ['A','F','B','C','D','E','H','L']:
            srcString += f'cpu->{src.lower()};\n'
        
        #addresses are stored in one of 3 registers, and need to be accessed using the memory module    
        elif src in ['BC', 'DE', 'HL']:
            srcString += f'read_mem(mem, GET_{src}(cpu));\n'
        
        else:
            print("Error determining code for source variable")
            print(codeParams)
            sys.exit(-1)
            
        
        #Dealing with actions. Custom actions are only taken on the 16b registers, so 
        #we only need to consider actions on them
        actionString = ""
        if srcAction:
            actionString = self.indent_string(f'SET_{src}(cpu,src {srcAction} 1);\n')
            
        #Append the action string to the assignment string, and return it            
        return self.indent_string(srcString + actionString)
    
    def get_ld_tgt(self, codeParams):
        tgt = codeParams['tgt']
        tgtAction = codeParams['tgtAction']
        
        #Loading a value into a register
        if tgt in ['A','F','B','C','D','E','H','L']:
            tgtString = f'cpu->{tgt.lower()} = src;\n'
            
        #Loading a value int a memory address
        elif tgt in ['BC', 'DE', 'HL']:
            tgtString = f'write_mem(mem, GET_{tgt}(cpu), src);\n'
            
        #Dealing with actions. Same case as for the src variable. We only need to use the 16b macro to increment 
        actionString = ""
        if tgtAction:
            actionString = self.indent_string(f'SET_{tgt}(cpu, GET_{tgt}(cpu) {tgtAction} 1);\n')
        
        #action string is already properly indented, so we just need to add an indent to the start of the string
        return self.indent_string(tgtString + actionString)
        
    def create_cases(self, mnemonic, indentLevel = 2):
        """
        Creates the case statements for a given mnemonic.
        """
        self.indentLevel = indentLevel
        #If the file already exists, clean it up.
        fName = f'{mnemonic}_cases.txt'
        self.cleanup(fName)
        
        #Execute the query that corresponds to this mnemonic
        self.cur.execute(self.queries['mnemonicQuery'], (mnemonic,))
        results = self.cur.fetchall()
        
        for row in results:
            code = row[0]
            cycles = row[1]
            
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
            with open(fName, 'a') as f:
                print(f'Creating case for {code} {tgt[0]} {src[0]}')
                f.write(self.indent_string(f"case {code}:{{ //{code} {tgt[0]} {src[0]}\n"))
                #Inside the case staetment so indent
                self.indentLevel += 1
                
                #write whatever the case statement has determined will be the code in this particular instance
                caseStr = self.case_builder[mnemonic](tgt, src)
                f.write(caseStr)
                
                #Update the cycles, break out and close the case
                f.write(self.indent_string(f'increment_timer(mem, {cycles});\n'))
                f.write(self.indent_string('break;\n'))
                
                #closing case statement
                self.indentLevel -= 1
                f.write(self.indent_string("}\n\n"))
        
                
    def create_8b_load_cases(self, fName):
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
        #Would be nice if the data model had a view we could use for this instead of parsing the data in mem
        codeQuery = """select 
                    distinct code, cycles
                    from opcodes_v
                    where mnemonic='LD'
                    and bytes < 3
                    and code !='0xF8'
                    and code != '0xF9'
                    order by code, op_order ;
                    """
        operandQuery = """select 
                        operand_name,
                        op_immediate,
                        op_action
                        from opcodes_v
                        where code =?
                        and op_order =?;
        """
        #fName = 'ldCases.txt'
        self.indentLevel = 2
        
        #Get all the distinct 8b load operations
        self.cur.execute(codeQuery)
        results = self.cur.fetchall()
        
        #construct a dict where each key has the info needed to build a case.
        for row in results:
            code = row[0]
            cycles = row[1]
            
            self.cur.execute(operandQuery, (code, 1))
            tgt = self.cur.fetchone()
            self.cur.execute(operandQuery, (code, 2))
            src = self.cur.fetchone()

            
            codeParams = {
                    'src': src[0],
                    'srcIm': src[1],
                    'srcAction': src[2],
                    'tgt': tgt[0],
                    'tgtIm': tgt[1],
                    'tgtAction': tgt[2],
                    'cycles':cycles
                }
            
            with open(fName, 'a') as f:
                print(f'Creating case for {code} {tgt[0]} {src[0]}')
                f.write(self.indent_string(f"case {code}:{{ // {code} {tgt[0]} {src[0]}\n"))
                
                self.indentLevel += 1
                f.write(self.get_ld_src(codeParams))
                f.write(self.get_ld_tgt(codeParams))
                f.write(self.indent_string(f'increment_timer(mem, {cycles});\n'))
                f.write(self.indent_string('break;\n'))
                
                self.indentLevel -= 1
                f.write(self.indent_string("}\n\n"))
        
        print("Done writing 8b LD cases")

    
    def _build_case_set(self, tgt, src):
        """
        The set functions are taking a bit of some register, and setting it to 1. The first 'operand' (not really passed as one, but represented as
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
            command2 = self.indent_string(f'set_bit_char(&addr, {bit}, 1);\n')
            case = command1 + command2
        
        else:
            print("Unhandled case for SET operand")
            print(tgt)
            print(src)
            sys.exit(-11)
        
        return case
            
        
                
    def create_set_cases(self, fName):
        """
        The set functions are taking a bit of some register, and setting it to 1. The first 'operand' (not really passed as one, but represented as
        such in the dataset) is the bit, the second is the register. There's two main patterns
        
        bit -> register
        bit -> memory location

        """
        
        codeQuery = """
                    select distinct code,cycles 
                    from opcodes_v 
                    where mnemonic='SET';
                    """
                
        operandQuery = """
                        select * from opcodes_v
                        where code=?
                        and op_order=?;
                        """
        
        self.indentLevel = 2
        
        self.cur.execute(codeQuery)
        results = self.cur.fetchall()
        
        for row in results:
            code = row[0]
            cycles = row[1]
            
            self.cur.execute(operandQuery, (code, 1))
            tgt = self.cur.fetchone()
            self.cur.execute(operandQuery, (code, 2))
            src = self.cur.fetchone()
        
        
    
    def create_reset_cases(self, fName):
        pass           
            
            
            
if __name__ == '__main__':
    g = OpcodeGenerator()
    
    
    g.create_cases('SET')
    
