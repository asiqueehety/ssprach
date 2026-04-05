#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic_analyzer.h"
#include "symbol_table.h"

/* Static variables for error tracking */
static int semantic_errors = 0;
static int semantic_warnings = 0;
static int undeclared_var_count = 0;
static int type_mismatch_count = 0;
static int uninitialized_var_count = 0;

/**
 * Initialize semantic analysis
 */
void init_semantic_analyzer(void) {
    semantic_errors = 0;
    semantic_warnings = 0;
    undeclared_var_count = 0;
    type_mismatch_count = 0;
    uninitialized_var_count = 0;
}

/**
 * Report a semantic error
 */
void report_semantic_error(int line, const char *message) {
    printf("[Line %d] Semantic Error: %s\n", line, message);
    semantic_errors++;
}

/**
 * Report a semantic warning
 */
void report_semantic_warning(int line, const char *message) {
    printf("[Line %d] Warning: %s\n", line, message);
    semantic_warnings++;
}

/**
 * Get string representation of data type
 */
const char* get_type_name(DataType type) {
    switch (type) {
        case TYPE_INT: return "int (GANZ)";
        case TYPE_DOUBLE: return "double (KOMM)";
        case TYPE_STRING: return "string (TEXT)";
        case TYPE_CHAR: return "char (ZEICHEN)";
        case TYPE_ARRAY: return "array";
        default: return "unknown";
    }
}

/**
 * Check if two data types are compatible
 */
int types_compatible(DataType type1, DataType type2) {
    if (type1 == type2) {
        return 1;
    }
    
    /* Allow some implicit conversions */
    if ((type1 == TYPE_INT && type2 == TYPE_DOUBLE) ||
        (type1 == TYPE_DOUBLE && type2 == TYPE_INT)) {
        return 1;
    }
    
    return 0;
}

/**
 * Validate expression type
 */
void validate_expression_type(SymbolTable *table, DataType expected, DataType actual, int line) {
    if (expected == TYPE_UNKNOWN || actual == TYPE_UNKNOWN) {
        return;
    }
    
    if (!types_compatible(expected, actual)) {
        char message[256];
        snprintf(message, sizeof(message), 
                 "Type mismatch: expected %s, got %s",
                 get_type_name(expected), 
                 get_type_name(actual));
        report_semantic_error(line, message);
        type_mismatch_count++;
    }
}

/**
 * Get semantic analysis results
 */
SemanticResult get_semantic_results(SymbolTable *table) {
    SemanticResult result;
    
    result.total_errors = semantic_errors;
    result.total_warnings = semantic_warnings;
    result.undeclared_variables = undeclared_var_count;
    result.type_mismatches = type_mismatch_count;
    result.uninitialized_variables = uninitialized_var_count;
    result.array_out_of_bounds = 0; /* Can be expanded */
    
    return result;
}

/**
 * Print semantic analysis report
 */
void print_semantic_report(void) {
    if (semantic_errors > 0 || semantic_warnings > 0) {
        printf("\n╔═══════════════════════════════════════╗\n");
        printf("║      SEMANTIC ANALYSIS REPORT       ║\n");
        printf("╚═══════════════════════════════════════╝\n\n");
        
        if (semantic_errors > 0) {
            printf("Total Semantic Errors: %d\n", semantic_errors);
        }
        if (semantic_warnings > 0) {
            printf("Total Warnings: %d\n", semantic_warnings);
        }
        printf("\n");
    }
}
