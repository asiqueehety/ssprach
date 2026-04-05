#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int line_num;

/* Helper function to convert DataType to string */
static const char* datatype_to_string(DataType type) {
    switch (type) {
        case TYPE_INT: return "GANZ (int)";
        case TYPE_DOUBLE: return "KOMM (double)";
        case TYPE_STRING: return "TEXT (string)";
        case TYPE_CHAR: return "ZEICHEN (char)";
        case TYPE_ARRAY: return "Array";
        default: return "UNKNOWN";
    }
}

/**
 * Create a new symbol table
 */
SymbolTable* create_symbol_table(void) {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (table == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for symbol table\n");
        return NULL;
    }
    
    table->global_scope = (Scope *)malloc(sizeof(Scope));
    if (table->global_scope == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for global scope\n");
        free(table);
        return NULL;
    }
    
    table->global_scope->symbols = (SymbolEntry *)malloc(100 * sizeof(SymbolEntry));
    if (table->global_scope->symbols == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for symbols\n");
        free(table->global_scope);
        free(table);
        return NULL;
    }
    
    table->global_scope->symbol_count = 0;
    table->global_scope->capacity = 100;
    table->global_scope->parent = NULL;
    
    table->current_scope = table->global_scope;
    table->total_variables = 0;
    table->total_functions = 0;
    
    return table;
}

/**
 * Add a variable to the current scope
 */
void add_variable(SymbolTable *table, const char *name, DataType type, int line) {
    if (table == NULL || name == NULL || table->current_scope == NULL) {
        return;
    }
    
    Scope *scope = table->current_scope;
    
    /* Check if variable already exists in current scope */
    for (int i = 0; i < scope->symbol_count; i++) {
        if (strcmp(scope->symbols[i].name, name) == 0) {
            fprintf(stderr, "[Line %d] Warning: Variable '%s' already declared\n", line, name);
            return;
        }
    }
    
    /* Resize if needed */
    if (scope->symbol_count >= scope->capacity) {
        scope->capacity *= 2;
        scope->symbols = (SymbolEntry *)realloc(scope->symbols, scope->capacity * sizeof(SymbolEntry));
        if (scope->symbols == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed\n");
            return;
        }
    }
    
    /* Add new variable */
    SymbolEntry *entry = &scope->symbols[scope->symbol_count];
    entry->name = (char *)malloc(strlen(name) + 1);
    if (entry->name == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for variable name\n");
        return;
    }
    
    strcpy(entry->name, name);
    entry->type = type;
    entry->is_array = 0;
    entry->array_size = 0;
    entry->is_initialized = 0;
    entry->is_function = 0;
    entry->line_declared = line;
    
    scope->symbol_count++;
    table->total_variables++;
}

/**
 * Add an array to the current scope
 */
void add_array(SymbolTable *table, const char *name, DataType type, int size, int line) {
    if (table == NULL || name == NULL || table->current_scope == NULL) {
        return;
    }
    
    Scope *scope = table->current_scope;
    
    /* Check if array already exists */
    for (int i = 0; i < scope->symbol_count; i++) {
        if (strcmp(scope->symbols[i].name, name) == 0) {
            fprintf(stderr, "[Line %d] Error: Array '%s' already declared\n", line, name);
            return;
        }
    }
    
    /* Resize if needed */
    if (scope->symbol_count >= scope->capacity) {
        scope->capacity *= 2;
        scope->symbols = (SymbolEntry *)realloc(scope->symbols, scope->capacity * sizeof(SymbolEntry));
        if (scope->symbols == NULL) {
            fprintf(stderr, "Error: Memory reallocation failed\n");
            return;
        }
    }
    
    /* Add new array */
    SymbolEntry *entry = &scope->symbols[scope->symbol_count];
    entry->name = (char *)malloc(strlen(name) + 1);
    if (entry->name == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    
    strcpy(entry->name, name);
    entry->type = type;
    entry->is_array = 1;
    entry->array_size = size;
    entry->is_initialized = 0;
    entry->is_function = 0;
    entry->line_declared = line;
    
    scope->symbol_count++;
    table->total_variables++;
}

/**
 * Check if a variable is declared
 */
int check_variable_declared(SymbolTable *table, const char *name, int line) {
    if (table == NULL || name == NULL || table->current_scope == NULL) {
        return 0;
    }
    
    Scope *scope = table->current_scope;
    
    /* Search in current scope and parent scopes */
    while (scope != NULL) {
        for (int i = 0; i < scope->symbol_count; i++) {
            if (strcmp(scope->symbols[i].name, name) == 0) {
                return 1;
            }
        }
        scope = scope->parent;
    }
    
    fprintf(stderr, "[Line %d] Error: Variable '%s' is not declared\n", line, name);
    return 0;
}

/**
 * Check if a variable is initialized
 */
int check_variable_initialized(SymbolTable *table, const char *name, int line) {
    if (table == NULL || name == NULL) {
        return 0;
    }
    
    Scope *scope = table->current_scope;
    
    /* Search in scopes */
    while (scope != NULL) {
        for (int i = 0; i < scope->symbol_count; i++) {
            if (strcmp(scope->symbols[i].name, name) == 0) {
                if (!scope->symbols[i].is_initialized) {
                    fprintf(stderr, "[Line %d] Warning: Variable '%s' may not be initialized\n", line, name);
                    return 0;
                }
                return 1;
            }
        }
        scope = scope->parent;
    }
    
    return 0;
}

/**
 * Perform type compatibility check
 */
void check_type_compatibility(SymbolTable *table, const char *name, int line) {
    /* Type checking logic can be expanded here */
}

/**
 * Analyze array access
 */
void analyze_array_access(SymbolTable *table, const char *name, int line) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    Scope *scope = table->current_scope;
    
    while (scope != NULL) {
        for (int i = 0; i < scope->symbol_count; i++) {
            if (strcmp(scope->symbols[i].name, name) == 0) {
                if (!scope->symbols[i].is_array) {
                    fprintf(stderr, "[Line %d] Error: '%s' is not an array\n", line, name);
                }
                return;
            }
        }
        scope = scope->parent;
    }
    
    fprintf(stderr, "[Line %d] Error: Array '%s' not declared\n", line, name);
}

