#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

/* ============= LEXER ============= */

typedef enum {
    TOK_EOF, TOK_INT, TOK_DOUBLE, TOK_ID, TOK_STRING,
    TOK_GANZ, TOK_KOMM, TOK_TEXT, TOK_ZEICHEN,
    TOK_WENN, TOK_SONST, TOK_FUR, TOK_WAEHREND, TOK_WAHL, TOK_FALL,
    TOK_GEBEN, TOK_LESEN,
    TOK_UND, TOK_ODER, TOK_NICHT,
    TOK_HINZUFUEGEN, TOK_ENTFERNEN, TOK_SORTIEREN,
    TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_MOD, TOK_POW,
    TOK_BITAND, TOK_BITOR, TOK_BITXOR, TOK_LSHIFT, TOK_RSHIFT,
    TOK_EQ, TOK_NE, TOK_LT, TOK_LE, TOK_GT, TOK_GE,
    TOK_ASSIGN, TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_LBRACKET, TOK_RBRACKET, TOK_SEMICOLON, TOK_COMMA, TOK_COLON
} TokenType;

typedef struct {
    TokenType type;
    union {
        int intval;
        double doubleval;
        char strval[256];
    } value;
} Token;

typedef struct {
    char *input;
    int pos;
    int current_char;
} Lexer;

Lexer *lexer_create(const char *input) {
    Lexer *lex = malloc(sizeof(Lexer));
    lex->input = strdup(input);
    lex->pos = 0;
    lex->current_char = lex->input[0];
    return lex;
}

void lexer_advance(Lexer *lex) {
    lex->pos++;
    if (lex->pos >= strlen(lex->input)) {
        lex->current_char = '\0';
    } else {
        lex->current_char = lex->input[lex->pos];
    }
}

void lexer_skip_whitespace(Lexer *lex) {
    while (lex->current_char != '\0' && isspace(lex->current_char)) {
        lexer_advance(lex);
    }
}

void lexer_skip_comment(Lexer *lex) {
    if (lex->current_char == '#') {
        // Check for multi-line comment ##
        if (lex->input[lex->pos + 1] == '#') {
            lexer_advance(lex); // skip first #
            lexer_advance(lex); // skip second #
            
            // Skip until we find ## again
            while (lex->current_char != '\0') {
                if (lex->current_char == '#' && lex->input[lex->pos + 1] == '#') {
                    lexer_advance(lex); // skip first #
                    lexer_advance(lex); // skip second #
                    break;
                }
                lexer_advance(lex);
            }
        } else {
            // Single-line comment
            while (lex->current_char != '\0' && lex->current_char != '\n') {
                lexer_advance(lex);
            }
        }
    }
}

Token *lexer_read_string(Lexer *lex) {
    Token *tok = malloc(sizeof(Token));
    char str[256] = "";
    lexer_advance(lex); // skip opening quote
    
    while (lex->current_char != '\0' && lex->current_char != '"') {
        sprintf(str + strlen(str), "%c", lex->current_char);
        lexer_advance(lex);
    }
    
    if (lex->current_char == '"') {
        lexer_advance(lex); // skip closing quote
    }
    
    tok->type = TOK_STRING;
    strcpy(tok->value.strval, str);
    return tok;
}

Token *lexer_read_char(Lexer *lex) {
    Token *tok = malloc(sizeof(Token));
    lexer_advance(lex); // skip opening quote
    char ch = lex->current_char;
    lexer_advance(lex);
    if (lex->current_char == '\'') {
        lexer_advance(lex); // skip closing quote
    }
    
    tok->type = TOK_STRING;
    sprintf(tok->value.strval, "%c", ch);
    return tok;
}

Token *lexer_read_number(Lexer *lex) {
    Token *tok = malloc(sizeof(Token));
    char num_str[256] = "";
    
    while (lex->current_char != '\0' && (isdigit(lex->current_char) || lex->current_char == '.')) {
        sprintf(num_str + strlen(num_str), "%c", lex->current_char);
        lexer_advance(lex);
    }
    
    if (strchr(num_str, '.')) {
        tok->type = TOK_DOUBLE;
        tok->value.doubleval = strtod(num_str, NULL);
    } else {
        tok->type = TOK_INT;
        tok->value.intval = atoi(num_str);
    }
    return tok;
}

Token *lexer_read_id(Lexer *lex) {
    Token *tok = malloc(sizeof(Token));
    char id_str[256] = "";
    
    while (lex->current_char != '\0' && (isalnum(lex->current_char) || lex->current_char == '_')) {
        sprintf(id_str + strlen(id_str), "%c", lex->current_char);
        lexer_advance(lex);
    }
    
    if (strcmp(id_str, "GANZ") == 0) tok->type = TOK_GANZ;
    else if (strcmp(id_str, "KOMM") == 0) tok->type = TOK_KOMM;
    else if (strcmp(id_str, "TEXT") == 0) tok->type = TOK_TEXT;
    else if (strcmp(id_str, "ZEICHEN") == 0) tok->type = TOK_ZEICHEN;
    else if (strcmp(id_str, "wenn") == 0) tok->type = TOK_WENN;
    else if (strcmp(id_str, "sonst") == 0) tok->type = TOK_SONST;
    else if (strcmp(id_str, "fur") == 0 || strcmp(id_str, "fuer") == 0) tok->type = TOK_FUR;
    else if (strcmp(id_str, "waehrend") == 0) tok->type = TOK_WAEHREND;
    else if (strcmp(id_str, "wahl") == 0) tok->type = TOK_WAHL;
    else if (strcmp(id_str, "fall") == 0) tok->type = TOK_FALL;
    else if (strcmp(id_str, "geben") == 0) tok->type = TOK_GEBEN;
    else if (strcmp(id_str, "lesen") == 0) tok->type = TOK_LESEN;
    else if (strcmp(id_str, "und") == 0) tok->type = TOK_UND;
    else if (strcmp(id_str, "oder") == 0) tok->type = TOK_ODER;
    else if (strcmp(id_str, "nicht") == 0) tok->type = TOK_NICHT;
    else if (strcmp(id_str, "hinzufuegen") == 0) tok->type = TOK_HINZUFUEGEN;
    else if (strcmp(id_str, "entfernen") == 0) tok->type = TOK_ENTFERNEN;
    else if (strcmp(id_str, "sortieren") == 0) tok->type = TOK_SORTIEREN;
    else {
        tok->type = TOK_ID;
        strcpy(tok->value.strval, id_str);
    }
    return tok;
}

