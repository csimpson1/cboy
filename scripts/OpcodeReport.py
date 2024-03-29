from collections import Counter as count
import csv
import json
import mariadb
import os
import re
import requests
import sys


def get_file_path(fName):
    """
    Assumes that we have the following directory structure 
    
    cboy -> src -> fName
         \> scripts -> OpcodeReport
    """
    currentDir = os.path.dirname(__file__)
    parentDir = os.path.split(currentDir)[0]
    fPath = os.path.join(parentDir, 'src', fName)
    
    return fPath


class ReportCreator:
    
    def __init__(self):
        self.cur = self.connect()
        self.implementedOpcodes = []
        #Getting the path for the opcode files
        self.find_implemented_opcodes(r'void interpret_opcode', get_file_path('opcodes.c'))
        self.find_implemented_opcodes(r'void interpret_prefixed_opcode', get_file_path('prefixed_opcodes.c'))
        
        
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
        print("Connected to DB")
        return cur
    
    
    def find_all_opcodes(self):
        """
        Return a list consisting of all opcodes in the GB database
        """
        query = """
                select distinct code, mnemonic
                from opcodes_v
                where mnemonic not like 'ILLEGAL%'
                """
        self.cur.execute(query)
        #return [x[0] for x in self.cur.fetchall()]
        return self.cur.fetchall()
                
        
        
    def find_implemented_opcodes(self, functionPattern, fName):
        """
        Update this object's list of opcodes with those that were implemented in a particular function, specified by functionPattern
        """
        
        
        cases = []
        functionStart = None
        fLines = None
        extended = 'prefixed' in functionPattern
        
        #find the line where the function starts
        with open(fName, 'r') as f:
            

            testPattern = re.compile(functionPattern)
            
            for (i, line) in enumerate(f):
                results = testPattern.findall(line)
                if results:
                    functionStart = i
                    break;
        
        #Not sure that this is strictly necessary, but calling f.readlines before the enumerator in the same block 
        #was causing nothing to be found in the loop.
        with open(fName, 'r') as f:
            fLines = f.readlines()
            
            
        counter = 0
        functionEnd = None
        
        #Find the line where the function ends by counting brackets
        #I think this will have a bug if the function was to end in one line
        #but I have enough self love that this isn't the case in my file
        for (i,line) in enumerate(fLines[functionStart:]):
            counter = counter + line.count('{') - line.count('}')
            if counter == 0:
                functionEnd = functionStart + i
                break;
        
        #Pattern to identify case statement
        casePattern = re.compile('case (0x..)')
            
        for line in fLines[functionStart:functionEnd + 1]:
            results = casePattern.findall(line)
            if results:
                code = results[0]
                if extended:
                    
                    self.implementedOpcodes.append(code[0:2]+"CB"+code[2:])
                    
                else:
                    self.implementedOpcodes.append(code)
            
        #return cases
    
    def output_base_rpt(self, fName):
        with open(fName, 'w', newline='') as f:
            reportWriter = csv.writer(f, )
            reportWriter.writerow(['Opcode', 'Mnemonic', 'Implemented'])
            for code in self.total_opcodes:
                
                implemented = 0
                if code[0] in self.implementedOpcodes:
                    implemented = 1
                
                reportWriter.writerow([code[0], code[1], implemented])
                
    def output_summary_rpt(self, fName):
        
        #Variables for the total row at the bottom of the sheet
        totalOps = 0
        totalImplemented = 0
        
        summary = {}
        for (code,mnemonic) in self.total_opcodes:
            
            if mnemonic not in summary:
                #Add the mnemonic in
                summary[mnemonic] = {'total':1, 'implemented':0 }
                totalOps += 1
                
            else:
                #We have seen the code so update the number of times we've seen it
                summary[mnemonic]['total'] += 1
                totalOps += 1
            
            #Now see if that particular opcode is in the lists of one found to be implemented. If so, add 1 to the count
            if code in self.implementedOpcodes:
                summary[mnemonic]['implemented'] += 1
                totalImplemented += 1
        
        with open(fName, 'w', newline='') as f:
            sReportWriter = csv.writer(f)
            sReportWriter.writerow(['Mnemonic', 'Implemented', 'Total', 'Percentage'])
            
            for mnemonic in summary:
                print
                sReportWriter.writerow([mnemonic, summary[mnemonic]['implemented'], summary[mnemonic]['total'], (summary[mnemonic]['implemented']/ summary[mnemonic]['total']) * 100 ])
                
            
            sReportWriter.writerow(["Grand Total", totalOps, totalImplemented, (totalImplemented/totalOps) * 100])
            

            

                
                
            
            
        
                    
        
if __name__ == '__main__':
    r = ReportCreator()
    r.output_base_rpt('opcodes.csv')
    r.output_summary_rpt('opcodes_summary.csv')
    print("Reports created")
    
    
        