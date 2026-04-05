%{%{%{%{%{%{

#include <stdio.h>

#include <stdlib.h>#include <stdio.h>

#include "symbol_table.h"

#include "semantic_analyzer.h"#include <stdlib.h>#include <stdio.h>



extern int yylex(void);#include <string.h>

extern int line_num;

void yyerror(const char *s);#include <math.h>#include <stdlib.h>#include <stdio.h>



int error_count = 0;#include "symbol_table.h"

SymbolTable *global_symbol_table = NULL;

#include "semantic_analyzer.h"#include <string.h>

DataType token_to_datatype(int tok) {

    if (tok == INT_TYPE) return TYPE_INT;

    if (tok == DOUBLE_TYPE) return TYPE_DOUBLE;

    if (tok == STRING_TYPE) return TYPE_STRING;extern int yylex(void);#include <math.h>#include <stdlib.h>#include <stdio.h>#include <stdio.h>

    if (tok == CHAR_TYPE) return TYPE_CHAR;

    return TYPE_UNKNOWN;extern int line_num;

}

%}void yyerror(const char *s);#include "symbol_table.h"



%union {

    char *str;

    int val;int error_count = 0;#include "semantic_analyzer.h"#include <string.h>

    double dval;

    char cval;SymbolTable *global_symbol_table = NULL;

    int ttype;

}



%token WENN SONST FUR WAEHREND WAHL FALL STANDARD LEERE RETURNDataType token_to_datatype(int token) {

%token <ttype> INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE

%token AND OR NOT    switch(token) {extern int yylex(void);#include "symbol_table.h"#include <stdlib.h>#include <stdlib.h>

%token INPUT OUTPUT

%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT        case INT_TYPE: return TYPE_INT;

%token MATHE_SIN MATHE_COS MATHE_TAN QUADRAT_WURZEL MATHE_LOG

%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN        case DOUBLE_TYPE: return TYPE_DOUBLE;extern int line_num;

%token PLUS MINUS STAR SLASH MOD

%token EQ NEQ LT GT LE GE        case STRING_TYPE: return TYPE_STRING;

%token <str> ID

%token <val> INT_VAL        case CHAR_TYPE: return TYPE_CHAR;extern FILE *yyin;#include "semantic_analyzer.h"

%token <dval> DOUBLE_VAL

%token <str> STRING_VAL        default: return TYPE_UNKNOWN;

%token <cval> CHAR_VAL

    }

%left OR

%left AND}

%left PLUS MINUS

%left STAR SLASH MOD%}void yyerror(const char *s);#include <string.h>



%start program



%%%union {



program: program stmt | ;    char *string;



stmt: WENN LPAREN expr RPAREN block    int integer;int error_count = 0;extern int yylex(void);

    | WAEHREND LPAREN expr RPAREN block

    | FUR LPAREN stmt SEMICOLON expr SEMICOLON stmt RPAREN block    double floating;

    | block

    | INPUT ID SEMICOLON    char character;SymbolTable *global_symbol_table = NULL;

    | OUTPUT expr SEMICOLON

    | decl SEMICOLON    int token_type;

    | assign SEMICOLON

    ;}extern int line_num;extern int yylex(void);#include "symbol_table.h"



block: LBRACE stmt_list RBRACE | LBRACE RBRACE ;



stmt_list: stmt | stmt_list stmt ;%token WENN SONST FUR IN WAEHREND WAHL FALL STANDARD LEERE RETURNDataType token_to_datatype(int token) {



decl: type ID { add_variable(global_symbol_table, $2, token_to_datatype($1), line_num); }%token <token_type> INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE

    | type ID ASSIGN expr { add_variable(global_symbol_table, $2, token_to_datatype($1), line_num); }

    | type ID LBRACKET INT_VAL RBRACKET { add_array(global_symbol_table, $2, token_to_datatype($1), $4, line_num); }%token AND OR NOT    switch(token) {extern FILE *yyin;

    ;

%token BIT_AND BIT_OR BIT_XOR BIT_NOT SHIFT_LEFT SHIFT_RIGHT

type: INT_TYPE | DOUBLE_TYPE | STRING_TYPE | CHAR_TYPE ;

%token INPUT OUTPUT        case INT_TYPE: return TYPE_INT;

assign: ID ASSIGN expr { check_variable_declared(global_symbol_table, $1, line_num); }

      | ID LBRACKET expr RBRACKET ASSIGN expr { analyze_array_access(global_symbol_table, $1, line_num); }%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT

      ;

%token MATHE_SIN MATHE_COS MATHE_TAN QUADRAT_WURZEL MATHE_LOG        case DOUBLE_TYPE: return TYPE_DOUBLE;extern int line_num;#include "semantic_analyzer.h"

expr: expr PLUS expr

    | expr MINUS expr%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN COLON

    | expr STAR expr

    | expr SLASH expr%token PLUS MINUS STAR SLASH MOD        case STRING_TYPE: return TYPE_STRING;

    | expr MOD expr

    | expr AND expr%token EQ NEQ LT GT LE GE

    | expr OR expr

    | expr EQ expr%token <string> ID        case CHAR_TYPE: return TYPE_CHAR;void yyerror(const char *s);

    | expr NEQ expr

    | expr LT expr%token <integer> INT_VAL

    | expr GT expr

    | expr LE expr%token <floating> DOUBLE_VAL        default: return TYPE_UNKNOWN;

    | expr GE expr

    | ID { check_variable_declared(global_symbol_table, $1, line_num); }%token <string> STRING_VAL

    | INT_VAL

    | DOUBLE_VAL%token <character> CHAR_VAL    }extern FILE *yyin;

    | STRING_VAL

    | CHAR_VAL

    | LPAREN expr RPAREN

    | NOT expr%left OR}

    | MINUS expr

    ;%left AND



%%%left BIT_ORint error_count = 0;



void yyerror(const char *s) {%left BIT_XOR

    fprintf(stderr, "[Line %d] Error: %s\n", line_num, s);

    error_count++;%left BIT_AND%}

}

%left EQ NEQ

int main(int argc, char *argv[]) {

    printf("\nSPRACH Compiler v1.0\n");%left LT GT LE GESymbolTable *global_symbol_table = NULL;int yylex(void);

    printf("German Programming Language\n\n");

    %left SHIFT_LEFT SHIFT_RIGHT

    global_symbol_table = create_symbol_table();

    if (!global_symbol_table) {%left PLUS MINUS%union {

        fprintf(stderr, "Fatal: Symbol table creation failed\n");

        return 1;%left STAR SLASH MOD

    }

    %right NOT BIT_NOT    char *string;

    printf("Parsing...\n");

    int result = yyparse();%right UMINUS

    

    printf("\n=== RESULTS ===\n");    int integer;

    if (result == 0 && error_count == 0) {

        printf("SUCCESS: Compilation passed\n");%start program

    } else {

        printf("FAILED: %d errors\n", error_count);    double floating;/* Helper function to convert token to DataType */void yyerror(const char *s);void yyerror(const char *s);

    }

    %%

    SemanticResult sem = get_semantic_results(global_symbol_table);

    printf("Variables: %d\n", count_variables(global_symbol_table));    char character;

    print_symbol_table(global_symbol_table);

    program

    free_symbol_table(global_symbol_table);

    return (error_count + sem.total_errors) ? 1 : 0;    : program declaration SEMICOLON    int token_type;DataType token_to_datatype(int token) {

}

    | program func_def

    | program statement}

    |

    ;    switch(token) {%}



declaration%token WENN SONST_WENN SONST FUR IN WAEHREND WAHL FALL STANDARD LEERE RETURN

    : type ID

    {%token <token_type> INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE        case INT_TYPE: return TYPE_INT;

        if (global_symbol_table != NULL) {

            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);%token AND OR NOT

        }

    }%token BIT_AND BIT_OR BIT_XOR BIT_NOT SHIFT_LEFT SHIFT_RIGHT        case DOUBLE_TYPE: return TYPE_DOUBLE;extern int line_num;

    | type ID ASSIGN expression

    {%token INPUT OUTPUT

        if (global_symbol_table != NULL) {

            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT        case STRING_TYPE: return TYPE_STRING;

        }

    }%token MATHE_SIN MATHE_COS MATHE_TAN QUADRAT_WURZEL MATHE_LOG

    | type ID LBRACKET INT_VAL RBRACKET

    {        case CHAR_TYPE: return TYPE_CHAR;%token WENN SONST FUR IN WAEHREND WAHL FALL STANDARD LEERE RETURNint error_count = 0;

        if (global_symbol_table != NULL) {

            add_array(global_symbol_table, $2, token_to_datatype($1), $4, line_num);%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN COLON

        }

    }%token PLUS MINUS STAR SLASH MOD        default: return TYPE_UNKNOWN;

    ;

%token EQ NEQ LT GT LE GE

type

    : INT_TYPE { $$ = INT_TYPE; }    }%token INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPESymbolTable *global_symbol_table = NULL;

    | DOUBLE_TYPE { $$ = DOUBLE_TYPE; }

    | STRING_TYPE { $$ = STRING_TYPE; }%token <string> ID

    | CHAR_TYPE { $$ = CHAR_TYPE; }

    ;%token <integer> INT_VAL}



func_def%token <floating> DOUBLE_VAL

    : type ID LPAREN RPAREN LBRACE stmt_list RBRACE

    | LEERE ID LPAREN RPAREN LBRACE stmt_list RBRACE%token <string> STRING_VAL%token AND OR NOT

    ;

%token <character> CHAR_VAL

stmt_list

    : statement%}

    | stmt_list statement

    |%left OR

    ;

