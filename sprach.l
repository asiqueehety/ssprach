%{%{%{

#include <stdio.h>

#include <stdlib.h>#include <stdio.h>#include <stdio.h>

#include <string.h>

#include "sprach.tab.h"#include <stdlib.h>#include <stdlib.h>



extern int line_num;#include <string.h>#include <string.h>

int line_num = 1;

%}#include "sprach.tab.h"#include "sprach.tab.h"



DIGIT       [0-9]

LETTER      [_a-zA-Z]

ID          {LETTER}({LETTER}|{DIGIT})*extern int line_num;extern int line_num;

INT         {DIGIT}+

DOUBLE      {DIGIT}+\.{DIGIT}+int line_num = 1;int line_num = 1;

STRING      \"([^\"\n])*\"

CHAR        \'([^\'\n])\'%}



%%%}



\n              { line_num++; }DIGIT       [0-9]

wenn            { return WENN; }

sonst           { return SONST; }LETTER      [_a-zA-Z]DIGIT       [0-9]

fuer            { return FUR; }

waehrend        { return WAEHREND; }ID          {LETTER}({LETTER}|{DIGIT})*LETTER      [_a-zA-Z]

wahl            { return WAHL; }

fall            { return FALL; }INT         {DIGIT}+ID          {LETTER}({LETTER}|{DIGIT})*

standard        { return STANDARD; }

leere           { return LEERE; }DOUBLE      {DIGIT}+\.{DIGIT}+INT         {DIGIT}+

zurueck         { return RETURN; }

GANZ            { return INT_TYPE; }STRING      \"([^\"\n])*\"DOUBLE      {DIGIT}+\.{DIGIT}+

KOMM            { return DOUBLE_TYPE; }

TEXT            { return STRING_TYPE; }CHAR        \'([^\'\n])\'STRING      \"([^\"\n])*\"

ZEICHEN         { return CHAR_TYPE; }

und             { return AND; }CHAR        \'([^\'\n])\'

oder            { return OR; }

nicht           { return NOT; }%%

lesen           { return INPUT; }

geben           { return OUTPUT; }%%

hinzufuegen     { return ARRAY_ADD; }

entfernen       { return ARRAY_REMOVE; }\n              { line_num++; }

sortieren       { return ARRAY_SORT; }

mathe_sin       { return MATHE_SIN; }    /* Line tracking */

mathe_cos       { return MATHE_COS; }

mathe_tan       { return MATHE_TAN; }wenn            { return WENN; }\n              { line_num++; }

quadrat_wurzel  { return QUADRAT_WURZEL; }

mathe_log       { return MATHE_LOG; }sonst           { return SONST; }

{DOUBLE}        { yylval.floating = atof(yytext); return DOUBLE_VAL; }

{INT}           { yylval.integer = atoi(yytext); return INT_VAL; }fuer            { return FUR; }    /* Keywords - Control Flow */

{STRING}        { yylval.string = strdup(yytext); return STRING_VAL; }

{CHAR}          { yylval.character = yytext[1]; return CHAR_VAL; }waehrend        { return WAEHREND; }wenn            { return WENN; }

{ID}            { yylval.string = strdup(yytext); return ID; }

"+"             { return PLUS; }wahl            { return WAHL; }sonst\ wenn     { return SONST_WENN; }

"-"             { return MINUS; }

"*"             { return STAR; }fall            { return FALL; }sonst           { return SONST; }

"/"             { return SLASH; }

"%"             { return MOD; }standard        { return STANDARD; }fuer            { return FUR; }

"="             { return ASSIGN; }

"=="            { return EQ; }leere           { return LEERE; }in              { return IN; }

"!="            { return NEQ; }

"<"             { return LT; }zurueck         { return RETURN; }waehrend        { return WAEHREND; }

">"             { return GT; }

"<="            { return LE; }wahl            { return WAHL; }

">="            { return GE; }

"{"             { return LBRACE; }GANZ            { return INT_TYPE; }fall            { return FALL; }

"}"             { return RBRACE; }

"("             { return LPAREN; }KOMM            { return DOUBLE_TYPE; }standard        { return STANDARD; }

")"             { return RPAREN; }

"["             { return LBRACKET; }TEXT            { return STRING_TYPE; }leere           { return LEERE; }

"]"             { return RBRACKET; }

";"             { return SEMICOLON; }ZEICHEN         { return CHAR_TYPE; }zurueck         { return RETURN; }

","             { return COMMA; }

":"             { return COLON; }

"#"[^\n]*       { }

"##"            { int c, prev=0; while ((c=input())!=EOF) { if (prev=='#'&&c=='#') break; if (c=='\n') line_num++; prev=c; } }und             { return AND; }    /* Data Types */

[ \t\r]+        { }

.               { fprintf(stderr, "Warning: Unrecognized '%c' at line %d\n", yytext[0], line_num); }oder            { return OR; }GANZ            { return INT_TYPE; }



%%nicht           { return NOT; }KOMM            { return DOUBLE_TYPE; }



int yywrap(void) { return 1; }TEXT            { return STRING_TYPE; }


lesen           { return INPUT; }ZEICHEN         { return CHAR_TYPE; }

geben           { return OUTPUT; }

    /* Logical Operators */

hinzufuegen     { return ARRAY_ADD; }und             { return AND; }

entfernen       { return ARRAY_REMOVE; }oder            { return OR; }

sortieren       { return ARRAY_SORT; }nicht           { return NOT; }



mathe_sin       { return MATHE_SIN; }    /* Bitwise Operators */

mathe_cos       { return MATHE_COS; }bit_und         { return BIT_AND; }

mathe_tan       { return MATHE_TAN; }bit_oder        { return BIT_OR; }

quadrat_wurzel  { return QUADRAT_WURZEL; }bit_xor         { return BIT_XOR; }

mathe_log       { return MATHE_LOG; }bit_nicht       { return BIT_NOT; }

verschieb_l     { return SHIFT_LEFT; }

{DOUBLE}        { yylval.floating = atof(yytext); return DOUBLE_VAL; }verschieb_r     { return SHIFT_RIGHT; }

{INT}           { yylval.integer = atoi(yytext); return INT_VAL; }

{STRING}        {     /* I/O Operations */

                    yylval.string = strdup(yytext);lesen           { return INPUT; }

                    return STRING_VAL;geben           { return OUTPUT; }

                }

{CHAR}          { yylval.character = yytext[1]; return CHAR_VAL; }    /* Array Operations */

hinzufuegen     { return ARRAY_ADD; }

{ID}            {entfernen       { return ARRAY_REMOVE; }

                    yylval.string = strdup(yytext);sortieren       { return ARRAY_SORT; }

                    return ID;

                }    /* Mathematical Functions */

mathe_sin       { return MATHE_SIN; }

"+"             { return PLUS; }mathe_cos       { return MATHE_COS; }

"-"             { return MINUS; }mathe_tan       { return MATHE_TAN; }

"*"             { return STAR; }quadrat_wurzel  { return QUADRAT_WURZEL; }

"/"             { return SLASH; }mathe_log       { return MATHE_LOG; }

"%"             { return MOD; }

    /* Literals */

"="             { return ASSIGN; }{DOUBLE}        { yylval.floating = atof(yytext); return DOUBLE_VAL; }

{INT}           { yylval.integer = atoi(yytext); return INT_VAL; }

"=="            { return EQ; }{STRING}        { 

"!="            { return NEQ; }                    char* str = (char*)malloc(strlen(yytext) + 1);

"<"             { return LT; }                    strcpy(str, yytext);

">"             { return GT; }                    yylval.string = str;

"<="            { return LE; }                    return STRING_VAL;

">="            { return GE; }                }

{CHAR}          { yylval.character = yytext[1]; return CHAR_VAL; }

"{"             { return LBRACE; }

"}"             { return RBRACE; }    /* Identifiers */

"("             { return LPAREN; }{ID}            {

")"             { return RPAREN; }                    char* id = (char*)malloc(strlen(yytext) + 1);

"["             { return LBRACKET; }                    strcpy(id, yytext);

"]"             { return RBRACKET; }                    yylval.string = id;

";"             { return SEMICOLON; }                    return ID;

","             { return COMMA; }                }

":"             { return COLON; }

    /* Operators */

"#"[^\n]*       { /* Single-line comment */ }"+"             { return PLUS; }

"-"             { return MINUS; }

"##"            { /* Multi-line comment start */ "*"             { return STAR; }

                    int c;"/"             { return SLASH; }

                    int prev = 0;"%"             { return MOD; }

                    while ((c = input()) != EOF) {

                        if (prev == '#' && c == '#') {"="             { return ASSIGN; }

                            break;

                        }"=="            { return EQ; }

                        if (c == '\n') line_num++;"!="            { return NEQ; }

                        prev = c;"<"             { return LT; }

                    }">"             { return GT; }

                }"<="            { return LE; }

">="            { return GE; }

[ \t\r]+        { /* Skip whitespace */ }

    /* Delimiters */

.               { fprintf(stderr, "Warning: Unrecognized character '%c' at line %d\n", yytext[0], line_num); }"{"             { return LBRACE; }

"}"             { return RBRACE; }

%%"("             { return LPAREN; }

")"             { return RPAREN; }

int yywrap(void) {"["             { return LBRACKET; }

    return 1;"]"             { return RBRACKET; }

}";"             { return SEMICOLON; }

","             { return COMMA; }
":"             { return COLON; }

    /* Comments */
"#"[^\n]*       { /* Single-line comment */ }
"##"            { /* Multi-line comment start */ 
                    int c;
                    int prev = 0;
                    while ((c = input()) != EOF) {
                        if (prev == '#' && c == '#') {
                            break;
                        }
                        if (c == '\n') line_num++;
                        prev = c;
                    }
                }

    /* Whitespace */
[ \t\r]+        { /* Skip whitespace except newlines */ }

.               { fprintf(stderr, "Warning: Unrecognized character '%c' at line %d\n", yytext[0], line_num); }

%%

int yywrap(void) {
    return 1;
}
