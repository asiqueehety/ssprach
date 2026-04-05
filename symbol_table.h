#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <time.h>

/* Data type enumeration */
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_ARRAY,
    TYPE_UNKNOWN
} DataType;

/* Symbol entry for variables */
typedef struct {
    char *name;
    DataType type;
    int is_array;
    int array_size;
    int is_initialized;
    int is_function;
    int line_declared;
} SymbolEntry;

/* Scope structure for managing local scopes */
typedef struct Scope {
    SymbolEntry *symbols;
    int symbol_count;
    int capacity;
    struct Scope *parent;
} Scope;

/* Symbol table structure */
typedef struct {
    Scope *global_scope;
    Scope *current_scope;
    int total_variables;
    int total_functions;
} SymbolTable;

/* Function declarations */

/**
 * Create a new symbol table
 * @return Pointer to newly created symbol table
 */
SymbolTable* create_symbol_table(void);

/**
 * Add a variable to the current scope
 * @param table Symbol table
 * @param name Variable name
 * @param type Data type of variable
 * @param line Line number where declared
 */
void add_variable(SymbolTable *table, const char *name, DataType type, int line);

/**
 * Add an array to the current scope
 * @param table Symbol table
 * @param name Array name
 * @param type Element data type
 * @param size Array size (-1 for dynamic)
 * @param line Line number where declared
 */
void add_array(SymbolTable *table, const char *name, DataType type, int size, int line);

/**
 * Check if a variable is declared
 * @param table Symbol table
 * @param name Variable name
 * @param line Current line for error reporting
 * @return 1 if declared, 0 otherwise
 */
int check_variable_declared(SymbolTable *table, const char *name, int line);

/**
 * Check if a variable is initialized
 * @param table Symbol table
 * @param name Variable name
 * @param line Current line for error reporting
 * @return 1 if initialized, 0 otherwise
 */
int check_variable_initialized(SymbolTable *table, const char *name, int line);

/**
 * Perform type compatibility check
 * @param table Symbol table
 * @param name Variable name
 * @param line Current line for error reporting
 */
void check_type_compatibility(SymbolTable *table, const char *name, int line);

/**
 * Analyze array access
 * @param table Symbol table
 * @param name Array name
 * @param line Current line for error reporting
 */
void analyze_array_access(SymbolTable *table, const char *name, int line);

/**
 * Analyze function call
 * @param table Symbol table
 * @param name Function name
 * @param arg_count Number of arguments
 * @param line Current line for error reporting
 */
void analyze_function_call(SymbolTable *table, const char *name, int arg_count, int line);

/**
 * Analyze variable declaration
 * @param table Symbol table
 * @param name Variable name
 * @param type Data type
 * @param is_array 1 if array, 0 otherwise
 * @param line Current line for error reporting
 */
void analyze_declaration(SymbolTable *table, const char *name, DataType type, int is_array, int line);

/**
 * Analyze variable assignment
 * @param table Symbol table
 * @param name Variable name
 * @param type Data type
 * @param line Current line for error reporting
 */
void analyze_assignment(SymbolTable *table, const char *name, DataType type, int line);

/**
 * Analyze function declaration
 * @param table Symbol table
 * @param name Function name
 * @param return_type Return type
 * @param param_types Array of parameter types
 * @param param_count Number of parameters
 * @param line Current line for error reporting
 */
void analyze_function_declaration(SymbolTable *table, const char *name, DataType return_type,
                                  DataType *param_types, int param_count, int line);

/**
 * Enter a new scope
 * @param table Symbol table
 */
void enter_scope(SymbolTable *table);

/**
 * Exit current scope
 * @param table Symbol table
 */
void exit_scope(SymbolTable *table);

/**
 * Count total variables in symbol table
 * @param table Symbol table
 * @return Total variable count
 */
int count_variables(SymbolTable *table);

/**
 * Count total functions in symbol table
 * @param table Symbol table
 * @return Total function count
 */
int count_functions(SymbolTable *table);

/**
 * Print the symbol table
 * @param table Symbol table
 */
void print_symbol_table(SymbolTable *table);

/**
 * Free symbol table and its contents
 * @param table Symbol table
 */
void free_symbol_table(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */
