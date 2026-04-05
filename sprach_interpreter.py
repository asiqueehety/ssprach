#!/usr/bin/env python3
"""
SPRACH Simple Interpreter
Supports basic variable operations and loops
"""

import sys
import re

class SpracheInterpreter:
    def __init__(self):
        self.variables = {}
        self.output = []
        
    def parse_and_execute(self, code):
        lines = code.strip().split('\n')
        self.execute_block(lines, 0, len(lines))
        
    def execute_block(self, lines, start, end):
        i = start
        while i < end:
            line = lines[i].strip()
            if not line or line.startswith('#'):
                i += 1
                continue
            
            # Handle while loops
            if line.startswith('waehrend'):
                i = self.execute_while(lines, i, end)
                continue
            
            # Handle if statements
            if line.startswith('wenn'):
                i = self.execute_if(lines, i, end)
                continue
            
            # Handle for loops
            if line.startswith('fur'):
                i = self.execute_for(lines, i, end)
                continue
            
            # Handle declarations and assignments
            self.execute_statement(line)
            i += 1
        
        return end
    
    def execute_while(self, lines, start, end):
        # Extract condition
        match = re.match(r'waehrend\s*\((.*?)\)\s*\{', lines[start])
        if not match:
            return start + 1
        
        condition_str = match.group(1)
        
        # Find matching braces
        brace_count = 0
        body_start = start + 1
        body_end = body_start
        
        in_condition = False
        for j in range(start, end):
            line = lines[j]
            if '{' in line:
                in_condition = True
            if in_condition:
                brace_count += line.count('{') - line.count('}')
                if brace_count == 0 and j > start:
                    body_end = j
                    break
        
        # Execute loop
        iterations = 0
        max_iterations = 10000  # Safety limit
        while iterations < max_iterations:
            condition = self.evaluate_expression(condition_str)
            if not condition:
                break
            
            self.execute_block(lines, body_start, body_end)
            iterations += 1
        
        return body_end + 1
    
    def execute_if(self, lines, start, end):
        # Extract condition
        match = re.match(r'wenn\s*\((.*?)\)\s*\{', lines[start])
        if not match:
            return start + 1
        
        condition_str = match.group(1)
        
        # Find matching braces
        brace_count = 0
        body_start = start + 1
        body_end = body_start
        
        for j in range(start, end):
            line = lines[j]
            brace_count += line.count('{') - line.count('}')
            if brace_count == 0 and j > start:
                body_end = j
                break
        
        # Execute if true
        if self.evaluate_expression(condition_str):
            self.execute_block(lines, body_start, body_end)
        
        return body_end + 1
    
    def execute_for(self, lines, start, end):
        # Parse for (init; condition; increment)
        match = re.match(r'fur\s*\((.*?);(.*?);(.*?)\)\s*\{', lines[start])
        if not match:
            return start + 1
        
        init_str, cond_str, incr_str = match.group(1), match.group(2), match.group(3)
        
        # Find loop body
        brace_count = 0
        body_start = start + 1
        body_end = body_start
        
        for j in range(start, end):
            line = lines[j]
            brace_count += line.count('{') - line.count('}')
            if brace_count == 0 and j > start:
                body_end = j
                break
        
        # Execute loop
        self.execute_statement(init_str)
        iterations = 0
        while iterations < 10000:
            if not self.evaluate_expression(cond_str):
                break
            self.execute_block(lines, body_start, body_end)
            self.execute_statement(incr_str)
            iterations += 1
        
        return body_end + 1
    
    def execute_statement(self, stmt):
        stmt = stmt.strip()
        if not stmt:
            return
        
        # Remove trailing semicolon
        if stmt.endswith(';'):
            stmt = stmt[:-1]
        
        # Handle output
        if stmt.startswith('geben '):
            expr_str = stmt[6:].strip()
            value = self.evaluate_expression(expr_str)
            print(int(value) if value == int(value) else value)
            return
        
        # Handle input
        if stmt.startswith('lesen '):
            var_name = stmt[6:].strip()
            try:
                self.variables[var_name] = float(input())
            except:
                pass
            return
        
        # Handle declaration with assignment
        if any(stmt.startswith(f'{t} ') for t in ['GANZ', 'KOMM', 'TEXT', 'ZEICHEN']):
            parts = stmt.split(None, 1)
            if len(parts) > 1:
                rest = parts[1]
                if ' = ' in rest:
                    var_name, value_str = rest.split(' = ', 1)
                    var_name = var_name.split('[')[0].strip()
                    value = self.evaluate_expression(value_str)
                    self.variables[var_name] = value
                else:
                    var_name = rest.split('[')[0].strip()
                    self.variables[var_name] = 0
            return
        
        # Handle assignment
        if ' = ' in stmt:
            var_name, value_str = stmt.split(' = ', 1)
            var_name = var_name.split('[')[0].strip()
            value = self.evaluate_expression(value_str)
            self.variables[var_name] = value
            return
    
    def evaluate_expression(self, expr):
        expr = expr.strip()
        
        # Replace variable names with values
        for var_name in sorted(self.variables.keys(), key=len, reverse=True):
            expr = re.sub(r'\b' + var_name + r'\b', str(self.variables[var_name]), expr)
        
        # Replace SPRACH operators
        expr = expr.replace(' und ', ' and ').replace(' oder ', ' or ').replace(' nicht ', ' not ')
        
        try:
            result = eval(expr)
            return float(result)
        except:
            return 0

if __name__ == '__main__':
    code = sys.stdin.read()
    interpreter = SpracheInterpreter()
    interpreter.parse_and_execute(code)