/**
 * Analyze function call
 */
void analyze_function_call(SymbolTable *table, const char *name, int arg_count, int line) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    /* Can be expanded for more detailed function analysis */
}

/**
 * Analyze variable declaration
 */
void analyze_declaration(SymbolTable *table, const char *name, DataType type, int is_array, int line) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    if (is_array) {
        add_array(table, name, type, 0, line);
    } else {
        add_variable(table, name, type, line);
    }
}

/**
 * Analyze variable assignment
 */
void analyze_assignment(SymbolTable *table, const char *name, DataType type, int line) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    check_variable_declared(table, name, line);
}

/**
 * Analyze function declaration
 */
void analyze_function_declaration(SymbolTable *table, const char *name, DataType return_type,
                                  DataType *param_types, int param_count, int line) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    /* Add function analysis logic here */
}

/**
 * Enter a new scope
 */
void enter_scope(SymbolTable *table) {
    if (table == NULL) {
        return;
    }
    
    Scope *new_scope = (Scope *)malloc(sizeof(Scope));
    if (new_scope == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new scope\n");
        return;
    }
    
    new_scope->symbols = (SymbolEntry *)malloc(50 * sizeof(SymbolEntry));
    if (new_scope->symbols == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(new_scope);
        return;
    }
    
    new_scope->symbol_count = 0;
    new_scope->capacity = 50;
    new_scope->parent = table->current_scope;
    
    table->current_scope = new_scope;
}

/**
 * Exit current scope
 */
void exit_scope(SymbolTable *table) {
    if (table == NULL || table->current_scope == NULL) {
        return;
    }
    
    if (table->current_scope == table->global_scope) {
        return; /* Don't exit global scope */
    }
    
    Scope *temp = table->current_scope;
    table->current_scope = temp->parent;
    
    /* Free the exited scope */
    for (int i = 0; i < temp->symbol_count; i++) {
        free(temp->symbols[i].name);
    }
    free(temp->symbols);
    free(temp);
}

/**
 * Count total variables
 */
int count_variables(SymbolTable *table) {
    if (table == NULL) {
        return 0;
    }
    return table->total_variables;
}

/**
 * Count total functions
 */
int count_functions(SymbolTable *table) {
    if (table == NULL) {
        return 0;
    }
    return table->total_functions;
}

/**
 * Print the symbol table
 */
void print_symbol_table(SymbolTable *table) {
    if (table == NULL || table->global_scope == NULL) {
        return;
    }
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║            SYMBOL TABLE - VARIABLES                ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");
    
    printf("%-20s %-20s %-15s %s\n", "Variable Name", "Type", "Array Size", "Declared at Line");
    printf("%-20s %-20s %-15s %s\n", "─────────────", "────", "──────────", "────────────────");
    
    Scope *scope = table->global_scope;
    int count = 0;
    
    while (scope != NULL) {
        for (int i = 0; i < scope->symbol_count; i++) {
            SymbolEntry *entry = &scope->symbols[i];
            if (!entry->is_function) {
                printf("%-20s %-20s", entry->name, datatype_to_string(entry->type));
                if (entry->is_array) {
                    printf("%-15d ", entry->array_size);
                } else {
                    printf("%-15s ", "-");
                }
                printf("%d\n", entry->line_declared);
                count++;
            }
        }
        scope = scope->parent;
    }
    
    printf("\nTotal Variables Declared: %d\n\n", count);
}

/**
 * Free symbol table
 */
void free_symbol_table(SymbolTable *table) {
    if (table == NULL) {
        return;
    }
    
    /* Free all scopes */
    Scope *scope = table->current_scope;
    while (scope != NULL) {
        Scope *parent = scope->parent;
        
        /* Free symbols in this scope */
        for (int i = 0; i < scope->symbol_count; i++) {
            free(scope->symbols[i].name);
        }
        free(scope->symbols);
        
        if (scope != table->global_scope) {
            free(scope);
        }
        
        scope = parent;
    }
    
    /* Free global scope */
    if (table->global_scope != NULL) {
        for (int i = 0; i < table->global_scope->symbol_count; i++) {
            free(table->global_scope->symbols[i].name);
        }
        free(table->global_scope->symbols);
        free(table->global_scope);
    }
    
    free(table);
}
