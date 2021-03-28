from collections import Counter as count
import csv
import json
import mariadb
import os
import re
import requests
import sys

class ReportCreator:
    
    def __init__(self):
        self.cur = self.connect()
        self.implemented_opcodes = self.find_implemented_opcodes()
        self.total_opcodes = self.find_all_opcodes()
    
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
    
    def find_all_opcodes(self):
        query = """
                select distinct code, mnemonic
                from opcodes_v;
                """
        self.cur.execute(query)
        #return [x[0] for x in self.cur.fetchall()]
        return self.cur.fetchall()
        
        
    def find_implemented_opcodes(self):
        #Get the path to the cpu.c file. This assumes that we have the following directory structure
        """
        
        base - src - cpu.c
            \
             - scripts - OpcodeReport.py
        """
        
        currentDir = os.path.dirname(__file__)
        parentDir = os.path.split(currentDir)[0]
        
        
        fPath = os.path.join(parentDir, 'src', 'cpu.c')
        
        p = re.compile('case (0x\d\d)')
        cases = []
        
        with open(fPath, 'r') as f:
            for line in f:
                results = p.findall(line)
                if results:
                    cases.append(results[0])
        
        return cases
    
    def output_base_rpt(self, fName):
        with open(fName, 'w', newline='') as f:
            reportWriter = csv.writer(f, )
            reportWriter.writerow(['Opcode', 'Mnemonic', 'Implemented'])
            for code in self.total_opcodes:
                
                implemented = 0
                if code[0] in self.implemented_opcodes:
                    implemented = 1
                
                reportWriter.writerow([code[0], code[1], implemented])
                
    def output_summary_rpt(self, fName):
        
        summary = {}
        for (code,mnemonic) in self.total_opcodes:
            print(code)
            print(mnemonic)
            
            if mnemonic not in summary:
                #Add the mnemonic in
                summary[mnemonic] = {'total':1, 'implemented':0 }
                
            else:
                #We have seen the code so update the number of times we've seen it
                summary[mnemonic]['total'] += 1
            
            #Now see if that particular opcode is in the lists of one found to be implemented. If so, add 1 to the count
            if code in self.implemented_opcodes:
                summary[mnemonic]['implemented'] += 1
        
        with open(fName, 'w', newline='') as f:
            sReportWriter = csv.writer(f)
            sReportWriter.writerow(['Mnemonic', 'Implemented', 'Total', 'Percentage'])
            
            for mnemonic in summary:
                print
                sReportWriter.writerow([mnemonic, summary[mnemonic]['implemented'], summary[mnemonic]['total'], (summary[mnemonic]['implemented']/ summary[mnemonic]['total']) * 100 ])
                
        
                
            
            
        
                    
        
if __name__ == '__main__':
    r = ReportCreator()
    
    """
    p = re.compile(r"case (0x\d\d)")
    line="        case 0x16:{ // LD D, d8"
    print(p.findall(line))
    """
    
    print(r.implemented_opcodes)
    print(r.total_opcodes)
    r.output_base_rpt('opcodes.csv')
    r.output_summary_rpt('opcodes_summary.csv')
        