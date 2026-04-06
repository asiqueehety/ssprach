# SPRACH Compiler - Error Handling System

## Overview

The SPRACH compiler now includes comprehensive runtime error handling with detailed error messages for:
- Type mismatches
- Undefined variables
- Undefined functions
- Math domain errors (sqrt of negative, log of non-positive)
- Division by zero
- Modulo by zero
- Array bounds violations

## Error Types

### 1. **Undefined Variable Errors**
Occurs when referencing a variable that hasn't been declared.

**Example:**
```sprach
geben undefined_var;
```

**Error Message:**
```
[ERROR] Undefined variable: 'undefined_var'
```

### 2. **Division by Zero**
Occurs when attempting to divide by zero.

**Example:**
```sprach
geben 10 / 0;
```

**Error Message:**
```
[ERROR] Division by zero error
```

### 3. **Modulo by Zero**
Occurs when attempting modulo operation with zero divisor.

**Example:**
```sprach
geben 17 % 0;
```

**Error Message:**
```
[ERROR] Modulo by zero error
```

### 4. **Math Domain Errors - Square Root**
Occurs when attempting to take the square root of a negative number.

**Example:**
```sprach
geben quadrat_wurzel(-4);
```

**Error Message:**
```
[ERROR] Math domain error: quadrat_wurzel() of negative number (-4.00)
```

### 5. **Math Domain Errors - Logarithm**
Occurs when attempting to take the logarithm of zero or negative numbers.

**Example:**
```sprach
geben mathe_log(0);
geben mathe_log(-5);
```

**Error Messages:**
```
[ERROR] Math domain error: mathe_log() of non-positive number (0.00)
[ERROR] Math domain error: mathe_log() of non-positive number (-5.00)
```

### 6. **Undefined Function Errors**
Occurs when calling a function that doesn't exist.

**Example:**
```sprach
geben my_function(5);
```

**Error Message:**
```
[ERROR] Undefined function: 'my_function'
```

### 7. **Array Bounds Errors**
Occurs when accessing an array element outside valid indices.

**Example:**
```sprach
GANZ arr[] = {1, 2, 3};
geben arr[100];        # Out of bounds
geben arr[-1];         # Negative index
```

**Error Message:**
```
[ERROR] Array index out of bounds: arr[100] (size=3)
[ERROR] Array index out of bounds: arr[-1] (size=3)
```

### 8. **Undefined Array Errors**
Occurs when referencing an array that doesn't exist.

**Example:**
```sprach
geben undefined_array[0];
```

**Error Message:**
```
[ERROR] Undefined array: 'undefined_array'
```

## Error Handling Features

### Real-time Error Detection
- Errors are detected and reported immediately during execution
- Error messages are printed to stderr with `[ERROR]` prefix
- Execution continues after non-fatal errors (graceful degradation)
- Errors return safe default values (0 or null)

### Error Tracking
- All errors are tracked in a global error handler
- Error count is maintained throughout execution
- Error summary printed at end of execution

### Error Messages Format
```
[ERROR] <Error Category>: <Detailed Message>
[ERROR SUMMARY] <count> error(s) encountered during execution
```

## Implementation Details

### Error Handler Structure
```c
typedef struct {
    int error_count;           // Count of errors
    char error_messages[100][512];  // Error messages buffer
    int runtime_error;         // Current error flag
    char last_error[512];      // Last error message
} ErrorHandler;
```

### Key Functions

**`error_report(format, ...)`**
- Reports a runtime error
- Prints to stderr with prefix
- Updates error count
- Supports formatted arguments like printf

**`has_error()`**
- Returns 1 if error occurred, 0 otherwise
- Used to check error state

**`clear_error()`**
- Clears current error state
- Resets error flags

**`error_handler_create()`**
- Creates error handler structure
- Called at program start

### Type Checking Helpers

**`variable_exists(env, varname)`**
- Checks if variable is defined
- Returns 1 if exists, 0 if not

