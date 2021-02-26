from collections import Counter as count
import json
import mariadb
import requests
import sys


class DBPopulater:
    
    def __init__(self):
        cmdRefUrl='https://gbdev.io/gb-opcodes/Opcodes.json'
        refPage = requests.get(cmdRefUrl)
        #print(refPage.text)
        self.opcodes = json.loads(refPage.text)
        self.cur = self.connect()
        
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
        print("connected!")
        return cur
    
    def cleanup(self):
        self.conn.close()
    
    def populate_operation(self):
        #Get the unique operations from the set of opcodes and populate the operation table
        
        uniqueRows = []
        
        for type in ['unprefixed', 'cbprefixed']:
            for code in self.opcodes[type]:
                #print(self.opcodes[type])
                opcode = self.opcodes[type][code]
                row = [opcode['mnemonic'], opcode['flags']['Z'], opcode['flags']['N'], opcode['flags']['H'], opcode['flags']['C']]
                row = tuple(['' if x == '-' else x for x in row])

                if row not in uniqueRows:
                    uniqueRows.append(row)
                    
        print('finished getting operations') 
        print('inserting values into operation table')
        self.cur.executemany("insert into operation (mnemonic, zero_flag, subtract_flag, half_carry_flag, carry_flag) values (?, ?, ?, ?, ?)", uniqueRows)
        self.conn.commit()
        print('done inserting')
        
        
    def get_operands(self):
        
        uniqueOperands = []
        
        for type in ['unprefixed', 'cbprefixed']:
            for code in self.opcodes[type]:
                for operand in self.opcodes[type][code]['operands']:
                    name = operand['name']
                    print(operand)
                    if 'bytes' in operand:
                        bytes = operand['bytes']
                
                    else:
                        bytes = 0
                        
                    
                    row = (name, bytes)
                
                    if row not in uniqueOperands:
                        uniqueOperands.append(row)
                
        for row in uniqueOperands:
            print(row)
                         
        print('finished getting operands')
        print('inserting values into operand table')
        self.cur.executemany('insert into operand (name, size) values (?, ?)', uniqueOperands)
        self.conn.commit()
        print('done inserting')
        


if __name__ == '__main__':
    codes = DBPopulater()
    #codes.populate_operation()
    codes.get_operands()
    codes.cleanup()
    