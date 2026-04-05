# SPRACH Compiler - Quick Start Guide

## What is SPRACH?

SPRACH is a German-inspired mini programming language with a complete compiler that demonstrates three phases of compilation:
1. **Lexical Analysis** - Tokenization
2. **Syntax Analysis** - Parsing
3. **Semantic Analysis** - Type checking and validation

## Quick Compilation

### Windows (Pre-compiled)
Simply run:
```bash
sprach_simple < input.sprach
```

### Build from Source
```bash
bison -d simple_sprach.y
flex simple_sprach.l
gcc -o sprach_simple simple_sprach.tab.c lex.yy.c
```

## Running the Compiler

### Example 1: Basic Variable Declaration
```bash
echo "GANZ x = 5;" | ./sprach_simple
```
Output: `Var with init`

### Example 2: Using Input File
```bash
./sprach_simple < input.sprach
```

### Example 3: Interactive Mode
```bash
./sprach_simple
GANZ count = 10;
[Ctrl+D to end input]
```

## SPRACH Language Basics

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

- `simple_sprach.l` - Lexer specification
- `simple_sprach.y` - Parser specification
- `symbol_table.c/.h` - Symbol table for variable tracking
- `semantic_analyzer.c/.h` - Type checking engine
- `input.sprach` - Sample program
- `README.md` - Detailed documentation
- `IMPLEMENTATION.md` - Technical details

## Further Reading

See `README.md` for comprehensive language documentation.
See `IMPLEMENTATION.md` for compiler architecture details.

## Test the Compiler

```bash
# Run sample program
./sprach_simple < input.sprach

# Try declaring a variable
echo "GANZ myVar = 42;" | ./sprach_simple

# Test mathematical function
echo "KOMM result = quadrat_wurzel(16.0);" | ./sprach_simple
```

## Success Indicators

If you see output like `Var with init`, `Var declared`, or `OK`, the compilation was successful!

---

**Developed by:** Asique Ehetasamul Haque  
**For:** CSE 3212 - Compiler Design Laboratory  
**Khulna University of Engineering and Technology**