Token *lexer_get_next_token(Lexer *lex) {
    while (lex->current_char != '\0') {
        if (isspace(lex->current_char)) {
            lexer_skip_whitespace(lex);
            continue;
        }
        if (lex->current_char == '#') {
            lexer_skip_comment(lex);
            continue;
        }
        break;
    }
    
    if (lex->current_char == '\0') {
        Token *tok = malloc(sizeof(Token));
        tok->type = TOK_EOF;
        return tok;
    }
    
    if (lex->current_char == '"') {
        return lexer_read_string(lex);
    }
    
    if (lex->current_char == '\'') {
        return lexer_read_char(lex);
    }
    
    if (isdigit(lex->current_char)) {
        return lexer_read_number(lex);
    }
    
    if (isalpha(lex->current_char) || lex->current_char == '_') {
        return lexer_read_id(lex);
    }
    
    Token *tok = malloc(sizeof(Token));
    
    if (lex->current_char == '+') { tok->type = TOK_PLUS; lexer_advance(lex); return tok; }
    if (lex->current_char == '-') { tok->type = TOK_MINUS; lexer_advance(lex); return tok; }
    if (lex->current_char == '*') { tok->type = TOK_MUL; lexer_advance(lex); return tok; }
    if (lex->current_char == '/') { tok->type = TOK_DIV; lexer_advance(lex); return tok; }
    if (lex->current_char == '%') { tok->type = TOK_MOD; lexer_advance(lex); return tok; }
    if (lex->current_char == '^') { tok->type = TOK_POW; lexer_advance(lex); return tok; }
    if (lex->current_char == '&') { tok->type = TOK_BITAND; lexer_advance(lex); return tok; }
    if (lex->current_char == '|') { tok->type = TOK_BITOR; lexer_advance(lex); return tok; }
    if (lex->current_char == '(') { tok->type = TOK_LPAREN; lexer_advance(lex); return tok; }
    if (lex->current_char == ')') { tok->type = TOK_RPAREN; lexer_advance(lex); return tok; }
    if (lex->current_char == '{') { tok->type = TOK_LBRACE; lexer_advance(lex); return tok; }
    if (lex->current_char == '}') { tok->type = TOK_RBRACE; lexer_advance(lex); return tok; }
    if (lex->current_char == '[') { tok->type = TOK_LBRACKET; lexer_advance(lex); return tok; }
    if (lex->current_char == ']') { tok->type = TOK_RBRACKET; lexer_advance(lex); return tok; }
    if (lex->current_char == ';') { tok->type = TOK_SEMICOLON; lexer_advance(lex); return tok; }
    if (lex->current_char == ',') { tok->type = TOK_COMMA; lexer_advance(lex); return tok; }
    if (lex->current_char == ':') { tok->type = TOK_COLON; lexer_advance(lex); return tok; }
    
    if (lex->current_char == '=') {
        lexer_advance(lex);
        if (lex->current_char == '=') {
            tok->type = TOK_EQ;
            lexer_advance(lex);
        } else {
            tok->type = TOK_ASSIGN;
        }
        return tok;
    }
    
    if (lex->current_char == '!') {
        lexer_advance(lex);
        if (lex->current_char == '=') {
            tok->type = TOK_NE;
            lexer_advance(lex);
        } else {
            tok->type = TOK_NICHT;
        }
        return tok;
    }
    
    if (lex->current_char == '<') {
        lexer_advance(lex);
        if (lex->current_char == '<') {
            tok->type = TOK_LSHIFT;
            lexer_advance(lex);
        } else if (lex->current_char == '=') {
            tok->type = TOK_LE;
            lexer_advance(lex);
        } else {
            tok->type = TOK_LT;
        }
        return tok;
    }
    
    if (lex->current_char == '>') {
        lexer_advance(lex);
        if (lex->current_char == '>') {
            tok->type = TOK_RSHIFT;
            lexer_advance(lex);
        } else if (lex->current_char == '=') {
            tok->type = TOK_GE;
            lexer_advance(lex);
        } else {
            tok->type = TOK_GT;
        }
        return tok;
    }
    
    lexer_advance(lex);
    tok->type = TOK_EOF;
    return tok;
}

/* ============= AST ============= */

typedef enum {
    NODE_PROGRAM, NODE_BLOCK,
    NODE_DECL, NODE_ASSIGN, NODE_OUTPUT, NODE_INPUT,
    NODE_WHILE, NODE_IF, NODE_FOR, NODE_WAHL,
    NODE_BINOP, NODE_UNOP, NODE_NUMBER, NODE_STRING, NODE_VAR, NODE_CALL,
    NODE_ARRAY_ACCESS, NODE_ARRAY_INIT, NODE_ARRAY_METHOD
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode **statements;
            int count;
        } block;
        struct {
            char varname[256];
            int type;
            struct ASTNode *value;
        } decl;
        struct {
            char varname[256];
            struct ASTNode *value;
        } assign;
        struct {
            struct ASTNode *expr;
        } output;
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
            struct ASTNode *else_body;
        } if_node;
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_node;
        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *increment;
            struct ASTNode *body;
        } for_node;
        struct {
            struct ASTNode *expr;
            struct {
                double value;
                struct ASTNode *body;
            } cases[100];
            int case_count;
            struct ASTNode *default_body;
        } wahl_node;
        struct {
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binop;
        struct {
            char op;
            struct ASTNode *operand;
        } unop;
        struct {
            double value;
        } number;
        struct {
            char value[256];
        } string;
        struct {
            char name[256];
        } var;
        struct {
            char name[256];
            struct ASTNode *index;
        } array_access;
        struct {
            struct ASTNode **elements;
            int count;
        } array_init;
        struct {
            char name[256];
            struct ASTNode **args;
            int arg_count;
        } call;
        struct {
            char array_name[256];
            int method;  // 0=hinzufuegen, 1=entfernen, 2=sortieren
            struct ASTNode *arg;  // For hinzufuegen, the value to add
        } array_method;
    } data;
} ASTNode;

ASTNode *ast_create_number(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->data.number.value = value;
    return node;
}

ASTNode *ast_create_string(const char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STRING;
    strcpy(node->data.string.value, value);
    return node;
}

ASTNode *ast_create_var(const char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    strcpy(node->data.var.name, name);
    return node;
}

