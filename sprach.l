%{
#include <stdio.h>
#include <stdlib.h>
%}

DIGIT   [0-9]
LETTER  [_a-zA-Z]
ID      {LETTER}({LETTER}|{DIGIT})*
INT     {DIGIT}+
DOUBLE  {DIGIT}+"."{DIGIT}+
STRING  \"([^\"\n])*\"
CHAR    \'([^\'\n])\'

%%

wenn        { printf("TOKEN: WENN\n"); }
sonst       { printf("TOKEN: SONST\n"); }
fur         { printf("TOKEN: FUR\n"); }
waehrend    { printf("TOKEN: WAEHREND\n"); }
wahl        { printf("TOKEN: WAHL\n"); }
fall        { printf("TOKEN: FALL\n"); }
standard    { printf("TOKEN: STANDARD\n"); }
leere       { printf("TOKEN: LEERE\n"); }

GANZ        { printf("TOKEN: INT_TYPE\n"); }
KOMM        { printf("TOKEN: DOUBLE_TYPE\n"); }
TEXT        { printf("TOKEN: STRING_TYPE\n"); }
ZEICHEN     { printf("TOKEN: CHAR_TYPE\n"); }

und         { printf("TOKEN: AND\n"); }
oder        { printf("TOKEN: OR\n"); }
nicht       { printf("TOKEN: NOT\n"); }

lesen       { printf("TOKEN: INPUT\n"); }
geben       { printf("TOKEN: OUTPUT\n"); }

hinzufuegen { printf("TOKEN: ARRAY_ADD\n"); }
entfernen   { printf("TOKEN: ARRAY_REMOVE\n"); }
sortieren   { printf("TOKEN: ARRAY_SORT\n"); }

{DOUBLE}    { printf("TOKEN: DOUBLE (%s)\n", yytext); }
{INT}       { printf("TOKEN: INT (%s)\n", yytext); }
{STRING}    { printf("TOKEN: STRING (%s)\n", yytext); }
{CHAR}      { printf("TOKEN: CHAR (%s)\n", yytext); }

{ID}        { printf("TOKEN: ID (%s)\n", yytext); }

"{"         { printf("TOKEN: {\n"); }
"}"         { printf("TOKEN: }\n"); }
"("         { printf("TOKEN: (\n"); }
")"         { printf("TOKEN: )\n"); }
";"         { printf("TOKEN: ;\n"); }
","         { printf("TOKEN: ,\n"); }

"+"|"-"|"*"|"/"                 { printf("TOKEN: ARITH_OP (%s)\n", yytext); }
"=="|"!="|"<"|">"|"<="|">="     { printf("TOKEN: REL_OP (%s)\n", yytext); }

"#".*                           { /* single line comment */ }
"##"([^#]|"#"[^#])*"##"         { /* multi line comment */ }

[ \t\n]+                        { /* for the whitespaces, ignoring */ }

.   { printf("TOKEN: UNKNOWN (%s)\n", yytext); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
