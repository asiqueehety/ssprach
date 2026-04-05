#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "symbol_table.h"

/* Semantic analysis result structure */
typedef struct {
    int total_errors;
    int total_warnings;
    int undeclared_variables;
    int type_mismatches;
    int uninitialized_variables;
    int array_out_of_bounds;
} SemanticResult;

/**
 * Initialize semantic analysis
 */
void init_semantic_analyzer(void);

/**
 * Perform semantic analysis
 * @param table Symbol table
 * @return Semantic analysis result
 */
SemanticResult get_semantic_results(SymbolTable *table);

/**
 * Report a semantic error
 * @param line Line number
 * @param message Error message
 */
void report_semantic_error(int line, const char *message);

/**
 * Report a semantic warning
 * @param line Line number
 * @param message Warning message
 */
void report_semantic_warning(int line, const char *message);

/**
 * Check if two data types are compatible
 * @param type1 First type
 * @param type2 Second type
 * @return 1 if compatible, 0 otherwise
 */
int types_compatible(DataType type1, DataType type2);

/**
 * Get string representation of data type
 * @param type Data type
 * @return String representation
 */
const char* get_type_name(DataType type);

/**
 * Validate expression type
 * @param table Symbol table
 * @param expected Expected type
 * @param actual Actual type
 * @param line Line number
 */
void validate_expression_type(SymbolTable *table, DataType expected, DataType actual, int line);

#endif /* SEMANTIC_ANALYZER_H */