ASTNode *ast_create_binop(char op, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->data.binop.op = op;
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

ASTNode *ast_create_while(ASTNode *condition, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->data.while_node.condition = condition;
    node->data.while_node.body = body;
    return node;
}

ASTNode *ast_create_output(ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_OUTPUT;
    node->data.output.expr = expr;
    return node;
}

/* ============= PARSER ============= */

typedef struct {
    Lexer *lexer;
    Token *current_token;
} Parser;

Parser *parser_create(const char *input) {
    Parser *parser = malloc(sizeof(Parser));
    parser->lexer = lexer_create(input);
    parser->current_token = lexer_get_next_token(parser->lexer);
    return parser;
}

void parser_advance(Parser *parser) {
    free(parser->current_token);
    parser->current_token = lexer_get_next_token(parser->lexer);
}

ASTNode *parser_parse_expression(Parser *parser);
ASTNode *parser_parse_logical_or(Parser *parser);
ASTNode *parser_parse_logical_and(Parser *parser);
ASTNode *parser_parse_logical_not(Parser *parser);
ASTNode *parser_parse_statement(Parser *parser);
ASTNode *parser_parse_block(Parser *parser);
ASTNode *parser_parse_power(Parser *parser);
ASTNode *parser_parse_bitwise(Parser *parser);

ASTNode *parser_parse_primary(Parser *parser) {
    Token *tok = parser->current_token;
    
    if (tok->type == TOK_INT) {
        parser_advance(parser);
        return ast_create_number(tok->value.intval);
    }
    if (tok->type == TOK_DOUBLE) {
        parser_advance(parser);
        return ast_create_number(tok->value.doubleval);
    }
    if (tok->type == TOK_STRING) {
        char str[256];
        strcpy(str, tok->value.strval);
        parser_advance(parser);
        return ast_create_string(str);
    }
    if (tok->type == TOK_ID) {
        char name[256];
        strcpy(name, tok->value.strval);
        parser_advance(parser);
        
        // Check for function call
        if (parser->current_token->type == TOK_LPAREN) {
            parser_advance(parser); // skip (
            
            ASTNode *call_node = malloc(sizeof(ASTNode));
            call_node->type = NODE_CALL;
            strcpy(call_node->data.call.name, name);
            call_node->data.call.args = malloc(sizeof(ASTNode*) * 10);
            call_node->data.call.arg_count = 0;
            
            while (parser->current_token->type != TOK_RPAREN && 
                   parser->current_token->type != TOK_EOF) {
                call_node->data.call.args[call_node->data.call.arg_count++] = 
                    parser_parse_expression(parser);
                
                if (parser->current_token->type == TOK_COMMA) {
                    parser_advance(parser);
                }
            }
            
            parser_advance(parser); // skip )
            return call_node;
        }
        
        // Check for array indexing
        if (parser->current_token->type == TOK_LBRACKET) {
            parser_advance(parser);
            ASTNode *index = parser_parse_expression(parser);
            parser_advance(parser); // skip ]
            
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = NODE_ARRAY_ACCESS;
            strcpy(node->data.array_access.name, name);
            node->data.array_access.index = index;
            return node;
        }
        
        return ast_create_var(name);
    }
    if (tok->type == TOK_LPAREN) {
        parser_advance(parser);
        ASTNode *expr = parser_parse_expression(parser);
        parser_advance(parser); // skip )
        return expr;
    }
    if (tok->type == TOK_MINUS) {
        parser_advance(parser);
        ASTNode *expr = parser_parse_primary(parser);
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = NODE_UNOP;
        node->data.unop.op = '-';
        node->data.unop.operand = expr;
        return node;
    }
    
    return ast_create_number(0);
}

ASTNode *parser_parse_term(Parser *parser) {
    ASTNode *left = parser_parse_power(parser);
    
    while (parser->current_token->type == TOK_MUL ||
           parser->current_token->type == TOK_DIV ||
           parser->current_token->type == TOK_MOD) {
        char op = parser->current_token->type == TOK_MUL ? '*' :
                 parser->current_token->type == TOK_DIV ? '/' : '%';
        parser_advance(parser);
        ASTNode *right = parser_parse_power(parser);
        left = ast_create_binop(op, left, right);
    }
    
    return left;
}

ASTNode *parser_parse_power(Parser *parser) {
    ASTNode *left = parser_parse_primary(parser);
    
    if (parser->current_token->type == TOK_POW) {
        parser_advance(parser);
        ASTNode *right = parser_parse_power(parser); // right associative
        left = ast_create_binop('^', left, right);
    }
    
    return left;
}

ASTNode *parser_parse_arithmetic(Parser *parser) {
    ASTNode *left = parser_parse_term(parser);
    
    while (parser->current_token->type == TOK_PLUS ||
           parser->current_token->type == TOK_MINUS) {
        char op = parser->current_token->type == TOK_PLUS ? '+' : '-';
        parser_advance(parser);
        ASTNode *right = parser_parse_term(parser);
        left = ast_create_binop(op, left, right);
    }
    
    return left;
}

ASTNode *parser_parse_comparison(Parser *parser) {
    ASTNode *left = parser_parse_bitwise(parser);
    
    while (parser->current_token->type == TOK_LT ||
           parser->current_token->type == TOK_LE ||
           parser->current_token->type == TOK_GT ||
           parser->current_token->type == TOK_GE ||
           parser->current_token->type == TOK_EQ ||
           parser->current_token->type == TOK_NE) {
        char op;
        if (parser->current_token->type == TOK_LT) op = '<';
        else if (parser->current_token->type == TOK_LE) op = 'L';
        else if (parser->current_token->type == TOK_GT) op = '>';
        else if (parser->current_token->type == TOK_GE) op = 'G';
        else if (parser->current_token->type == TOK_EQ) op = '=';
        else op = '!';
        
        parser_advance(parser);
        ASTNode *right = parser_parse_bitwise(parser);
        left = ast_create_binop(op, left, right);
    }
    
    return left;
}

ASTNode *parser_parse_bitwise(Parser *parser) {
    ASTNode *left = parser_parse_arithmetic(parser);
    
    while (parser->current_token->type == TOK_BITAND ||
           parser->current_token->type == TOK_BITOR ||
           parser->current_token->type == TOK_LSHIFT ||
           parser->current_token->type == TOK_RSHIFT) {
        char op;
        if (parser->current_token->type == TOK_BITAND) op = '&';
        else if (parser->current_token->type == TOK_BITOR) op = '|';
        else if (parser->current_token->type == TOK_LSHIFT) op = 'l';
        else op = 'r';
        
        parser_advance(parser);
        ASTNode *right = parser_parse_arithmetic(parser);
        left = ast_create_binop(op, left, right);
    }
    
    return left;
}

ASTNode *parser_parse_expression(Parser *parser) {
    return parser_parse_logical_or(parser);
}

ASTNode *parser_parse_logical_or(Parser *parser) {
    ASTNode *left = parser_parse_logical_and(parser);
    
    while (parser->current_token->type == TOK_ODER) {
        parser_advance(parser);
        ASTNode *right = parser_parse_logical_and(parser);
        left = ast_create_binop('O', left, right);
    }
    
    return left;
}

ASTNode *parser_parse_logical_and(Parser *parser) {
    ASTNode *left = parser_parse_logical_not(parser);
    
    while (parser->current_token->type == TOK_UND) {
        parser_advance(parser);
        ASTNode *right = parser_parse_logical_not(parser);
        left = ast_create_binop('A', left, right);
    }
    
    return left;
}

ASTNode *parser_parse_logical_not(Parser *parser) {
    if (parser->current_token->type == TOK_NICHT) {
        parser_advance(parser);
        ASTNode *expr = parser_parse_logical_not(parser);
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = NODE_UNOP;
        node->data.unop.op = 'N';
        node->data.unop.operand = expr;
        return node;
    }
    
    return parser_parse_comparison(parser);
}

ASTNode *parser_parse_block(Parser *parser) {
    if (parser->current_token->type != TOK_LBRACE) {
        return parser_parse_statement(parser);
    }
    
    parser_advance(parser); // skip {
    
    ASTNode *block = malloc(sizeof(ASTNode));
    block->type = NODE_BLOCK;
    block->data.block.statements = malloc(sizeof(ASTNode*) * 1000);
    block->data.block.count = 0;
    
    while (parser->current_token->type != TOK_RBRACE && parser->current_token->type != TOK_EOF) {
        block->data.block.statements[block->data.block.count++] = parser_parse_statement(parser);
    }
    
    parser_advance(parser); // skip }
    return block;
}

ASTNode *parser_parse_statement(Parser *parser) {
    if (parser->current_token->type == TOK_GANZ ||
        parser->current_token->type == TOK_KOMM ||
        parser->current_token->type == TOK_TEXT ||
        parser->current_token->type == TOK_ZEICHEN) {
        
        int type = parser->current_token->type;
        parser_advance(parser);
        
        char varname[256];
        strcpy(varname, parser->current_token->value.strval);
        parser_advance(parser);
        
        ASTNode *decl = malloc(sizeof(ASTNode));
        decl->type = NODE_DECL;
        strcpy(decl->data.decl.varname, varname);
        decl->data.decl.type = type;
        
        // Check for array declaration
        if (parser->current_token->type == TOK_LBRACKET) {
            parser_advance(parser); // skip [
            parser_advance(parser); // skip ]
            
            // Check for initialization
            if (parser->current_token->type == TOK_ASSIGN) {
                parser_advance(parser); // skip =
                
                if (parser->current_token->type == TOK_LBRACE) {
                    parser_advance(parser); // skip {
                    
                    ASTNode *arr_init = malloc(sizeof(ASTNode));
                    arr_init->type = NODE_ARRAY_INIT;
                    arr_init->data.array_init.elements = malloc(sizeof(ASTNode*) * 1000);
                    arr_init->data.array_init.count = 0;
                    
                    while (parser->current_token->type != TOK_RBRACE && 
                           parser->current_token->type != TOK_EOF) {
                        arr_init->data.array_init.elements[arr_init->data.array_init.count++] = 
                            parser_parse_expression(parser);
                        
                        if (parser->current_token->type == TOK_COMMA) {
                            parser_advance(parser);
                        }
                    }
                    
                    parser_advance(parser); // skip }
                    decl->data.decl.value = arr_init;
                } else {
                    decl->data.decl.value = ast_create_number(0);
                }
            } else {
                decl->data.decl.value = ast_create_number(0);
            }
        } else if (parser->current_token->type == TOK_ASSIGN) {
            parser_advance(parser);
            decl->data.decl.value = parser_parse_expression(parser);
        } else {
            decl->data.decl.value = ast_create_number(0);
        }
        
        if (parser->current_token->type == TOK_SEMICOLON) {
            parser_advance(parser);
        }
        
        return decl;
    }
    
    if (parser->current_token->type == TOK_ID) {
        char varname[256];
        strcpy(varname, parser->current_token->value.strval);
        parser_advance(parser);
        
        // Check for array method calls
        if (parser->current_token->type == TOK_HINZUFUEGEN ||
            parser->current_token->type == TOK_ENTFERNEN ||
            parser->current_token->type == TOK_SORTIEREN) {
            
            int method;
            if (parser->current_token->type == TOK_HINZUFUEGEN) method = 0;
            else if (parser->current_token->type == TOK_ENTFERNEN) method = 1;
            else method = 2;
            
            parser_advance(parser);
            
            ASTNode *arr_method = malloc(sizeof(ASTNode));
            arr_method->type = NODE_ARRAY_METHOD;
            strcpy(arr_method->data.array_method.array_name, varname);
            arr_method->data.array_method.method = method;
            arr_method->data.array_method.arg = NULL;
            
            // For hinzufuegen, parse the argument
            if (method == 0) {
                arr_method->data.array_method.arg = parser_parse_expression(parser);
            }
            
            if (parser->current_token->type == TOK_SEMICOLON) {
                parser_advance(parser);
            }
            
            return arr_method;
        }
        
        if (parser->current_token->type == TOK_ASSIGN) {
            parser_advance(parser);
            ASTNode *assign = malloc(sizeof(ASTNode));
            assign->type = NODE_ASSIGN;
            strcpy(assign->data.assign.varname, varname);
            assign->data.assign.value = parser_parse_expression(parser);
            
            if (parser->current_token->type == TOK_SEMICOLON) {
                parser_advance(parser);
            }
            
            return assign;
        }
    }
    
    if (parser->current_token->type == TOK_GEBEN) {
        parser_advance(parser);
        ASTNode *output = ast_create_output(parser_parse_expression(parser));
        if (parser->current_token->type == TOK_SEMICOLON) {
            parser_advance(parser);
        }
        return output;
    }
    
    if (parser->current_token->type == TOK_WAEHREND) {
        parser_advance(parser);
        parser_advance(parser); // skip (
        ASTNode *condition = parser_parse_expression(parser);
        parser_advance(parser); // skip )
        
        ASTNode *body = parser_parse_block(parser);
        
        return ast_create_while(condition, body);
    }
    
    if (parser->current_token->type == TOK_FUR) {
        parser_advance(parser);
        parser_advance(parser); // skip (
        
        // Parse init (just expression, not full statement)
        ASTNode *init = NULL;
        if (parser->current_token->type == TOK_ID) {
            char varname[256];
            strcpy(varname, parser->current_token->value.strval);
            parser_advance(parser);
            
            if (parser->current_token->type == TOK_ASSIGN) {
                parser_advance(parser);
                ASTNode *assign = malloc(sizeof(ASTNode));
                assign->type = NODE_ASSIGN;
                strcpy(assign->data.assign.varname, varname);
                assign->data.assign.value = parser_parse_expression(parser);
                init = assign;
            }
        }
        
        parser_advance(parser); // skip ;
        
        // Parse condition
        ASTNode *condition = parser_parse_expression(parser);
        parser_advance(parser); // skip ;
        
        // Parse increment (just expression)
        ASTNode *increment = NULL;
        if (parser->current_token->type == TOK_ID) {
            char varname[256];
            strcpy(varname, parser->current_token->value.strval);
            parser_advance(parser);
            
            if (parser->current_token->type == TOK_ASSIGN) {
                parser_advance(parser);
                ASTNode *assign = malloc(sizeof(ASTNode));
                assign->type = NODE_ASSIGN;
                strcpy(assign->data.assign.varname, varname);
                assign->data.assign.value = parser_parse_expression(parser);
                increment = assign;
            }
        }
        
        parser_advance(parser); // skip )
        
        // Parse body
        ASTNode *body = parser_parse_block(parser);
        
        // Create FOR node
        ASTNode *for_node = malloc(sizeof(ASTNode));
        for_node->type = NODE_FOR;
        for_node->data.for_node.init = init;
        for_node->data.for_node.condition = condition;
        for_node->data.for_node.increment = increment;
        for_node->data.for_node.body = body;
        
        return for_node;
    }
    
    if (parser->current_token->type == TOK_WENN) {
        parser_advance(parser);
        parser_advance(parser); // skip (
        ASTNode *condition = parser_parse_expression(parser);
        parser_advance(parser); // skip )
        
        ASTNode *if_node = malloc(sizeof(ASTNode));
        if_node->type = NODE_IF;
        if_node->data.if_node.condition = condition;
        if_node->data.if_node.body = parser_parse_block(parser);
        if_node->data.if_node.else_body = NULL;
        
        if (parser->current_token->type == TOK_SONST) {
            parser_advance(parser);
            if_node->data.if_node.else_body = parser_parse_block(parser);
        }
        
        return if_node;
    }
    
    if (parser->current_token->type == TOK_WAHL) {
        parser_advance(parser);
        parser_advance(parser); // skip (
        ASTNode *expr = parser_parse_expression(parser);
        parser_advance(parser); // skip )
        
        parser_advance(parser); // skip {
        
        ASTNode *wahl_node = malloc(sizeof(ASTNode));
        wahl_node->type = NODE_WAHL;
        wahl_node->data.wahl_node.expr = expr;
        wahl_node->data.wahl_node.case_count = 0;
        wahl_node->data.wahl_node.default_body = NULL;
        
        while (parser->current_token->type != TOK_RBRACE && 
               parser->current_token->type != TOK_EOF) {
            
            if (parser->current_token->type == TOK_FALL) {
                parser_advance(parser); // skip 'fall'
                
                // Parse case value
                double case_val = parser->current_token->value.intval;
                parser_advance(parser); // skip number
                parser_advance(parser); // skip :
                
                // Parse case body (single statement)
                ASTNode *case_body = parser_parse_statement(parser);
                
                wahl_node->data.wahl_node.cases[wahl_node->data.wahl_node.case_count].value = case_val;
                wahl_node->data.wahl_node.cases[wahl_node->data.wahl_node.case_count].body = case_body;
                wahl_node->data.wahl_node.case_count++;
            } else if (parser->current_token->type == TOK_ID &&
                       strcmp(parser->current_token->value.strval, "standard") == 0) {
                parser_advance(parser); // skip 'standard'
                parser_advance(parser); // skip :
                wahl_node->data.wahl_node.default_body = parser_parse_statement(parser);
            } else {
                parser_advance(parser);
            }
        }
        
        parser_advance(parser); // skip }
        
        return wahl_node;
    }
    
    parser_advance(parser);
    return NULL;
}

/* ============= ERROR HANDLING ============= */

typedef struct {
    int error_count;
    char error_messages[100][512];
    int runtime_error;
    char last_error[512];
} ErrorHandler;

ErrorHandler *error_handler = NULL;

ErrorHandler *error_handler_create() {
    ErrorHandler *eh = malloc(sizeof(ErrorHandler));
    eh->error_count = 0;
    eh->runtime_error = 0;
    memset(eh->last_error, 0, 512);
    return eh;
}

void error_report(const char *format, ...) {
    if (!error_handler) return;
    
    va_list args;
    va_start(args, format);
    vsnprintf(error_handler->last_error, 512, format, args);
    va_end(args);
    
    if (error_handler->error_count < 100) {
        strcpy(error_handler->error_messages[error_handler->error_count], error_handler->last_error);
        error_handler->error_count++;
    }
    
    fprintf(stderr, "[ERROR] %s\n", error_handler->last_error);
    error_handler->runtime_error = 1;
}

int has_error() {
    return error_handler && error_handler->runtime_error;
}

void clear_error() {
    if (error_handler) {
        error_handler->runtime_error = 0;
        memset(error_handler->last_error, 0, 512);
    }
}

/* ============= INTERPRETER ============= */

#define MAX_VARS 1000
#define MAX_ARRAY_SIZE 1000

typedef struct {
    char name[256];
    double value;
    char string_value[256];
    int is_string;
    double *array;
    char **string_array;  // For TEXT arrays
    int array_size;
    int is_array;
    int is_string_array;  // 1 if array contains strings, 0 if numbers
} Variable;

typedef struct {
    Variable vars[MAX_VARS];
    int var_count;
} Environment;

Environment *env_create() {
    Environment *env = malloc(sizeof(Environment));
    env->var_count = 0;
    return env;
}

double env_get(Environment *env, const char *name) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            return env->vars[i].value;
        }
    }
    return 0;
}