%left AND%token BIT_AND BIT_OR BIT_XOR BIT_NOT SHIFT_LEFT SHIFT_RIGHT/* Helper function to convert token to DataType */

statement

    : simple_stmt SEMICOLON%left BIT_OR

    | compound_stmt

    ;%left BIT_XOR%union {



simple_stmt%left BIT_AND

    : declaration

    | assignment%left EQ NEQ    char *string;%token INPUT OUTPUTDataType token_to_datatype(int token) {

    | input_stmt

    | output_stmt%left LT GT LE GE

    | array_op

    | function_call%left SHIFT_LEFT SHIFT_RIGHT    int integer;

    | return_stmt

    ;%left PLUS MINUS



compound_stmt%left STAR SLASH MOD    double floating;%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT    switch(token) {

    : if_stmt

    | if_else_stmt%right NOT BIT_NOT

    | for_stmt

    | while_stmt%right UMINUS    char character;

    | switch_stmt

    | block

    ;

%start program    int token_type;%token MATHE_SIN MATHE_COS MATHE_TAN QUADRAT_WURZEL MATHE_LOG        case INT_TYPE: return TYPE_INT;

assignment

    : ID ASSIGN expression

    {

        if (global_symbol_table != NULL) {%%    DataType data_type;

            check_variable_declared(global_symbol_table, $1, line_num);

        }

    }

    | ID LBRACKET expression RBRACKET ASSIGN expressionprogram}        case DOUBLE_TYPE: return TYPE_DOUBLE;

    {

        if (global_symbol_table != NULL) {    : program declaration SEMICOLON

            analyze_array_access(global_symbol_table, $1, line_num);

        }    | program func_def

    }

    ;    | program statement



return_stmt    |%token WENN SONST_WENN SONST FUR IN WAEHREND WAHL FALL STANDARD LEERE RETURN%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN COLON        case STRING_TYPE: return TYPE_STRING;

    : RETURN

    | RETURN expression    ;

    ;

%token <token_type> INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE

input_stmt

    : INPUT IDdeclaration

    {

        if (global_symbol_table != NULL) {    : type ID%token AND OR NOT%token PLUS MINUS STAR SLASH MOD        case CHAR_TYPE: return TYPE_CHAR;

            check_variable_declared(global_symbol_table, $2, line_num);

        }    {

    }

    ;        if (global_symbol_table != NULL) {%token BIT_AND BIT_OR BIT_XOR BIT_NOT SHIFT_LEFT SHIFT_RIGHT



output_stmt            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);

    : OUTPUT expression

    ;        }%token INPUT OUTPUT%token EQ NEQ LT GT LE GE        default: return TYPE_UNKNOWN;



array_op    }

    : ID ARRAY_ADD expression

    {    | type ID ASSIGN expression%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    {

        }

    }        if (global_symbol_table != NULL) {%token MATHE_SIN MATHE_COS MATHE_TAN QUADRAT_WURZEL MATHE_LOG%token ID    }

    | ID ARRAY_REMOVE

    {            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);        }

        }

    }    }

    | ID ARRAY_SORT

    {    | type ID LBRACKET INT_VAL RBRACKET%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN COLON%token INT_VAL DOUBLE_VAL STRING_VAL CHAR_VAL}

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    {

        }

    }        if (global_symbol_table != NULL) {

    ;

            add_array(global_symbol_table, $2, token_to_datatype($1), $4, line_num);

if_stmt

    : WENN LPAREN expression RPAREN block        }%token PLUS MINUS STAR SLASH MOD

    ;

    }

if_else_stmt

    : WENN LPAREN expression RPAREN block SONST block    ;%token EQ NEQ LT GT LE GE

    ;



while_stmt

    : WAEHREND LPAREN expression RPAREN blocktype%left OR%}

    ;

    : INT_TYPE { $$ = INT_TYPE; }

for_stmt

    : FUR LPAREN assignment SEMICOLON expression SEMICOLON assignment RPAREN block    | DOUBLE_TYPE { $$ = DOUBLE_TYPE; }%token <string> ID

    ;

    | STRING_TYPE { $$ = STRING_TYPE; }

switch_stmt

    : WAHL LPAREN expression RPAREN LBRACE cases RBRACE    | CHAR_TYPE { $$ = CHAR_TYPE; }%token <integer> INT_VAL%left AND

    ;

    ;

cases

    : case_stmt%token <floating> DOUBLE_VAL

    | cases case_stmt

    ;func_def



case_stmt    : type ID LPAREN RPAREN LBRACE stmt_list RBRACE%token <string> STRING_VAL%left BIT_OR%union {

    : FALL expression COLON stmt_list

    ;    {



block        if (global_symbol_table != NULL) {%token <character> CHAR_VAL

    : LBRACE stmt_list RBRACE

    | LBRACE RBRACE            enter_scope(global_symbol_table);

    ;

        }%left BIT_XOR    char *string;

function_call

    : ID LPAREN RPAREN    }

    {

        if (global_symbol_table != NULL) {    | LEERE ID LPAREN RPAREN LBRACE stmt_list RBRACE%left OR

            analyze_function_call(global_symbol_table, $1, 0, line_num);

        }    {

    }

    | ID LPAREN expr_list RPAREN        if (global_symbol_table != NULL) {%left AND%left BIT_AND    int integer;

    {

        if (global_symbol_table != NULL) {            enter_scope(global_symbol_table);

            analyze_function_call(global_symbol_table, $1, 1, line_num);

        }        }%left BIT_OR

    }

    | math_function    }

    ;

    ;%left BIT_XOR%left EQ NEQ    double floating;

math_function

    : MATHE_SIN LPAREN expression RPAREN

    | MATHE_COS LPAREN expression RPAREN

    | MATHE_TAN LPAREN expression RPARENstmt_list%left BIT_AND

    | QUADRAT_WURZEL LPAREN expression RPAREN

    | MATHE_LOG LPAREN expression RPAREN    : statement

    ;

    | stmt_list statement%left EQ NEQ%left LT GT LE GE    char character;

expr_list

    : expression    |

    | expr_list COMMA expression

    ;    ;%left LT GT LE GE



expression

    : term

    | expression PLUS termstatement%left SHIFT_LEFT SHIFT_RIGHT%left SHIFT_LEFT SHIFT_RIGHT    int token_type;

    | expression MINUS term

    | expression AND expression    : simple_stmt SEMICOLON

    | expression OR expression

    | NOT expression    | compound_stmt%left PLUS MINUS

    ;

    ;

term

    : factor%left STAR SLASH MOD%left PLUS MINUS}

    | term STAR factor

    | term SLASH factorsimple_stmt

    | term MOD factor

    | term EQ factor    : declaration%right NOT BIT_NOT

    | term NEQ factor

    | term LT factor    | assignment

    | term GT factor

    | term LE factor    | input_stmt%right UMINUS%left STAR SLASH MOD

    | term GE factor

    | term BIT_AND factor    | output_stmt

    | term BIT_OR factor

    | term BIT_XOR factor    | array_op

    | term SHIFT_LEFT factor

    | term SHIFT_RIGHT factor    | function_call

    ;

    | return_stmt%start program%right NOT BIT_NOT%token WENN SONST FUR WAEHREND WAHL FALL STANDARD LEERE RETURN

factor

    : ID    ;

    {

        if (global_symbol_table != NULL) {

            check_variable_declared(global_symbol_table, $1, line_num);

        }compound_stmt

    }

    | ID LBRACKET expression RBRACKET    : if_stmt%%%token <token_type> INT_TYPE DOUBLE_TYPE STRING_TYPE CHAR_TYPE

    {

        if (global_symbol_table != NULL) {    | if_else_stmt

            analyze_array_access(global_symbol_table, $1, line_num);

        }    | for_stmt

    }

    | INT_VAL    | while_stmt

    | DOUBLE_VAL

    | STRING_VAL    | switch_stmt/*==== PROGRAM STRUCTURE ====*/%start program%token AND OR NOT

    | CHAR_VAL

    | function_call    | block

    | LPAREN expression RPAREN

    | MINUS factor %prec UMINUS    ;program

    | BIT_NOT factor

    ;



%%assignment    : program declaration SEMICOLON%token INPUT OUTPUT



