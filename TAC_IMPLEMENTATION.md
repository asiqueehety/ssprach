# Intermediate Code Generation (Three-Address Code) - TAC

## Overview

The SPRACH compiler now includes **Intermediate Code Generation** using **Three-Address Code (TAC)** representation. This is an important phase in compiler development that sits between the AST interpretation and potential optimization/code generation phases.

## What is Three-Address Code (TAC)?

Three-Address Code is an intermediate representation where:
- Each instruction has **at most 3 operands** (2 sources, 1 destination)
- Operations are atomic and simple
- Temporary variables (`_t0`, `_t1`, etc.) hold intermediate results
- Control flow uses labels and goto statements

### Example Conversion

**Original SPRACH Code**:
```sprach
GANZ result = 2 + 3 * 4;
geben result;
```

**TAC Generated**:
```
[  0] _t0 = CONST 2          # Load constant 2
[  1] _t1 = CONST 3          # Load constant 3
[  2] _t2 = CONST 4          # Load constant 4
[  3] _t3 = _t1 * _t2        # Multiply 3 * 4 = 12
[  4] _t4 = _t0 + _t3        # Add 2 + 12 = 14
[  5] STORE result = _t4      # Store result
[  6] LOAD _t5 = result       # Load result
[  7] PRINT _t5               # Print result
```

## Architecture

### TAC Operations Supported

#### Arithmetic Operations
- `TAC_ADD` - Addition
- `TAC_SUB` - Subtraction
- `TAC_MUL` - Multiplication
- `TAC_DIV` - Division
- `TAC_MOD` - Modulo
- `TAC_POW` - Power

#### Comparison Operations
- `TAC_LT` - Less than (<)
- `TAC_LE` - Less or equal (<=)
- `TAC_GT` - Greater than (>)
- `TAC_GE` - Greater or equal (>=)
- `TAC_EQ` - Equal (==)
- `TAC_NE` - Not equal (!=)

#### Logical Operations
- `TAC_AND` - Logical AND (und)
- `TAC_OR` - Logical OR (oder)
- `TAC_NOT` - Logical NOT (nicht)

#### Bitwise Operations
- `TAC_BITAND` - Bitwise AND (&)
- `TAC_BITOR` - Bitwise OR (|)
- `TAC_LSHIFT` - Left shift (<<)
- `TAC_RSHIFT` - Right shift (>>)

#### Memory Operations
- `TAC_LOAD_CONST` - Load constant value
- `TAC_LOAD_VAR` - Load variable from memory
- `TAC_STORE_VAR` - Store value to variable
- `TAC_ARRAY_ACCESS` - Access array element

#### Control Flow
- `TAC_LABEL` - Program label (target for jumps)
- `TAC_GOTO` - Unconditional jump
- `TAC_COND_GOTO` - Conditional jump (if-goto)

#### I/O Operations
- `TAC_PRINT` - Output statement
- `TAC_INPUT` - Input statement

#### Function Operations
- `TAC_CALL` - Function call

#### Array Operations
- `TAC_ARRAY_APPEND` - Append to array (hinzufuegen)
- `TAC_ARRAY_REMOVE` - Remove from array (entfernen)
- `TAC_ARRAY_SORT` - Sort array (sortieren)

### Code Structure

**Location in sprach.c**: Lines 1381-1885

**Key Components**:

```c
// TAC Program Structure
typedef struct {
    TACInstruction *instructions;  // Array of TAC instructions
    int count;                     // Number of instructions
    int capacity;                  // Buffer capacity
} TACProgram;

// TAC Instruction Format
typedef struct {
    TACOpType op;                  // Operation type
    char arg1[256];                // First operand
    char arg2[256];                // Second operand
    char result[256];              // Result/destination
    char label[256];               // For labels and jumps
} TACInstruction;
```

### Key Functions

```c
TACProgram *tac_create()
    - Creates a new TAC program with initial capacity

char *tac_new_temp()
    - Generates unique temporary variable names (_t0, _t1, etc.)

char *tac_new_label()
    - Generates unique label names (_L0, _L1, etc.)

void tac_emit(TACProgram *prog, TACOpType op, ...)
    - Emits a new TAC instruction to the program

void tac_emit_label(TACProgram *prog, const char *label)
    - Emits a label instruction

char *generate_tac_expression(ASTNode *node, TACProgram *prog)
    - Generates TAC for expressions
    - Returns the temporary variable holding the result

void generate_tac_statement(ASTNode *node, TACProgram *prog)
    - Generates TAC for statements
    - Handles loops, conditionals, assignments, etc.

void tac_print(TACProgram *prog)
    - Prints TAC to console (stdout)

void tac_save_to_file(TACProgram *prog, const char *filename)
    - Saves TAC to file (tac_output.txt)
```

## Usage

### How TAC is Generated