void env_set(Environment *env, const char *name, double value) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            env->vars[i].value = value;
            env->vars[i].is_string = 0;
            env->vars[i].is_array = 0;
            env->vars[i].array = NULL;
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        env->vars[env->var_count].value = value;
        env->vars[env->var_count].is_string = 0;
        env->vars[env->var_count].is_array = 0;
        env->vars[env->var_count].array = NULL;
        env->var_count++;
    }
}

void env_set_string(Environment *env, const char *name, const char *value) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            strcpy(env->vars[i].string_value, value);
            env->vars[i].is_string = 1;
            env->vars[i].is_array = 0;
            env->vars[i].array = NULL;
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        strcpy(env->vars[env->var_count].string_value, value);
        env->vars[env->var_count].is_string = 1;
        env->vars[env->var_count].is_array = 0;
        env->vars[env->var_count].array = NULL;
        env->var_count++;
    }
}

void env_set_array(Environment *env, const char *name, double *array, int size) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            env->vars[i].array = array;
            env->vars[i].array_size = size;
            env->vars[i].is_array = 1;
            env->vars[i].is_string = 0;
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        env->vars[env->var_count].array = array;
        env->vars[env->var_count].array_size = size;
        env->vars[env->var_count].is_array = 1;
        env->vars[env->var_count].is_string = 0;
        env->var_count++;
    }
}