void yyerror(const char *s) {    : ID ASSIGN expression

    fprintf(stderr, "\n[Line %d] Syntax Error: %s\n", line_num, s);

    error_count++;    {    | program func_def

}

        if (global_symbol_table != NULL) {

int main(int argc, char *argv[]) {

    printf("\n╔═════════════════════════════════════════╗\n");            check_variable_declared(global_symbol_table, $1, line_num);    | program statement%%%token ARRAY_ADD ARRAY_REMOVE ARRAY_SORT

    printf("║  SPRACH COMPILER v1.0                  ║\n");

    printf("║  German-Inspired Programming Language  ║\n");        }

    printf("╚═════════════════════════════════════════╝\n\n");

        }    |

    global_symbol_table = create_symbol_table();

        | ID LBRACKET expression RBRACKET ASSIGN expression

    if (global_symbol_table == NULL) {

        fprintf(stderr, "Error: Failed to initialize symbol table\n");    {    ;%token LBRACE RBRACE LPAREN RPAREN SEMICOLON COMMA LBRACKET RBRACKET ASSIGN COLON

        return 1;

    }        if (global_symbol_table != NULL) {

    

    printf("[Phase 1] Lexical and Syntax Analysis...\n\n");            analyze_array_access(global_symbol_table, $1, line_num);

    

    int parse_result = yyparse();        }

    

    printf("\n╔═════════════════════════════════════════╗\n");    }/*==== DECLARATIONS ====*/program

    printf("║           RESULTS                      ║\n");

    printf("╚═════════════════════════════════════════╝\n\n");    ;

    

    if (parse_result == 0 && error_count == 0) {declaration

        printf("✓ Analysis: PASSED\n");

    } else {return_stmt

        printf("✗ Analysis: FAILED (%d errors)\n", error_count);

    }    : RETURN    : type ID    : items%token <string> ARITH_OP REL_OP

    

    SemanticResult semantic_result = get_semantic_results(global_symbol_table);    | RETURN expression

    

    printf("  Variables: %d\n", count_variables(global_symbol_table));    ;    {

    print_symbol_table(global_symbol_table);

    

    int total_errors = error_count + semantic_result.total_errors;

    input_stmt        if (global_symbol_table != NULL) {    ;%token <string> ID

    printf("\n");

    if (total_errors == 0) {    : INPUT ID

        printf("✓✓✓ COMPILATION SUCCESSFUL ✓✓✓\n");

    } else {    {            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);

        printf("✗ COMPILATION FAILED (%d total errors)\n", total_errors);

    }        if (global_symbol_table != NULL) {

    printf("\n");

                check_variable_declared(global_symbol_table, $2, line_num);        }%token <integer> INT_VAL

    free_symbol_table(global_symbol_table);

    return (total_errors == 0) ? 0 : 1;        }

}

    }    }

    ;

    | type ID ASSIGN expressionitems%token <floating> DOUBLE_VAL

output_stmt

    : OUTPUT expression    {

    ;

        if (global_symbol_table != NULL) {    : /* empty */%token <string> STRING_VAL

array_op

    : ID ARRAY_ADD expression            add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);

    {

        if (global_symbol_table != NULL) {            check_type_compatibility(global_symbol_table, $2, line_num);    | items item%token <character> CHAR_VAL

            analyze_array_access(global_symbol_table, $1, line_num);

        }        }

    }

    | ID ARRAY_REMOVE    }    ;

    {

        if (global_symbol_table != NULL) {    | type ID LBRACKET INT_VAL RBRACKET

            analyze_array_access(global_symbol_table, $1, line_num);

        }    {%start program

    }

    | ID ARRAY_SORT        if (global_symbol_table != NULL) {

    {

        if (global_symbol_table != NULL) {            add_array(global_symbol_table, $2, token_to_datatype($1), $4, line_num);item

            analyze_array_access(global_symbol_table, $1, line_num);

        }        }

    }

    ;    }    : statement%%



if_stmt    | type var_list

    : WENN LPAREN expression RPAREN block

    ;    {    | func_def



if_else_stmt        /* Multiple variable declarations */

    : WENN LPAREN expression RPAREN block SONST block

    ;    }    ;program



while_stmt    ;

    : WAEHREND LPAREN expression RPAREN block

    ;    : program statement



for_stmtvar_list

    : FUR LPAREN assignment SEMICOLON expression SEMICOLON assignment RPAREN block

    ;    : ID COMMA IDfunc_def    | program func_def



switch_stmt    {

    : WAHL LPAREN expression RPAREN LBRACE cases RBRACE

    ;        if (global_symbol_table != NULL) {    : type ID LPAREN param_list_opt RPAREN block    |



cases            add_variable(global_symbol_table, $1, TYPE_INT, line_num);

    : case_stmt

    | cases case_stmt            add_variable(global_symbol_table, $3, TYPE_INT, line_num);    | LEERE ID LPAREN param_list_opt RPAREN block    ;

    ;

        }

case_stmt

    : FALL expression COLON stmt_list    }    ;

    ;

    | var_list COMMA ID

block

    : LBRACE stmt_list RBRACE    {/* Function definition */

    | LBRACE RBRACE

    ;        if (global_symbol_table != NULL) {



function_call            add_variable(global_symbol_table, $3, TYPE_INT, line_num);param_list_optfunc_def

    : ID LPAREN RPAREN

    {        }

        if (global_symbol_table != NULL) {

            analyze_function_call(global_symbol_table, $1, 0, line_num);    }    : /* empty */    : type ID LPAREN param_list RPAREN LBRACE stmt_list RBRACE

        }

    }    ;

    | ID LPAREN expr_list RPAREN

    {    | param_list    {

        if (global_symbol_table != NULL) {

            analyze_function_call(global_symbol_table, $1, 1, line_num);type

        }

    }    : INT_TYPE { $$ = INT_TYPE; }    ;        if (global_symbol_table != NULL) {

    | math_function

    ;    | DOUBLE_TYPE { $$ = DOUBLE_TYPE; }



math_function    | STRING_TYPE { $$ = STRING_TYPE; }            analyze_function_declaration(global_symbol_table, $2, 

    : MATHE_SIN LPAREN expression RPAREN

    | MATHE_COS LPAREN expression RPAREN    | CHAR_TYPE { $$ = CHAR_TYPE; }

    | MATHE_TAN LPAREN expression RPAREN

    | QUADRAT_WURZEL LPAREN expression RPAREN    ;param_list                                        token_to_datatype($1), NULL, 0, line_num);

    | MATHE_LOG LPAREN expression RPAREN

    ;



expr_list/*==== FUNCTION DEFINITIONS ====*/    : param            enter_scope(global_symbol_table);

    : expression

    | expr_list COMMA expressionfunc_def

    ;

    : type ID LPAREN param_list RPAREN LBRACE stmt_list RBRACE    | param_list COMMA param        }

expression

    : term    {

    | expression PLUS term

    | expression MINUS term        if (global_symbol_table != NULL) {    ;    }

    | expression AND expression

    | expression OR expression            enter_scope(global_symbol_table);

    | NOT expression

    ;        }    | type ID LPAREN RPAREN LBRACE stmt_list RBRACE



term    }

    : factor

    | term STAR factor    | type ID LPAREN RPAREN LBRACE stmt_list RBRACEparam    {

    | term SLASH factor

    | term MOD factor    {

    | term EQ factor

    | term NEQ factor        if (global_symbol_table != NULL) {    : type ID        if (global_symbol_table != NULL) {

    | term LT factor

    | term GT factor            enter_scope(global_symbol_table);

    | term LE factor

    | term GE factor        }    | type ID LBRACKET RBRACKET            analyze_function_declaration(global_symbol_table, $2,

    | term BIT_AND factor

    | term BIT_OR factor    }

    | term BIT_XOR factor

    | term SHIFT_LEFT factor    | LEERE ID LPAREN param_list RPAREN LBRACE stmt_list RBRACE    ;                                        token_to_datatype($1), NULL, 0, line_num);

    | term SHIFT_RIGHT factor

    ;    {



factor        if (global_symbol_table != NULL) {            enter_scope(global_symbol_table);

    : ID

    {            enter_scope(global_symbol_table);

        if (global_symbol_table != NULL) {

            check_variable_declared(global_symbol_table, $1, line_num);        }block        }

        }

    }    }

    | ID LBRACKET expression RBRACKET

    {    | LEERE ID LPAREN RPAREN LBRACE stmt_list RBRACE    : LBRACE stmt_list RBRACE    }

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    {

        }

    }        if (global_symbol_table != NULL) {    ;    ;

    | INT_VAL

    | DOUBLE_VAL            enter_scope(global_symbol_table);

    | STRING_VAL

    | CHAR_VAL        }

    | function_call

    | LPAREN expression RPAREN    }

    | MINUS factor %prec UMINUS

    | BIT_NOT factor    ;stmt_listparam_list

    ;