**`is_string_variable(env, varname)`**
- Checks if variable is string type

**`is_array_variable(env, varname)`**
- Checks if variable is array type

## Testing Error Handling

### Running Error Tests
```bash
# Run comprehensive error test suite
Get-Content testing.sprach | ./sprach 2>&1

# Capture only errors
Get-Content testing.sprach | ./sprach 2>&1 | Select-String "ERROR"

# Run with error test script
.\run_error_tests.ps1
```

### Test File: `testing.sprach`
Contains 50+ test cases covering:
- Type mismatches
- Undefined variables (3 tests)
- Array boundaries (3 tests)
- Division by zero (3 tests)
- Invalid arithmetic operations
- Function call errors
- Math domain errors (3 tests)
- And 11 more categories

### Example Error Output
```
========== INTERMEDIATE CODE (THREE-ADDRESS CODE) ==========

[  0] _t0 = CONST 10
[  1] _t1 = CONST 0
[  2] _t2 = _t1 / _t1
[  3] PRINT _t2

===========================================================

[ERROR] Division by zero error
0

[ERROR SUMMARY] 1 error(s) encountered during execution
```

## Error Propagation

### How Errors Propagate Through Evaluation
1. **Error occurs** in expression evaluation
2. **Error flag set** via `error_report()`
3. **Safe value returned** (0 or null)
4. **Caller checks** `has_error()` to propagate
5. **Error logged** to stderr
6. **Execution continues** with default value

### Example: Division by Zero
```c
case '/':
    if (right == 0) {
        error_report("Division by zero error");
        return 0;    // Safe return value
    }
    return left / right;
```

## Error Recovery

The compiler uses graceful error recovery:
- **Non-fatal errors**: Program continues execution
- **Safe defaults**: Operations return 0 or empty on error
- **No crash**: Errors don't terminate the program
- **User visibility**: All errors printed to stderr

### Recovery Strategy
```
Error Detected
    ↓
Report Error
    ↓
Set Error Flag
    ↓
Return Safe Value
    ↓
Continue Execution
    ↓
Print Error Summary
```

## Compilation

The error handling system requires:
- **C99 or later** (for `stdarg.h`)
- **Standard math library** (`-lm` flag)

```bash
gcc -o sprach sprach.c -lm
```

## Error Output Examples

### Example 1: Undefined Variable
```
Input:  geben undefined_var;
Output: [ERROR] Undefined variable: 'undefined_var'
        0
```

### Example 2: Division by Zero
```
Input:  geben 10 / 0;
Output: [ERROR] Division by zero error
        0
```

### Example 3: Math Domain Error
```
Input:  geben quadrat_wurzel(-4);
Output: [ERROR] Math domain error: quadrat_wurzel() of negative number (-4.00)
        0
```

### Example 4: Array Bounds
```
Input:  GANZ arr[] = {1, 2, 3}; geben arr[100];
Output: [ERROR] Array index out of bounds: arr[100] (size=3)
        0
```

### Example 5: Undefined Function
```
Input:  geben my_function(5);
Output: [ERROR] Undefined function: 'my_function'
        0
```

## Future Enhancements

Potential improvements to error handling:
1. **Line number tracking** - Report error at specific line
2. **Error codes** - Return specific error codes
3. **Custom error messages** - User-defined error callbacks
4. **Error recovery strategies** - More intelligent recovery
5. **Error context** - Show AST node context
6. **Stack traces** - Function call stack on error
7. **Type coercion** - Optional automatic type conversion
8. **Warnings** - Non-fatal warnings vs. errors

## Compliance

The error handling system:
- ✅ Handles all major runtime errors
- ✅ Provides clear error messages
- ✅ Maintains program state consistency
- ✅ Continues execution after errors
- ✅ Logs all errors for debugging
- ✅ Integrates with TAC generation
- ✅ Works with all language features

---

**Status**: ✅ **Comprehensive Error Handling Implemented**

The SPRACH compiler now provides professional-grade runtime error detection and reporting for all major error categories.