double env_get_array_element(Environment *env, const char *name, int index) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0 && env->vars[i].is_array) {
            if (index >= 0 && index < env->vars[i].array_size) {
                return env->vars[i].array[index];
            }
        }
    }
    return 0;
}

char *env_get_string_array_element(Environment *env, const char *name, int index) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0 && env->vars[i].is_string_array) {
            if (index >= 0 && index < env->vars[i].array_size) {
                return env->vars[i].string_array[index];
            }
        }
    }
    return "";
}

void env_set_string_array(Environment *env, const char *name, char **array, int size) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, name) == 0) {
            env->vars[i].string_array = array;
            env->vars[i].array_size = size;
            env->vars[i].is_array = 1;
            env->vars[i].is_string_array = 1;
            env->vars[i].is_string = 0;
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        env->vars[env->var_count].string_array = array;
        env->vars[env->var_count].array_size = size;
        env->vars[env->var_count].is_array = 1;
        env->vars[env->var_count].is_string_array = 1;
        env->vars[env->var_count].is_string = 0;
        env->var_count++;
    }
}

/* ============= TYPE CHECKING HELPERS ============= */

int is_string_variable(Environment *env, const char *varname) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, varname) == 0) {
            return env->vars[i].is_string;
        }
    }
    return 0;
}

int is_array_variable(Environment *env, const char *varname) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, varname) == 0) {
            return env->vars[i].is_array;
        }
    }
    return 0;
}

int variable_exists(Environment *env, const char *varname) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->vars[i].name, varname) == 0) {
            return 1;
        }
    }
    return 0;
}

double eval(ASTNode *node, Environment *env);