%%

param_list    : /* empty */    : param_list COMMA type ID

void yyerror(const char *s) {

    fprintf(stderr, "\n");    : param

    fprintf(stderr, "╔════════════════════════════════════════╗\n");

    fprintf(stderr, "║       SYNTAX ERROR DETECTED            ║\n");    | param_list COMMA param    | stmt_list statement    | type ID

    fprintf(stderr, "╚════════════════════════════════════════╝\n");

    fprintf(stderr, "[Line %d] %s\n", line_num, s);    ;

    fprintf(stderr, "\n");

    error_count++;    ;    ;

}

param

int main(int argc, char *argv[]) {

    printf("\n");    : type ID

    printf("╔═════════════════════════════════════════╗\n");

    printf("║  SPRACH COMPILER v1.0                  ║\n");    {

    printf("║  German-Inspired Programming Language  ║\n");

    printf("║  Lexical, Syntax & Semantic Analysis   ║\n");        if (global_symbol_table != NULL) {statementstatement

    printf("╚═════════════════════════════════════════╝\n\n");

                add_variable(global_symbol_table, $2, token_to_datatype($1), line_num);

    global_symbol_table = create_symbol_table();

            }    : declaration SEMICOLON    : simple_stmt SEMICOLON

    if (global_symbol_table == NULL) {

        fprintf(stderr, "Error: Failed to initialize symbol table\n");    }

        return 1;

    }    | type ID LBRACKET RBRACKET    | assignment SEMICOLON    | compound_stmt

    

    printf("[Phase 1] Initializing compiler...\n");    {

    printf("[Phase 2] Starting lexical and syntax analysis...\n\n");

            if (global_symbol_table != NULL) {    | output_stmt SEMICOLON    ;

    int parse_result = yyparse();

                add_array(global_symbol_table, $2, token_to_datatype($1), -1, line_num);

    printf("\n╔═════════════════════════════════════════╗\n");

    printf("║           COMPILATION RESULTS          ║\n");        }    | input_stmt SEMICOLON

    printf("╚═════════════════════════════════════════╝\n\n");

        }

    if (parse_result == 0 && error_count == 0) {

        printf("✓ Lexical Analysis:    PASSED\n");    ;    | array_stmt SEMICOLONsimple_stmt

        printf("✓ Syntax Analysis:     PASSED\n");

    } else {

        printf("✗ Lexical Analysis:    FAILED\n");

        printf("✗ Syntax Analysis:     FAILED\n");/*==== STATEMENTS ====*/    | func_call SEMICOLON    : declaration

        printf("  └─ Total Syntax Errors: %d\n", error_count);

    }stmt_list

    

    printf("\n[Phase 3] Performing semantic analysis...\n");    : statement    | return_stmt SEMICOLON    | assignment

    

    SemanticResult semantic_result;    | stmt_list statement

    if (global_symbol_table != NULL) {

        semantic_result = get_semantic_results(global_symbol_table);    |    | if_stmt    | input_stmt

        

        printf("\n✓ Semantic Analysis:   PASSED\n");    ;

        printf("  ├─ Variables Declared: %d\n", count_variables(global_symbol_table));

        printf("  ├─ Functions Defined:  %d\n", count_functions(global_symbol_table));    | while_stmt    | output_stmt

        printf("  └─ Semantic Errors:    %d\n", semantic_result.total_errors);

    }statement

    

    if (global_symbol_table != NULL) {    : simple_stmt SEMICOLON    | for_stmt    | array_op

        printf("\n");

        print_symbol_table(global_symbol_table);    | compound_stmt

    }

        ;    | switch_stmt    | function_call

    printf("\n╔═════════════════════════════════════════╗\n");

    printf("║              FINAL RESULT              ║\n");

    printf("╚═════════════════════════════════════════╝\n\n");

    simple_stmt    | block    | return_stmt

    int total_errors = error_count + semantic_result.total_errors;

        : declaration

    if (total_errors == 0 && semantic_result.total_warnings == 0) {

        printf("✓✓✓ COMPILATION SUCCESSFUL ✓✓✓\n");    | assignment    ;    ;

        printf("  ├─ No syntax errors\n");

        printf("  ├─ No semantic errors\n");    | input_stmt

        printf("  └─ No warnings\n");

    } else if (total_errors == 0) {    | output_stmt

        printf("✓ COMPILATION SUCCESSFUL (with warnings)\n");

        printf("  ├─ No syntax errors\n");    | array_op

        printf("  ├─ No semantic errors\n");

        printf("  └─ Warnings: %d\n", semantic_result.total_warnings);    | function_calldeclarationreturn_stmt

    } else {

        printf("✗ COMPILATION FAILED\n");    | return_stmt

        printf("  ├─ Syntax Errors:  %d\n", error_count);

        printf("  ├─ Semantic Errors: %d\n", semantic_result.total_errors);    ;    : type ID    : LEERE

        printf("  └─ Warnings: %d\n", semantic_result.total_warnings);

    }

    

    printf("\n╚═════════════════════════════════════════╝\n\n");compound_stmt    | type ID ASSIGN expression    | LEERE expression

    

    if (global_symbol_table != NULL) {    : if_stmt

        free_symbol_table(global_symbol_table);

    }    | if_else_stmt    | type ID LBRACKET RBRACKET    ;

    

    return (total_errors == 0) ? 0 : 1;    | for_stmt

}

    | while_stmt    | type ID LBRACKET expression RBRACKET

    | switch_stmt

    | block    ;compound_stmt

    ;

    : if_stmt

