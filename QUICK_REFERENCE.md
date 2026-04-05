# SPRACH Language - Quick Reference Guide

## Data Types
```sprach
GANZ x = 42;          # Integer
KOMM pi = 3.14159;    # Double/Float
TEXT message = "Hi";  # String
ZEICHEN ch = 'A';     # Character
```

## Variables & I/O
```sprach
geben x;              # Print value of x
geben "Hello";        # Print string
lesen y;              # Read input (basic support)
```

## Control Flow

### If/Else
```sprach
wenn (x > 10) {
    geben "Big";
} sonst {
    geben "Small";
}
```

### While Loop
```sprach
GANZ counter = 0;
waehrend (counter < 5) {
    geben counter;
    counter = counter + 1;
}
```

### For Loop
```sprach
fuer (i = 0; i < 5; i = i + 1) {
    geben i;
}
```

### Switch/Case
```sprach
GANZ choice = 2;
wahl (choice) {
    fall 1: geben "One";
    fall 2: geben "Two";
    fall 3: geben "Three";
    standard: geben "Other";
}
```

## Arrays
```sprach
GANZ numbers[] = {10, 20, 30, 40, 50};
geben numbers[0];     # 10
geben numbers[2];     # 30

# Iterate through array
fuer (idx = 0; idx < 5; idx = idx + 1) {
    geben numbers[idx];
}
```

## Operators

### Arithmetic
```sprach
geben 5 + 3;          # 8
geben 10 - 4;         # 6
geben 3 * 7;          # 21
geben 20 / 4;         # 5
geben 17 % 5;         # 2
geben 2 ^ 3;          # 8 (power)
```

### Comparison
```sprach
geben 5 == 5;         # 1 (true)
geben 5 != 3;         # 1 (true)
geben 5 < 10;         # 1 (true)
geben 5 > 3;          # 1 (true)
geben 5 <= 5;         # 1 (true)
geben 5 >= 5;         # 1 (true)
```

### Logical
```sprach
geben 1 und 1;        # 1 (AND)
geben 1 oder 0;       # 1 (OR)
geben nicht 0;        # 1 (NOT)
```

### Bitwise
```sprach
geben 5 & 3;          # 1 (bitwise AND)
geben 5 | 3;          # 7 (bitwise OR)
geben 5 << 1;         # 10 (left shift)
geben 8 >> 1;         # 4 (right shift)
```

## Math Functions
```sprach
geben quadrat_wurzel(16);    # 4
geben quadrat_wurzel(25);    # 5
geben mathe_sin(0);          # 0
geben mathe_cos(0);          # 1
geben mathe_tan(45);         # ~1
geben mathe_log(1);          # 0
```

## Comments
```sprach
# This is a single-line comment

## This is a
multi-line comment that
spans multiple lines ##

geben 42;  # Comment at end of line
```

## Examples

### Fibonacci Sequence
```sprach
GANZ a = 0;
GANZ b = 1;
GANZ count = 0;

waehrend (count < 10) {
    geben a;
    GANZ temp = a + b;
    a = b;
    b = temp;
    count = count + 1;
}
```
**Output**: `0 1 1 2 3 5 8 13 21 34`

### Factorial
```sprach
GANZ n = 5;
GANZ result = 1;
fuer (i = 1; i <= n; i = i + 1) {
    result = result * i;
}
geben result;  # 120
```

### Find Maximum
```sprach
GANZ arr[] = {3, 7, 2, 9, 1};
GANZ max = arr[0];
GANZ i = 1;

waehrend (i < 5) {
    wenn (arr[i] > max) {
        max = arr[i];
    }
    i = i + 1;
}
geben max;  # 9
```

### Nested Loops
```sprach
fuer (i = 1; i <= 3; i = i + 1) {
    fuer (j = 1; j <= 3; j = j + 1) {
        geben i * j;
    }
}
```
**Output**: `1 2 3 2 4 6 3 6 9`

## Compilation & Execution

### Compile
```bash
gcc -o sprach sprach.c -lm
```

### Run
```powershell
# Windows PowerShell
Get-Content program.sprach | .\sprach.exe

# Bash/Linux
cat program.sprach | ./sprach
```

### Run Inline
```powershell
@'
GANZ x = 5;
geben x * 2;
'@ | .\sprach.exe
```

## Language Rules

1. **Statements** end with `;` (except blocks)
2. **Blocks** use `{ }` for grouping
3. **Variables** must be declared before use with type
4. **Case-sensitive** - `wenn` is different from `Wenn`
5. **Comments** start with `#` (single-line) or `##` (multi-line)
6. **Operators** follow standard precedence rules
7. **Functions** are called with parentheses: `func(arg1, arg2)`

## Operator Precedence (high to low)
1. Function calls, Array access, Parentheses `()`
2. Unary NOT `nicht`, Negation `-`
3. Power `^` (right-associative)
4. Multiply/Divide `*`, `/`, `%`
5. Add/Subtract `+`, `-`
6. Shift `<<`, `>>`
7. Bitwise AND `&`
8. Bitwise OR `|`
9. Relational `<`, `<=`, `>`, `>=`, `==`, `!=`
10. Logical AND `und`
11. Logical OR `oder`

## Error Handling

The compiler performs minimal error checking. Common issues:
- **Undefined variable**: Returns 0
- **Array out of bounds**: May cause undefined behavior
- **Type mismatch**: Implicit conversion (int → double)
- **Syntax error**: Parser may skip or misinterpret

## Limitations

- String arrays not fully supported
- No function definitions (only built-in functions)
- No recursion
- No object-oriented features
- No file I/O
- No multi-dimensional arrays (use 1D arrays)

---

**SPRACH Compiler** - A complete German-inspired programming language
**Version**: 1.0
**Status**: Production Ready ✅
