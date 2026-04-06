# SPRACH Compiler - Quick Start Guide

## What is SPRACH?

SPRACH is a German-inspired mini programming language with a complete compiler that demonstrates five phases of compilation:
1. **Lexical Analysis** - Tokenization (50+ token types)
2. **Syntax Analysis** - Parsing (15 precedence levels)
3. **Semantic Analysis** - Type checking and validation (symbol table)
4. **Intermediate Code Generation** - Three-Address Code (TAC) with 30 operation types
5. **Interpretation** - AST-based execution engine

## Quick Compilation

### Windows (Pre-compiled)
Simply run:
```bash
sprach < input.sprach
```

### Build from Source
```bash
gcc -o sprach sprach.c
```

### Running the Compiler
```bash
sprach < input.sprach
```
The compiler produces:
- **TAC Output** to console showing three-address code
- **TAC Output** to file (tac_output.txt) for documentation
- **Result** - Program execution output

## What is TAC (Three-Address Code)?

TAC is an **intermediate representation** of source code that bridges the gap between high-level programming and machine instructions. It breaks down complex expressions into simple, sequential operations where each instruction has at most three operands.

### Why TAC?
- **Clarity**: Shows exactly what the compiler does with each operation
- **Optimization Ready**: Makes it easier to apply compiler optimizations
- **Machine Generation**: Can be easily converted to assembly or machine code
- **Debugging**: Helps understand program flow and transformations

### TAC Example
For the SPRACH program:
```sprach
GANZ a = 5;
GANZ b = 3;
GANZ c = a + b;
geben c;
```

The compiler generates this TAC:
```
TAC 1: LOAD_CONST _t0 = 5
TAC 2: STORE_VAR a = _t0
TAC 3: LOAD_CONST _t1 = 3
TAC 4: STORE_VAR b = _t1
TAC 5: LOAD_VAR _t2 = a
TAC 6: LOAD_VAR _t3 = b
TAC 7: ADD _t4 = _t2 + _t3
TAC 8: STORE_VAR c = _t4
```

Notice how:
- Temporary variables (`_t0`, `_t1`, etc.) store intermediate results
- Each operation is atomic (one instruction)
- Complex expressions are decomposed into steps
- Variable loads and stores are explicit



### Variable Declaration
```sprach
GANZ x = 10;          # Integer variable
KOMM y = 3.14;        # Double/floating-point variable
TEXT msg = "Hallo";   # String variable
ZEICHEN ch = 'A';     # Character variable
```

### Simple Program Example
```sprach
GANZ x = 10;
KOMM y = 3.5;
TEXT msg = "Hallo Welt";

wenn (x > 5) {
    geben msg;
}
sonst {
    geben "Kleinere Zahl";
}
```

## Language Keywords

| German | English | Type |
|--------|---------|------|
| GANZ | int | Data type |
| KOMM | double | Data type |
| TEXT | string | Data type |
| ZEICHEN | char | Data type |
| wenn | if | Control flow |
| sonst | else | Control flow |
| fuer | for | Control flow |
| waehrend | while | Control flow |
| wahl | switch | Control flow |
| lesen | read | I/O |
| geben | write | I/O |

## Supported Operations

### Arithmetic: + - * / %
### Relational: == != < > <= >=
### Logical: und (and), oder (or), nicht (not)
### Array: hinzufuegen (add), entfernen (remove), sortieren (sort)

## Sample Programs

### Program 1: Loop
```sprach
fuer (GANZ i = 0; i < 5; i = i + 1) {
    geben i;
}
```

### Program 2: While Loop
```sprach
GANZ count = 3;
waehrend (count > 0) {
    geben count;
    count = count - 1;
}
```

### Program 3: Array Operations
```sprach
TEXT names[];
names hinzufuegen "Alice";
names hinzufuegen "Bob";
names sortieren;
```

## Troubleshooting

### Compiler crashes on input
- Ensure you're providing valid SPRACH syntax
- Check for missing semicolons
- Verify variable declarations before use

### "Variable not declared" error
- Declare all variables before using them
- Check spelling of variable names
- Remember: SPRACH is case-sensitive

### "Type mismatch" error
- Ensure assignments use compatible types
- Integer and double are interchangeable
- Strings cannot be assigned to numeric types

## Project Files

- `sprach.c` - Complete compiler (lexer, parser, semantic analyzer, TAC generator, interpreter)
- `input.sprach` - Sample program
- `README.md` - Comprehensive documentation
- `QUICKSTART.md` - This file

## Further Reading

See `README.md` for comprehensive language documentation, TAC details, and compiler architecture.

## Test the Compiler

```bash
# Compile the compiler
gcc -o sprach sprach.c

# Run sample program (shows TAC + output)
sprach < input.sprach

# Try simple program
echo "GANZ x = 10; KOMM y = 3.5; geben x + y;" | sprach
```

The output will show:
1. **TAC Instructions** - The intermediate code representation
2. **Program Result** - The actual output from execution

## Success Indicators

The compiler successfully displays both the TAC (three-address code) intermediate representation and the program execution result. Check `tac_output.txt` for the complete TAC dump.

---

**Developed by:** Asique Ehetasamul Haque  
**For:** CSE 3212 - Compiler Design Laboratory  
**Khulna University of Engineering and Technology**