/*==== ASSIGNMENTS ====*/

assignmentassignment    | if_else_stmt

    : ID ASSIGN expression

    {    : ID ASSIGN expression    | for_stmt

        if (global_symbol_table != NULL) {

            check_variable_declared(global_symbol_table, $1, line_num);    | ID LBRACKET expression RBRACKET ASSIGN expression    | while_stmt

        }

    }    ;    | switch_stmt

    | ID LBRACKET expression RBRACKET ASSIGN expression

    {    ;

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);output_stmt

        }

    }    : OUTPUT expressiondeclaration

    ;

    ;    : type ID

/*==== RETURN STATEMENT ====*/

return_stmt    {

    : RETURN

    | RETURN expressioninput_stmt        if (global_symbol_table != NULL) {

    ;

    : INPUT ID            analyze_declaration(global_symbol_table, $2, 

/*==== INPUT/OUTPUT ====*/

input_stmt    ;                              token_to_datatype($1), 0, line_num);

    : INPUT ID

    {        }

        if (global_symbol_table != NULL) {

            check_variable_declared(global_symbol_table, $2, line_num);array_stmt    }

        }

    }    : ID ARRAY_ADD expression    | type ID ASSIGN expression

    ;

    | ID ARRAY_REMOVE    {

output_stmt

    : OUTPUT expression    | ID ARRAY_SORT        if (global_symbol_table != NULL) {

    | OUTPUT output_exprs

    ;    ;            analyze_declaration(global_symbol_table, $2,



output_exprs                              token_to_datatype($1), 0, line_num);

    : expression COMMA expression

    | output_exprs COMMA expressionfunc_call            analyze_assignment(global_symbol_table, $2,

    ;

    : ID LPAREN arg_list_opt RPAREN                             token_to_datatype($1), line_num);

/*==== ARRAY OPERATIONS ====*/

array_op    | math_call        }

    : ID ARRAY_ADD expression

    {    ;    }

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    | type ID LBRACKET INT_VAL RBRACKET

        }

    }math_call    {

    | ID ARRAY_REMOVE

    {    : MATHE_SIN LPAREN expression RPAREN        if (global_symbol_table != NULL) {

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    | MATHE_COS LPAREN expression RPAREN            analyze_declaration(global_symbol_table, $2,

        }

    }    | MATHE_TAN LPAREN expression RPAREN                              token_to_datatype($1), 1, line_num);

    | ID ARRAY_SORT

    {    | QUADRAT_WURZEL LPAREN expression RPAREN        }

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    | MATHE_LOG LPAREN expression RPAREN    }

        }

    }    ;    | type var_list

    ;

    ;

/*==== CONTROL FLOW ====*/

if_stmtarg_list_opt

    : WENN LPAREN expression RPAREN block

    ;    : /* empty */type



if_else_stmt    | arg_list    : INT_TYPE { $$ = INT_TYPE; }

    : WENN LPAREN expression RPAREN block SONST block

    | WENN LPAREN expression RPAREN block SONST_WENN LPAREN expression RPAREN block    ;    | DOUBLE_TYPE { $$ = DOUBLE_TYPE; }

    | WENN LPAREN expression RPAREN block SONST_WENN LPAREN expression RPAREN block SONST block

    ;    | STRING_TYPE { $$ = STRING_TYPE; }



while_stmtarg_list    | CHAR_TYPE { $$ = CHAR_TYPE; }

    : WAEHREND LPAREN expression RPAREN block

    ;    : expression    ;



for_stmt    | arg_list COMMA expression

    : FUR LPAREN for_init SEMICOLON expression SEMICOLON for_update RPAREN block

    ;    ;var_list



for_init    : ID COMMA ID

    : assignment

    | declarationreturn_stmt    | ID COMMA ID ASSIGN expression

    |

    ;    : RETURN    | var_list COMMA ID



for_update    | RETURN expression    | var_list COMMA ID ASSIGN expression

    : assignment

    |    ;    ;

    ;



switch_stmt

    : WAHL LPAREN expression RPAREN LBRACE cases RBRACEif_stmtassignment

    | WAHL LPAREN expression RPAREN LBRACE cases default_case RBRACE

    ;    : WENN LPAREN expression RPAREN statement    : ID ASSIGN expression



cases    | WENN LPAREN expression RPAREN statement SONST statement    {

    : case_stmt

    | cases case_stmt    ;        if (global_symbol_table != NULL) {

    ;

            check_variable_declared(global_symbol_table, $1, line_num);

case_stmt

    : FALL expression COLON stmt_listwhile_stmt        }

    ;

    : WAEHREND LPAREN expression RPAREN statement    }