**In main() function** (Lines 1855-1885):

```c
// Parse the source code into AST
ASTNode *program = parser_parse_statement(parser);

// Generate intermediate code (TAC)
TACProgram *tac_prog = tac_create();
generate_tac_statement(program, tac_prog);

// Print TAC to console
tac_print(tac_prog);

// Save TAC to file
tac_save_to_file(tac_prog, "tac_output.txt");

// Execute the original AST (still interpreted directly)
Environment *env = env_create();
eval(program, env);
```

### Compilation

```bash
gcc -o sprach sprach.c -lm
```

### Execution

```bash
cat input.sprach | ./sprach.exe
```

### Output

When you run the compiler, you will see:

1. **TAC printed to stdout** (console):
   - Shows the intermediate code representation
   - Useful for debugging and understanding code flow

2. **TAC saved to file** (`tac_output.txt`):
   - Complete TAC program for documentation
   - Can be used for further analysis/optimization

3. **Program execution** (original):
   - The program still executes normally
   - TAC generation doesn't affect runtime behavior

## Examples

### Example 1: Simple Arithmetic with Array

**Input Code**:
```sprach
GANZ arr[] = {10, 20, 30};
arr hinzufuegen 40;
geben arr[0];
geben arr[3];
```

**TAC Output**:
```
[  0] _t0 = CONST 40
[  1] APPEND arr <- _t0
[  2] _t1 = CONST 0
[  3] _t2 = arr[_t1]
[  4] PRINT _t2
[  5] _t3 = CONST 3
[  6] _t4 = arr[_t3]
[  7] PRINT _t4
```

**Console Output**:
```
========== INTERMEDIATE CODE (THREE-ADDRESS CODE) ==========

[  0] _t0 = CONST 40
[  1] APPEND arr <- _t0
[  2] _t1 = CONST 0
[  3] _t2 = arr[_t1]
[  4] PRINT _t2
[  5] _t3 = CONST 3
[  6] _t4 = arr[_t3]
[  7] PRINT _t4

===========================================================

10
40
```

### Example 2: Control Flow with Loops

**Input Code**:
```sprach
GANZ sum = 0;
fuer (i = 1; i <= 5; i = i + 1) {
    sum = sum + i;
}
geben sum;
```

**TAC Output**:
```
[  0] _t0 = CONST 0
[  1] STORE sum = _t0
[  2] _t1 = CONST 1
[  3] STORE i = _t1
[  4] _L0:
[  5] _t2 = LOAD i
[  6] _t3 = CONST 5
[  7] _t4 = _t2 <= _t3
[  8] IF _t4 GOTO _L1
[  9] GOTO _L2
[ 10] _L1:
[ 11] _t5 = LOAD sum
[ 12] _t6 = LOAD i
[ 13] _t7 = _t5 + _t6
[ 14] STORE sum = _t7
[ 15] _t8 = LOAD i
[ 16] _t9 = CONST 1
[ 17] _t10 = _t8 + _t9
[ 18] STORE i = _t10
[ 19] GOTO _L0
[ 20] _L2:
[ 21] _t11 = LOAD sum
[ 22] PRINT _t11
```

**Output**:
```
========== INTERMEDIATE CODE (THREE-ADDRESS CODE) ==========
[...TAC listing...]
===========================================================
15
```

## Benefits of TAC

1. **Optimization Opportunities**:
   - Constant folding (2+3 → 5 at compile time)
   - Dead code elimination
   - Loop invariant motion
   - Common subexpression elimination

2. **Backend Independence**:
   - Can generate code for different target architectures
   - TAC is architecture-neutral

3. **Debugging and Analysis**:
   - Clear representation of program semantics
   - Easy to understand program flow
   - Useful for educational purposes

4. **JIT Compilation**:
   - TAC can be used as input for Just-In-Time compilation
   - Better performance for repeated execution

## Future Enhancements

Potential improvements to TAC generation:

1. **Optimization Passes**:
   - Constant folding optimizer
   - Dead code eliminator
   - Loop optimizer

2. **Backend Code Generation**:
   - x86 assembly generation
   - WebAssembly generation
   - LLVM IR generation

3. **Advanced Analysis**:
   - Data flow analysis
   - Control flow graph generation
   - Use-def chains

## Files

- **Implementation**: `sprach.c` (lines 1381-1885)
- **Output**: `tac_output.txt` (generated during compilation)
- **Test cases**: `test_tac.sprach`, `test_tac_control.sprach`

## Summary

The intermediate code generation feature demonstrates professional compiler architecture:

✅ AST → TAC → Interpretation (current)
✅ Extensible to AST → TAC → Optimization → Code Generation
✅ Clean separation of concerns
✅ Educational value for compiler theory

This makes the SPRACH compiler suitable for advanced compilation techniques and optimizations!