double eval(ASTNode *node, Environment *env) {
    if (!node) return 0;
    
    switch (node->type) {
        case NODE_NUMBER:
            return node->data.number.value;
        
        case NODE_STRING:
            return 0;  // Strings don't have numeric value
        
        case NODE_VAR:
            if (!variable_exists(env, node->data.var.name)) {
                error_report("Undefined variable: '%s'", node->data.var.name);
                return 0;
            }
            return env_get(env, node->data.var.name);
        
        case NODE_BINOP: {
            double left = eval(node->data.binop.left, env);
            double right = eval(node->data.binop.right, env);
            
            // Check for error from operand evaluation
            if (has_error()) {
                return 0;
            }
            
            switch (node->data.binop.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': 
                    if (right == 0) {
                        error_report("Division by zero error");
                        return 0;
                    }
                    return left / right;
                case '%': 
                    if ((int)right == 0) {
                        error_report("Modulo by zero error");
                        return 0;
                    }
                    return (int)left % (int)right;
                case '^': return pow(left, right);
                case '&': return (int)left & (int)right;
                case '|': return (int)left | (int)right;
                case '<': return (left < right) ? 1 : 0;
                case 'l': return (int)left << (int)right;
                case 'r': return (int)left >> (int)right;
                case 'L': return (left <= right) ? 1 : 0;
                case '>': return (left > right) ? 1 : 0;
                case 'G': return (left >= right) ? 1 : 0;
                case '=': return (left == right) ? 1 : 0;
                case '!': return (left != right) ? 1 : 0;
                case 'A': return (left != 0 && right != 0) ? 1 : 0;  // AND
                case 'O': return (left != 0 || right != 0) ? 1 : 0;  // OR
            }
            return 0;
        }
        
        case NODE_UNOP: {
            double val = eval(node->data.unop.operand, env);
            if (node->data.unop.op == '-') return -val;
            if (node->data.unop.op == 'N') return (val == 0) ? 1 : 0;  // NOT
            return 0;
        }
        
        case NODE_DECL: {
            if (node->data.decl.value->type == NODE_ARRAY_INIT) {
                // Handle array initialization
                ASTNode *arr_init = node->data.decl.value;
                
                // Check if this is a TEXT array
                if (node->data.decl.type == TOK_TEXT) {
                    // String array
                    char **string_array = malloc(sizeof(char*) * arr_init->data.array_init.count);
                    for (int i = 0; i < arr_init->data.array_init.count; i++) {
                        ASTNode *elem = arr_init->data.array_init.elements[i];
                        if (elem->type == NODE_STRING) {
                            string_array[i] = malloc(256);
                            strcpy(string_array[i], elem->data.string.value);
                        } else {
                            // If not a string literal, try to convert
                            string_array[i] = malloc(256);
                            sprintf(string_array[i], "%.0f", eval(elem, env));
                        }
                    }
                    env_set_string_array(env, node->data.decl.varname, string_array, arr_init->data.array_init.count);
                } else {
                    // Numeric array
                    double *array = malloc(sizeof(double) * arr_init->data.array_init.count);
                    for (int i = 0; i < arr_init->data.array_init.count; i++) {
                        array[i] = eval(arr_init->data.array_init.elements[i], env);
                    }
                    env_set_array(env, node->data.decl.varname, array, arr_init->data.array_init.count);
                }
            } else if (node->data.decl.value->type == NODE_STRING) {
                // Handle string initialization
                env_set_string(env, node->data.decl.varname, node->data.decl.value->data.string.value);
            } else {
                // Handle numeric initialization
                double val = eval(node->data.decl.value, env);
                env_set(env, node->data.decl.varname, val);
            }
            return 0;
        }
        
        case NODE_ARRAY_ACCESS: {
            // Check if array exists
            if (!variable_exists(env, node->data.array_access.name)) {
                error_report("Undefined array: '%s'", node->data.array_access.name);
                return 0;
            }
            
            int index = (int)eval(node->data.array_access.index, env);
            
            if (has_error()) {
                return 0;
            }
            
            // Check bounds
            for (int i = 0; i < env->var_count; i++) {
                if (strcmp(env->vars[i].name, node->data.array_access.name) == 0) {
                    if (index < 0 || index >= env->vars[i].array_size) {
                        error_report("Array index out of bounds: %s[%d] (size=%d)", 
                                   node->data.array_access.name, index, env->vars[i].array_size);
                        return 0;
                    }
                    
                    if (env->vars[i].is_string_array) {
                        return 0;  // String arrays handled elsewhere
                    }
                    break;
                }
            }
            
            return env_get_array_element(env, node->data.array_access.name, index);
        }
        
        case NODE_ASSIGN: {
            double val = eval(node->data.assign.value, env);
            env_set(env, node->data.assign.varname, val);
            return 0;
        }
        
        case NODE_OUTPUT: {
            ASTNode *expr = node->data.output.expr;
            if (expr->type == NODE_STRING) {
                printf("%s\n", expr->data.string.value);
            } else if (expr->type == NODE_ARRAY_ACCESS) {
                // Handle array element access
                int index = (int)eval(expr->data.array_access.index, env);
                // Check if this is a string array
                for (int i = 0; i < env->var_count; i++) {
                    if (strcmp(env->vars[i].name, expr->data.array_access.name) == 0) {
                        if (env->vars[i].is_string_array) {
                            char *elem = env_get_string_array_element(env, expr->data.array_access.name, index);
                            printf("%s\n", elem);
                        } else {
                            double val = env_get_array_element(env, expr->data.array_access.name, index);
                            printf("%.0f\n", val);
                        }
                        return 0;
                    }
                }
            } else if (expr->type == NODE_VAR) {
                // Check if it's a string variable
                for (int i = 0; i < env->var_count; i++) {
                    if (strcmp(env->vars[i].name, expr->data.var.name) == 0) {
                        if (env->vars[i].is_string) {
                            printf("%s\n", env->vars[i].string_value);
                        } else {
                            printf("%.0f\n", env->vars[i].value);
                        }
                        return 0;
                    }
                }
                printf("%.0f\n", env_get(env, expr->data.var.name));
            } else {
                double val = eval(expr, env);
                printf("%.0f\n", val);
            }
            return 0;
        }
        
        case NODE_WHILE: {
            int count = 0;
            while (eval(node->data.while_node.condition, env) && count++ < 100000) {
                eval(node->data.while_node.body, env);
            }
            return 0;
        }
        
        case NODE_FOR: {
            int count = 0;
            if (node->data.for_node.init) {
                eval(node->data.for_node.init, env);
            }
            while (eval(node->data.for_node.condition, env) && count++ < 100000) {
                eval(node->data.for_node.body, env);
                if (node->data.for_node.increment) {
                    eval(node->data.for_node.increment, env);
                }
            }
            return 0;
        }
        
        case NODE_IF: {
            if (eval(node->data.if_node.condition, env)) {
                eval(node->data.if_node.body, env);
            } else if (node->data.if_node.else_body) {
                eval(node->data.if_node.else_body, env);
            }
            return 0;
        }
        
        case NODE_WAHL: {
            double expr_val = eval(node->data.wahl_node.expr, env);
            int matched = 0;
            
            for (int i = 0; i < node->data.wahl_node.case_count; i++) {
                if (expr_val == node->data.wahl_node.cases[i].value) {
                    eval(node->data.wahl_node.cases[i].body, env);
                    matched = 1;
                    break;  // Exit after first match (no fall-through)
                }
            }
            
            if (!matched && node->data.wahl_node.default_body) {
                eval(node->data.wahl_node.default_body, env);
            }
            
            return 0;
        }
        
        case NODE_CALL: {
            if (strcmp(node->data.call.name, "quadrat_wurzel") == 0) {
                if (node->data.call.arg_count > 0) {
                    double arg = eval(node->data.call.args[0], env);
                    if (arg < 0) {
                        error_report("Math domain error: quadrat_wurzel() of negative number (%.2f)", arg);
                        return 0;
                    }
                    return sqrt(arg);
                }
            } else if (strcmp(node->data.call.name, "mathe_sin") == 0) {
                if (node->data.call.arg_count > 0) {
                    double arg = eval(node->data.call.args[0], env);
                    return sin(arg * 3.14159265359 / 180.0);
                }
            } else if (strcmp(node->data.call.name, "mathe_cos") == 0) {
                if (node->data.call.arg_count > 0) {
                    double arg = eval(node->data.call.args[0], env);
                    return cos(arg * 3.14159265359 / 180.0);
                }
            } else if (strcmp(node->data.call.name, "mathe_tan") == 0) {
                if (node->data.call.arg_count > 0) {
                    double arg = eval(node->data.call.args[0], env);
                    return tan(arg * 3.14159265359 / 180.0);
                }
            } else if (strcmp(node->data.call.name, "mathe_log") == 0) {
                if (node->data.call.arg_count > 0) {
                    double arg = eval(node->data.call.args[0], env);
                    if (arg <= 0) {
                        error_report("Math domain error: mathe_log() of non-positive number (%.2f)", arg);
                        return 0;
                    }
                    return log(arg);
                }
            } else {
                error_report("Undefined function: '%s'", node->data.call.name);
                return 0;
            }
            return 0;
        }
        
        case NODE_BLOCK: {
            for (int i = 0; i < node->data.block.count; i++) {
                eval(node->data.block.statements[i], env);
            }
            return 0;
        }
        
        case NODE_ARRAY_METHOD: {
            char *array_name = node->data.array_method.array_name;
            int method = node->data.array_method.method;
            
            // Find the array in environment
            for (int i = 0; i < env->var_count; i++) {
                if (strcmp(env->vars[i].name, array_name) == 0) {
                    if (method == 0) {  // hinzufuegen - append
                        double value = eval(node->data.array_method.arg, env);
                        int new_size = env->vars[i].array_size + 1;
                        
                        if (env->vars[i].is_string_array) {
                            // For string arrays, this shouldn't happen typically
                        } else {
                            // Resize numeric array
                            double *new_array = malloc(sizeof(double) * new_size);
                            for (int j = 0; j < env->vars[i].array_size; j++) {
                                new_array[j] = env->vars[i].array[j];
                            }
                            new_array[env->vars[i].array_size] = value;
                            
                            if (env->vars[i].array) free(env->vars[i].array);
                            env->vars[i].array = new_array;
                            env->vars[i].array_size = new_size;
                        }
                    } else if (method == 1) {  // entfernen - remove last
                        if (env->vars[i].array_size > 0) {
                            if (env->vars[i].is_string_array) {
                                if (env->vars[i].string_array[env->vars[i].array_size - 1]) {
                                    free(env->vars[i].string_array[env->vars[i].array_size - 1]);
                                }
                                env->vars[i].array_size--;
                            } else {
                                env->vars[i].array_size--;
                            }
                        }
                    } else if (method == 2) {  // sortieren - sort
                        if (env->vars[i].is_string_array) {
                            // Sort string array
                            for (int j = 0; j < env->vars[i].array_size - 1; j++) {
                                for (int k = j + 1; k < env->vars[i].array_size; k++) {
                                    if (strcmp(env->vars[i].string_array[j], env->vars[i].string_array[k]) > 0) {
                                        char *temp = env->vars[i].string_array[j];
                                        env->vars[i].string_array[j] = env->vars[i].string_array[k];
                                        env->vars[i].string_array[k] = temp;
                                    }
                                }
                            }
                        } else {
                            // Sort numeric array using bubble sort
                            for (int j = 0; j < env->vars[i].array_size - 1; j++) {
                                for (int k = j + 1; k < env->vars[i].array_size; k++) {
                                    if (env->vars[i].array[j] > env->vars[i].array[k]) {
                                        double temp = env->vars[i].array[j];
                                        env->vars[i].array[j] = env->vars[i].array[k];
                                        env->vars[i].array[k] = temp;
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
            
            return 0;
        }
        
        default:
            return 0;
    }
}

/* ============= INTERMEDIATE CODE GENERATION (TAC) ============= */

typedef enum {
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV, TAC_MOD, TAC_POW,
    TAC_AND, TAC_OR, TAC_NOT, TAC_BITAND, TAC_BITOR,
    TAC_LT, TAC_LE, TAC_GT, TAC_GE, TAC_EQ, TAC_NE,
    TAC_LSHIFT, TAC_RSHIFT,
    TAC_ASSIGN, TAC_LOAD_CONST, TAC_LOAD_VAR, TAC_STORE_VAR,
    TAC_ARRAY_ACCESS, TAC_ARRAY_ASSIGN,
    TAC_CALL, TAC_RETURN,
    TAC_LABEL, TAC_GOTO, TAC_COND_GOTO,
    TAC_PRINT, TAC_INPUT,
    TAC_ARRAY_APPEND, TAC_ARRAY_REMOVE, TAC_ARRAY_SORT
} TACOpType;

typedef struct {
    TACOpType op;
    char arg1[256];
    char arg2[256];
    char result[256];
    char label[256];
} TACInstruction;

typedef struct {
    TACInstruction *instructions;
    int count;
    int capacity;
} TACProgram;

int temp_var_counter = 0;
int label_counter = 0;

TACProgram *tac_create() {
    TACProgram *prog = malloc(sizeof(TACProgram));
    prog->capacity = 10000;
    prog->count = 0;
    prog->instructions = malloc(sizeof(TACInstruction) * prog->capacity);
    return prog;
}

char *tac_new_temp() {
    static char temp[256];
    sprintf(temp, "_t%d", temp_var_counter++);
    return strdup(temp);
}

char *tac_new_label() {
    static char label[256];
    sprintf(label, "_L%d", label_counter++);
    return strdup(label);
}

void tac_emit(TACProgram *prog, TACOpType op, const char *arg1, 
              const char *arg2, const char *result) {
    if (prog->count >= prog->capacity) {
        prog->capacity *= 2;
        prog->instructions = realloc(prog->instructions, 
                                    sizeof(TACInstruction) * prog->capacity);
    }
    
    TACInstruction *instr = &prog->instructions[prog->count++];
    instr->op = op;
    strcpy(instr->arg1, arg1 ? arg1 : "");
    strcpy(instr->arg2, arg2 ? arg2 : "");
    strcpy(instr->result, result ? result : "");
    strcpy(instr->label, "");
}

void tac_emit_label(TACProgram *prog, const char *label) {
    if (prog->count >= prog->capacity) {
        prog->capacity *= 2;
        prog->instructions = realloc(prog->instructions, 
                                    sizeof(TACInstruction) * prog->capacity);
    }
    
    TACInstruction *instr = &prog->instructions[prog->count++];
    instr->op = TAC_LABEL;
    strcpy(instr->label, label);
    strcpy(instr->arg1, "");
    strcpy(instr->arg2, "");
    strcpy(instr->result, "");
}

char *generate_tac_expression(ASTNode *node, TACProgram *prog);
void generate_tac_statement(ASTNode *node, TACProgram *prog);

char *generate_tac_expression(ASTNode *node, TACProgram *prog) {
    if (!node) return "";
    
    static char result[256];
    char *temp;
    char *left_var;
    char *right_var;
    
    switch (node->type) {
        case NODE_NUMBER: {
            temp = tac_new_temp();
            sprintf(result, "%.0f", node->data.number.value);
            tac_emit(prog, TAC_LOAD_CONST, result, "", temp);
            strcpy(result, temp);
            return result;
        }
        
        case NODE_STRING: {
            temp = tac_new_temp();
            tac_emit(prog, TAC_LOAD_CONST, node->data.string.value, "", temp);
            strcpy(result, temp);
            return result;
        }
        
        case NODE_VAR: {
            temp = tac_new_temp();
            tac_emit(prog, TAC_LOAD_VAR, node->data.var.name, "", temp);
            strcpy(result, temp);
            return result;
        }
        
        case NODE_BINOP: {
            left_var = generate_tac_expression(node->data.binop.left, prog);
            right_var = generate_tac_expression(node->data.binop.right, prog);
            temp = tac_new_temp();
            
            TACOpType tac_op = TAC_ADD;
            switch (node->data.binop.op) {
                case '+': tac_op = TAC_ADD; break;
                case '-': tac_op = TAC_SUB; break;
                case '*': tac_op = TAC_MUL; break;
                case '/': tac_op = TAC_DIV; break;
                case '%': tac_op = TAC_MOD; break;
                case '^': tac_op = TAC_POW; break;
                case '&': tac_op = TAC_BITAND; break;
                case '|': tac_op = TAC_BITOR; break;
                case '<': tac_op = TAC_LT; break;
                case '>': tac_op = TAC_GT; break;
                case '=': tac_op = TAC_EQ; break;
                case '!': tac_op = TAC_NE; break;
            }
            
            tac_emit(prog, tac_op, left_var, right_var, temp);
            strcpy(result, temp);
            return result;
        }
        
        case NODE_UNOP: {
            char *operand_var = generate_tac_expression(node->data.unop.operand, prog);
            temp = tac_new_temp();
            
            if (node->data.unop.op == '-') {
                tac_emit(prog, TAC_SUB, "0", operand_var, temp);
            } else if (node->data.unop.op == 'N') {
                tac_emit(prog, TAC_NOT, operand_var, "", temp);
            }
            
            strcpy(result, temp);
            return result;
        }
        
        case NODE_ARRAY_ACCESS: {
            char *index_var = generate_tac_expression(node->data.array_access.index, prog);
            temp = tac_new_temp();
            
            char arg[512];
            sprintf(arg, "%s[%s]", node->data.array_access.name, index_var);
            tac_emit(prog, TAC_ARRAY_ACCESS, arg, "", temp);
            
            strcpy(result, temp);
            return result;
        }
        
        case NODE_CALL: {
            temp = tac_new_temp();
            
            char args_str[1024] = "";
            for (int i = 0; i < node->data.call.arg_count; i++) {
                char *arg_var = generate_tac_expression(node->data.call.args[i], prog);
                strcat(args_str, arg_var);
                if (i < node->data.call.arg_count - 1) strcat(args_str, ",");
            }
            
            char call_str[512];
            sprintf(call_str, "%s(%s)", node->data.call.name, args_str);
            tac_emit(prog, TAC_CALL, call_str, "", temp);
            
            strcpy(result, temp);
            return result;
        }
        
        default:
            strcpy(result, "0");
            return result;
    }
}

void generate_tac_statement(ASTNode *node, TACProgram *prog) {
    if (!node) return;
    
    char *expr_var;
    char *cond_var;
    char *loop_start_label;
    char *loop_end_label;
    char *else_label;
    char *endif_label;
    
    switch (node->type) {
        case NODE_DECL: {
            if (node->data.decl.value && node->data.decl.value->type != NODE_ARRAY_INIT) {
                expr_var = generate_tac_expression(node->data.decl.value, prog);
                tac_emit(prog, TAC_STORE_VAR, expr_var, "", node->data.decl.varname);
            }
            break;
        }
        
        case NODE_ASSIGN: {
            expr_var = generate_tac_expression(node->data.assign.value, prog);
            tac_emit(prog, TAC_STORE_VAR, expr_var, "", node->data.assign.varname);
            break;
        }
        
        case NODE_OUTPUT: {
            expr_var = generate_tac_expression(node->data.output.expr, prog);
            tac_emit(prog, TAC_PRINT, expr_var, "", "");
            break;
        }
        
        case NODE_WHILE: {
            loop_start_label = tac_new_label();
            loop_end_label = tac_new_label();
            
            tac_emit_label(prog, loop_start_label);
            
            cond_var = generate_tac_expression(node->data.while_node.condition, prog);
            tac_emit(prog, TAC_COND_GOTO, cond_var, "", loop_end_label);
            
            generate_tac_statement(node->data.while_node.body, prog);
            tac_emit(prog, TAC_GOTO, "", "", loop_start_label);
            
            tac_emit_label(prog, loop_end_label);
            break;
        }
        
        case NODE_FOR: {
            loop_start_label = tac_new_label();
            loop_end_label = tac_new_label();
            
            if (node->data.for_node.init) {
                generate_tac_statement(node->data.for_node.init, prog);
            }
            
            tac_emit_label(prog, loop_start_label);
            
            cond_var = generate_tac_expression(node->data.for_node.condition, prog);
            tac_emit(prog, TAC_COND_GOTO, cond_var, "", loop_end_label);
            
            generate_tac_statement(node->data.for_node.body, prog);
            
            if (node->data.for_node.increment) {
                generate_tac_statement(node->data.for_node.increment, prog);
            }
            
            tac_emit(prog, TAC_GOTO, "", "", loop_start_label);
            
            tac_emit_label(prog, loop_end_label);
            break;
        }
        
        case NODE_IF: {
            else_label = tac_new_label();
            endif_label = tac_new_label();
            
            cond_var = generate_tac_expression(node->data.if_node.condition, prog);
            tac_emit(prog, TAC_COND_GOTO, cond_var, "", else_label);
            
            generate_tac_statement(node->data.if_node.body, prog);
            tac_emit(prog, TAC_GOTO, "", "", endif_label);
            
            tac_emit_label(prog, else_label);
            if (node->data.if_node.else_body) {
                generate_tac_statement(node->data.if_node.else_body, prog);
            }
            
            tac_emit_label(prog, endif_label);
            break;
        }
        
        case NODE_BLOCK: {
            for (int i = 0; i < node->data.block.count; i++) {
                generate_tac_statement(node->data.block.statements[i], prog);
            }
            break;
        }
        
        case NODE_ARRAY_METHOD: {
            if (node->data.array_method.method == 0) {
                char *value_var = generate_tac_expression(node->data.array_method.arg, prog);
                tac_emit(prog, TAC_ARRAY_APPEND, node->data.array_method.array_name, value_var, "");
            } else if (node->data.array_method.method == 1) {
                tac_emit(prog, TAC_ARRAY_REMOVE, node->data.array_method.array_name, "", "");
            } else if (node->data.array_method.method == 2) {
                tac_emit(prog, TAC_ARRAY_SORT, node->data.array_method.array_name, "", "");
            }
            break;
        }
        
        default:
            break;
    }
}

void tac_print(TACProgram *prog) {
    printf("\n========== INTERMEDIATE CODE (THREE-ADDRESS CODE) ==========\n\n");
    
    for (int i = 0; i < prog->count; i++) {
        TACInstruction *instr = &prog->instructions[i];
        
        printf("[%3d] ", i);
        
        switch (instr->op) {
            case TAC_ADD:
                printf("%s = %s + %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_SUB:
                printf("%s = %s - %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_MUL:
                printf("%s = %s * %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_DIV:
                printf("%s = %s / %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_MOD:
                printf("%s = %s %% %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_POW:
                printf("%s = %s ^ %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LT:
                printf("%s = %s < %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LE:
                printf("%s = %s <= %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_GT:
                printf("%s = %s > %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_GE:
                printf("%s = %s >= %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_EQ:
                printf("%s = %s == %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_NE:
                printf("%s = %s != %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_BITAND:
                printf("%s = %s & %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_BITOR:
                printf("%s = %s | %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LSHIFT:
                printf("%s = %s << %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_RSHIFT:
                printf("%s = %s >> %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_AND:
                printf("%s = %s AND %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_OR:
                printf("%s = %s OR %s", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_NOT:
                printf("%s = NOT %s", instr->result, instr->arg1);
                break;
            case TAC_LOAD_CONST:
                printf("%s = CONST %s", instr->result, instr->arg1);
                break;
            case TAC_LOAD_VAR:
                printf("%s = LOAD %s", instr->result, instr->arg1);
                break;
            case TAC_STORE_VAR:
                printf("STORE %s = %s", instr->result, instr->arg1);
                break;
            case TAC_ARRAY_ACCESS:
                printf("%s = %s", instr->result, instr->arg1);
                break;
            case TAC_CALL:
                printf("%s = CALL %s", instr->result, instr->arg1);
                break;
            case TAC_PRINT:
                printf("PRINT %s", instr->arg1);
                break;
            case TAC_LABEL:
                printf("%s:", instr->label);
                break;
            case TAC_GOTO:
                printf("GOTO %s", instr->label);
                break;
            case TAC_COND_GOTO:
                printf("IF %s GOTO %s", instr->arg1, instr->label);
                break;
            case TAC_ARRAY_APPEND:
                printf("APPEND %s <- %s", instr->arg1, instr->arg2);
                break;
            case TAC_ARRAY_REMOVE:
                printf("REMOVE %s", instr->arg1);
                break;
            case TAC_ARRAY_SORT:
                printf("SORT %s", instr->arg1);
                break;
            default:
                printf("UNKNOWN");
                break;
        }
        
        printf("\n");
    }
    
    printf("\n===========================================================\n\n");
}

void tac_save_to_file(TACProgram *prog, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    
    fprintf(f, "INTERMEDIATE CODE (THREE-ADDRESS CODE)\n");
    fprintf(f, "=====================================\n\n");
    
    for (int i = 0; i < prog->count; i++) {
        TACInstruction *instr = &prog->instructions[i];
        
        fprintf(f, "[%3d] ", i);
        
        switch (instr->op) {
            case TAC_ADD:
                fprintf(f, "%s = %s + %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_SUB:
                fprintf(f, "%s = %s - %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_MUL:
                fprintf(f, "%s = %s * %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_DIV:
                fprintf(f, "%s = %s / %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_MOD:
                fprintf(f, "%s = %s %% %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_POW:
                fprintf(f, "%s = %s ^ %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LT:
                fprintf(f, "%s = %s < %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LE:
                fprintf(f, "%s = %s <= %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_GT:
                fprintf(f, "%s = %s > %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_GE:
                fprintf(f, "%s = %s >= %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_EQ:
                fprintf(f, "%s = %s == %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_NE:
                fprintf(f, "%s = %s != %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_BITAND:
                fprintf(f, "%s = %s & %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_BITOR:
                fprintf(f, "%s = %s | %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_LSHIFT:
                fprintf(f, "%s = %s << %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_RSHIFT:
                fprintf(f, "%s = %s >> %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_AND:
                fprintf(f, "%s = %s AND %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_OR:
                fprintf(f, "%s = %s OR %s\n", instr->result, instr->arg1, instr->arg2);
                break;
            case TAC_NOT:
                fprintf(f, "%s = NOT %s\n", instr->result, instr->arg1);
                break;
            case TAC_LOAD_CONST:
                fprintf(f, "%s = CONST %s\n", instr->result, instr->arg1);
                break;
            case TAC_LOAD_VAR:
                fprintf(f, "%s = LOAD %s\n", instr->result, instr->arg1);
                break;
            case TAC_STORE_VAR:
                fprintf(f, "STORE %s = %s\n", instr->result, instr->arg1);
                break;
            case TAC_ARRAY_ACCESS:
                fprintf(f, "%s = %s\n", instr->result, instr->arg1);
                break;
            case TAC_CALL:
                fprintf(f, "%s = CALL %s\n", instr->result, instr->arg1);
                break;
            case TAC_PRINT:
                fprintf(f, "PRINT %s\n", instr->arg1);
                break;
            case TAC_LABEL:
                fprintf(f, "%s:\n", instr->label);
                break;
            case TAC_GOTO:
                fprintf(f, "GOTO %s\n", instr->label);
                break;
            case TAC_COND_GOTO:
                fprintf(f, "IF %s GOTO %s\n", instr->arg1, instr->label);
                break;
            case TAC_ARRAY_APPEND:
                fprintf(f, "APPEND %s <- %s\n", instr->arg1, instr->arg2);
                break;
            case TAC_ARRAY_REMOVE:
                fprintf(f, "REMOVE %s\n", instr->arg1);
                break;
            case TAC_ARRAY_SORT:
                fprintf(f, "SORT %s\n", instr->arg1);
                break;
            default:
                fprintf(f, "UNKNOWN\n");
                break;
        }
    }
    
    fclose(f);
}

int main() {
    // Initialize error handler
    error_handler = error_handler_create();
    
    char input[100000];
    int len = 0;
    
    while (fgets(input + len, sizeof(input) - len, stdin)) {
        len = strlen(input);
    }
    
    Parser *parser = parser_create(input);
    
    ASTNode *program = malloc(sizeof(ASTNode));
    program->type = NODE_BLOCK;
    program->data.block.statements = malloc(sizeof(ASTNode*) * 1000);
    program->data.block.count = 0;
    
    while (parser->current_token->type != TOK_EOF) {
        ASTNode *stmt = parser_parse_statement(parser);
        if (stmt) {
            program->data.block.statements[program->data.block.count++] = stmt;
        }
    }
    
    // Generate intermediate code
    TACProgram *tac_prog = tac_create();
    generate_tac_statement(program, tac_prog);
    
    // Print TAC to console
    tac_print(tac_prog);
    
    // Save TAC to file
    tac_save_to_file(tac_prog, "tac_output.txt");
    
    // Execute the program (original AST interpretation)
    Environment *env = env_create();
    eval(program, env);
    
    // Print error summary if any errors occurred
    if (error_handler->error_count > 0) {
        fprintf(stderr, "\n[ERROR SUMMARY] %d error(s) encountered during execution\n", error_handler->error_count);
    }
    
    return 0;
}