default_case

    : STANDARD COLON stmt_list    ;    | ID LBRACKET expression RBRACKET ASSIGN expression

    ;

    {

block

    : LBRACE stmt_list RBRACEfor_stmt        if (global_symbol_table != NULL) {

    | LBRACE RBRACE

    ;    : FUR LPAREN for_init_opt SEMICOLON for_cond_opt SEMICOLON for_step_opt RPAREN statement            analyze_array_access(global_symbol_table, $1, line_num);



/*==== FUNCTION CALLS ====*/    ;        }

function_call

    : ID LPAREN RPAREN    }

    {

        if (global_symbol_table != NULL) {for_init_opt    ;

            analyze_function_call(global_symbol_table, $1, 0, line_num);

        }    : /* empty */

    }

    | ID LPAREN expr_list RPAREN    | declarationexpression

    {

        if (global_symbol_table != NULL) {    | assignment    : expr_term

            analyze_function_call(global_symbol_table, $1, 1, line_num);

        }    ;    | expression ARITH_OP expr_term

    }

    | math_function    | expression REL_OP expr_term

    ;

for_cond_opt    | expression AND expression

math_function

    : MATHE_SIN LPAREN expression RPAREN    : /* empty */    | expression OR expression

    | MATHE_COS LPAREN expression RPAREN

    | MATHE_TAN LPAREN expression RPAREN    | expression    | NOT expression

    | QUADRAT_WURZEL LPAREN expression RPAREN

    | MATHE_LOG LPAREN expression RPAREN    ;    | LPAREN expression RPAREN

    ;

    ;

expr_list

    : expressionfor_step_opt

    | expr_list COMMA expression

    ;    : /* empty */expr_term



/*==== EXPRESSIONS ====*/    | assignment    : ID

expression

    : expr_term    ;    {

    | expression PLUS expr_term

    | expression MINUS expr_term        if (global_symbol_table != NULL) {

    | expression STAR expr_term

    | expression SLASH expr_termswitch_stmt            check_variable_declared(global_symbol_table, $1, line_num);

    | expression MOD expr_term

    | expression EQ expr_term    : WAHL LPAREN expression RPAREN LBRACE case_list default_opt RBRACE            check_variable_initialized(global_symbol_table, $1, line_num);

    | expression NEQ expr_term

    | expression LT expr_term    ;        }

    | expression GT expr_term

    | expression LE expr_term    }

    | expression GE expr_term

    | expression AND expressioncase_list    | INT_VAL

    | expression OR expression

    | expression BIT_AND expression    : /* empty */    | DOUBLE_VAL

    | expression BIT_OR expression

    | expression BIT_XOR expression    | case_list FALL literal COLON stmt_list    | STRING_VAL

    | expression SHIFT_LEFT expression

    | expression SHIFT_RIGHT expression    ;    | CHAR_VAL

    | NOT expression

    | BIT_NOT expression    | ID LBRACKET expression RBRACKET

    | LPAREN expression RPAREN

    ;default_opt    {



expr_term    : /* empty */        if (global_symbol_table != NULL) {

    : ID

    {    | STANDARD COLON stmt_list            analyze_array_access(global_symbol_table, $1, line_num);

        if (global_symbol_table != NULL) {

            check_variable_declared(global_symbol_table, $1, line_num);    ;        }

        }

    }    }

    | ID LBRACKET expression RBRACKET

    {expression    | function_call

        if (global_symbol_table != NULL) {

            analyze_array_access(global_symbol_table, $1, line_num);    : expression PLUS expression    | LPAREN expression RPAREN

        }

    }    | expression MINUS expression    ;

    | INT_VAL

    | DOUBLE_VAL    | expression STAR expression

    | STRING_VAL

    | CHAR_VAL    | expression SLASH expressionfunction_call

    | function_call

    | LPAREN expression RPAREN    | expression MOD expression    : ID LPAREN RPAREN

    | MINUS expr_term %prec UMINUS

    ;    | expression EQ expression    {



%%    | expression NEQ expression        if (global_symbol_table != NULL) {



void yyerror(const char *s) {    | expression LT expression            analyze_function_call(global_symbol_table, $1, 0, line_num);

    fprintf(stderr, "\n");

    fprintf(stderr, "╔════════════════════════════════════════╗\n");    | expression GT expression        }

    fprintf(stderr, "║       SYNTAX ERROR DETECTED            ║\n");

    fprintf(stderr, "╚════════════════════════════════════════╝\n");    | expression LE expression    }

    fprintf(stderr, "[Line %d] %s\n", line_num, s);

    fprintf(stderr, "\n");    | expression GE expression    | ID LPAREN expr_list RPAREN

    error_count++;

}    | expression AND expression    {



int main(int argc, char *argv[]) {    | expression OR expression        if (global_symbol_table != NULL) {

    printf("\n");

    printf("╔═════════════════════════════════════════╗\n");    | expression BIT_AND expression            analyze_function_call(global_symbol_table, $1, 3, line_num);

    printf("║  SPRACH COMPILER v1.0                  ║\n");

    printf("║  German-Inspired Programming Language  ║\n");    | expression BIT_OR expression        }

    printf("║  Lexical, Syntax & Semantic Analysis   ║\n");

    printf("╚═════════════════════════════════════════╝\n\n");    | expression BIT_XOR expression    }

    

    /* Initialize symbol table */    | expression SHIFT_LEFT expression    ;

    global_symbol_table = create_symbol_table();

        | expression SHIFT_RIGHT expression

    if (global_symbol_table == NULL) {

        fprintf(stderr, "Error: Failed to initialize symbol table\n");    | NOT expressionexpr_list

        return 1;

    }    | BIT_NOT expression    : expression

    

    printf("[Phase 1] Initializing compiler...\n");    | LPAREN expression RPAREN    | expr_list COMMA expression

    printf("[Phase 2] Starting lexical and syntax analysis...\n\n");

        | ID    ;

    /* Parse the program */

    int parse_result = yyparse();    | ID LBRACKET expression RBRACKET

    

    printf("\n╔═════════════════════════════════════════╗\n");    | literalif_stmt

    printf("║           COMPILATION RESULTS          ║\n");

    printf("╚═════════════════════════════════════════╝\n\n");    | func_call    : WENN LPAREN expression RPAREN block

    

    if (parse_result == 0 && error_count == 0) {    ;    ;

        printf("✓ Lexical Analysis:    PASSED\n");

        printf("✓ Syntax Analysis:     PASSED\n");

    } else {

        printf("✗ Lexical Analysis:    FAILED\n");literalif_else_stmt

        printf("✗ Syntax Analysis:     FAILED\n");

        printf("  └─ Total Syntax Errors: %d\n", error_count);    : INT_VAL    : WENN LPAREN expression RPAREN block SONST block

    }

        | DOUBLE_VAL    ;

    /* Perform semantic analysis */

    printf("\n[Phase 3] Performing semantic analysis...\n");    | STRING_VAL

    

    SemanticResult semantic_result;    | CHAR_VALblock

    if (global_symbol_table != NULL) {

        semantic_result = get_semantic_results(global_symbol_table);    ;    : LBRACE stmt_list RBRACE

        

        printf("\n✓ Semantic Analysis:   PASSED\n");    | LBRACE RBRACE

        printf("  ├─ Variables Declared: %d\n", count_variables(global_symbol_table));

        printf("  ├─ Functions Defined:  %d\n", count_functions(global_symbol_table));type    ;

        printf("  └─ Semantic Errors:    %d\n", semantic_result.total_errors);

    }    : INT_TYPE

    

    /* Print symbol table */    | DOUBLE_TYPEstmt_list

    if (global_symbol_table != NULL) {

        printf("\n");    | STRING_TYPE    : statement

        print_symbol_table(global_symbol_table);

    }    | CHAR_TYPE    | stmt_list statement

    

    /* Final result */    ;    ;

    printf("\n╔═════════════════════════════════════════╗\n");

    printf("║              FINAL RESULT              ║\n");

    printf("╚═════════════════════════════════════════╝\n\n");

    %%for_stmt

    int total_errors = error_count + semantic_result.total_errors;

        : FUR LPAREN for_init SEMICOLON expression SEMICOLON for_update RPAREN block

    if (total_errors == 0 && semantic_result.total_warnings == 0) {

        printf("✓✓✓ COMPILATION SUCCESSFUL ✓✓✓\n");void yyerror(const char *s) {    ;

        printf("  ├─ No syntax errors\n");

        printf("  ├─ No semantic errors\n");    fprintf(stderr, "Syntax error at line %d: %s\n", line_num, s);

        printf("  └─ No warnings\n");

    } else if (total_errors == 0) {}for_init

        printf("✓ COMPILATION SUCCESSFUL (with warnings)\n");

        printf("  ├─ No syntax errors\n");    : assignment

        printf("  ├─ No semantic errors\n");    | declaration

        printf("  └─ Warnings: %d\n", semantic_result.total_warnings);    ;

    } else {

        printf("✗ COMPILATION FAILED\n");for_update

        printf("  ├─ Syntax Errors:  %d\n", error_count);    : assignment

        printf("  ├─ Semantic Errors: %d\n", semantic_result.total_errors);    ;

        printf("  └─ Warnings: %d\n", semantic_result.total_warnings);

    }while_stmt

        : WAEHREND LPAREN expression RPAREN block

    printf("\n╚═════════════════════════════════════════╝\n\n");    ;

    

    /* Cleanup */switch_stmt

    if (global_symbol_table != NULL) {    : WAHL LPAREN expression RPAREN LBRACE cases RBRACE

        free_symbol_table(global_symbol_table);    | WAHL LPAREN expression RPAREN LBRACE cases default_case RBRACE

    }    ;

    

    return (total_errors == 0) ? 0 : 1;cases

}    : case_stmt

    | cases case_stmt
    ;

