#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* ============= LEXER ============= */

typedef enum {
    TOK_EOF, TOK_INT, TOK_DOUBLE, TOK_ID, TOK_STRING,
    TOK_GANZ, TOK_KOMM, TOK_TEXT, TOK_ZEICHEN,
    TOK_WENN, TOK_SONST, TOK_FUR, TOK_WAEHREND, TOK_WAHL, TOK_FALL,
    TOK_GEBEN, TOK_LESEN,
    TOK_UND, TOK_ODER, TOK_NICHT,
    TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_MOD,
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
        while (lex->current_char != '\0' && lex->current_char != '\n') {
            lexer_advance(lex);
        }
    }
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
    else if (strcmp(id_str, "fur") == 0) tok->type = TOK_FUR;
    else if (strcmp(id_str, "waehrend") == 0) tok->type = TOK_WAEHREND;
    else if (strcmp(id_str, "wahl") == 0) tok->type = TOK_WAHL;
    else if (strcmp(id_str, "fall") == 0) tok->type = TOK_FALL;
    else if (strcmp(id_str, "geben") == 0) tok->type = TOK_GEBEN;
    else if (strcmp(id_str, "lesen") == 0) tok->type = TOK_LESEN;
    else if (strcmp(id_str, "und") == 0) tok->type = TOK_UND;
    else if (strcmp(id_str, "oder") == 0) tok->type = TOK_ODER;
    else if (strcmp(id_str, "nicht") == 0) tok->type = TOK_NICHT;
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
        if (lex->current_char == '=') {
            tok->type = TOK_LE;
            lexer_advance(lex);
        } else {
            tok->type = TOK_LT;
        }
        return tok;
    }
    
    if (lex->current_char == '>') {
        lexer_advance(lex);
        if (lex->current_char == '=') {
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
    NODE_WHILE, NODE_IF, NODE_FOR,
    NODE_BINOP, NODE_UNOP, NODE_NUMBER, NODE_VAR, NODE_CALL,
    NODE_ARRAY_ACCESS, NODE_ARRAY_INIT
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
    } data;
} ASTNode;

ASTNode *ast_create_number(double value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->data.number.value = value;
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
ASTNode *parser_parse_statement(Parser *parser);
ASTNode *parser_parse_block(Parser *parser);

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
    if (tok->type == TOK_ID) {
        char name[256];
        strcpy(name, tok->value.strval);
        parser_advance(parser);
        
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
    ASTNode *left = parser_parse_primary(parser);
    
    while (parser->current_token->type == TOK_MUL ||
           parser->current_token->type == TOK_DIV ||
           parser->current_token->type == TOK_MOD) {
        char op = parser->current_token->type == TOK_MUL ? '*' :
                 parser->current_token->type == TOK_DIV ? '/' : '%';
        parser_advance(parser);
        ASTNode *right = parser_parse_primary(parser);
        left = ast_create_binop(op, left, right);
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
    ASTNode *left = parser_parse_arithmetic(parser);
    
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
        ASTNode *right = parser_parse_arithmetic(parser);
        left = ast_create_binop(op, left, right);
    }
    
    return left;
}

ASTNode *parser_parse_expression(Parser *parser) {
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
    
    parser_advance(parser);
    return NULL;
}

/* ============= INTERPRETER ============= */

#define MAX_VARS 1000
#define MAX_ARRAY_SIZE 1000

typedef struct {
    char name[256];
    double value;
    double *array;
    int array_size;
    int is_array;
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
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        env->vars[env->var_count].value = value;
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
            return;
        }
    }
    if (env->var_count < MAX_VARS) {
        strcpy(env->vars[env->var_count].name, name);
        env->vars[env->var_count].array = array;
        env->vars[env->var_count].array_size = size;
        env->vars[env->var_count].is_array = 1;
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

double eval(ASTNode *node, Environment *env);

double eval(ASTNode *node, Environment *env) {
    if (!node) return 0;
    
    switch (node->type) {
        case NODE_NUMBER:
            return node->data.number.value;
        
        case NODE_VAR:
            return env_get(env, node->data.var.name);
        
        case NODE_BINOP: {
            double left = eval(node->data.binop.left, env);
            double right = eval(node->data.binop.right, env);
            switch (node->data.binop.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return (right != 0) ? left / right : 0;
                case '%': return (int)left % (int)right;
                case '<': return (left < right) ? 1 : 0;
                case 'L': return (left <= right) ? 1 : 0;
                case '>': return (left > right) ? 1 : 0;
                case 'G': return (left >= right) ? 1 : 0;
                case '=': return (left == right) ? 1 : 0;
                case '!': return (left != right) ? 1 : 0;
            }
            return 0;
        }
        
        case NODE_UNOP: {
            double val = eval(node->data.unop.operand, env);
            if (node->data.unop.op == '-') return -val;
            return 0;
        }
        
        case NODE_DECL: {
            if (node->data.decl.value->type == NODE_ARRAY_INIT) {
                // Handle array initialization
                ASTNode *arr_init = node->data.decl.value;
                double *array = malloc(sizeof(double) * arr_init->data.array_init.count);
                for (int i = 0; i < arr_init->data.array_init.count; i++) {
                    array[i] = eval(arr_init->data.array_init.elements[i], env);
                }
                env_set_array(env, node->data.decl.varname, array, arr_init->data.array_init.count);
            } else {
                double val = eval(node->data.decl.value, env);
                env_set(env, node->data.decl.varname, val);
            }
            return 0;
        }
        
        case NODE_ARRAY_ACCESS: {
            int index = (int)eval(node->data.array_access.index, env);
            return env_get_array_element(env, node->data.array_access.name, index);
        }
        
        case NODE_ASSIGN: {
            double val = eval(node->data.assign.value, env);
            env_set(env, node->data.assign.varname, val);
            return 0;
        }
        
        case NODE_OUTPUT: {
            double val = eval(node->data.output.expr, env);
            printf("%.0f\n", val);
            return 0;
        }
        
        case NODE_WHILE: {
            int count = 0;
            while (eval(node->data.while_node.condition, env) && count++ < 100000) {
                eval(node->data.while_node.body, env);
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
        
        case NODE_BLOCK: {
            for (int i = 0; i < node->data.block.count; i++) {
                eval(node->data.block.statements[i], env);
            }
            return 0;
        }
        
        default:
            return 0;
    }
}

int main() {
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
    
    Environment *env = env_create();
    eval(program, env);
    
    return 0;
}