case_stmt
    : FALL expression COLON stmt_list
    ;

default_case
    : STANDARD COLON stmt_list
    ;

input_stmt
    : INPUT ID
    {
        if (global_symbol_table != NULL) {
            check_variable_declared(global_symbol_table, $2, line_num);
        }
    }
    | INPUT input_ids
    ;

input_ids
    : ID COMMA ID
    | input_ids COMMA ID
    ;

output_stmt
    : OUTPUT expression
    | OUTPUT output_exprs
    ;

output_exprs
    : expression COMMA expression
    | output_exprs COMMA expression
    ;

array_op
    : ID ARRAY_ADD expression
    {
        if (global_symbol_table != NULL) {
            analyze_array_access(global_symbol_table, $1, line_num);
        }
    }
    | ID ARRAY_REMOVE
    {
        if (global_symbol_table != NULL) {
            analyze_array_access(global_symbol_table, $1, line_num);
        }
    }
    | ID ARRAY_SORT
    {
        if (global_symbol_table != NULL) {
            analyze_array_access(global_symbol_table, $1, line_num);
        }
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "SYNTAX ERROR [Line %d]: %s\n", line_num, s);
    error_count++;
}

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf("   SPRACH COMPILER - PHASE 3\n");
    printf("   Semantic Analysis & Type Checking\n");
    printf("===========================================\n\n");
    
    /* Initialize symbol table */
    global_symbol_table = create_symbol_table();
    
    /* Parse the program */
    int parse_result = yyparse();
    
    printf("\n========== PARSING PHASE RESULTS ==========\n");
    
    if (parse_result == 0 && error_count == 0) {
        printf("✓ Syntax Analysis: PASSED\n");
    } else {
        printf("✗ Syntax Analysis: FAILED\n");
        printf("  Syntax Errors: %d\n", error_count);
    }
    
    /* Print symbol table */
    if (global_symbol_table != NULL) {
        print_symbol_table(global_symbol_table);
    }
    
    /* Get and print semantic results */
    SemanticResult result = get_semantic_results(global_symbol_table);
    print_semantic_results(result);
    
    /* Final result */
    printf("========== FINAL RESULT ==========\n");
    int total_errors = error_count + result.total_errors;
    
    if (total_errors == 0 && result.total_warnings == 0) {
        printf("✓ COMPILATION SUCCESSFUL\n");
        printf("  - No syntax errors\n");
        printf("  - No semantic errors\n");
        printf("  - No warnings\n");
    } else if (total_errors == 0) {
        printf("✓ COMPILATION SUCCESSFUL (with warnings)\n");
        printf("  - No syntax errors\n");
        printf("  - No semantic errors\n");
        printf("  - Warnings: %d\n", result.total_warnings);
    } else {
        printf("✗ COMPILATION FAILED\n");
        printf("  - Syntax Errors: %d\n", error_count);
        printf("  - Semantic Errors: %d\n", result.total_errors);
        printf("  - Warnings: %d\n", result.total_warnings);
    }
    
    printf("====================================\n\n");
    
    /* Cleanup */
    free_symbol_table(global_symbol_table);
    
    return (total_errors == 0) ? 0 : 1;
}

