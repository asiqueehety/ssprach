%{
#include <string.h>
#include <stdlib.h>
#include "simple_sprach.tab.h"
int line_num = 1;
%}

%%

wenn        { return WENN; }
sonst       { return SONST; }
fur         { return FUR; }
waehrend    { return WAEHREND; }
wahl        { return WAHL; }
fall        { return FALL; }
standard    { return STANDARD; }
leere       { return LEERE; }
geben       { return GEBEN; }
lesen       { return LESEN; }

GANZ        { return GANZ; }
KOMM        { return KOMM; }
TEXT        { return TEXT; }
ZEICHEN     { return ZEICHEN; }

hinzufuegen { return HINZUFUEGEN; }
entfernen   { return ENTFERNEN; }
sortieren   { return SORTIEREN; }

mathe_sin       { return MATHE_SIN; }
mathe_cos       { return MATHE_COS; }
mathe_tan       { return MATHE_TAN; }
quadrat_wurzel  { return QUADRAT_WURZEL; }
mathe_log       { return MATHE_LOG; }

und         { return UND; }
oder        { return ODER; }
nicht       { return NICHT; }

[0-9]+      { yylval.intval = atoi(yytext); return INT_VAL; }
[0-9]+"."[0-9]+ { yylval.doubleval = atof(yytext); return DOUBLE_VAL; }
\"[^\"]*\"  { yylval.strval = strdup(yytext); return STRING_VAL; }
[_a-zA-Z][_a-zA-Z0-9]* { yylval.strval = strdup(yytext); return ID; }

"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
";"         { return SEMICOLON; }
","         { return COMMA; }
":"         { return COLON; }
"="         { return ASSIGN; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"%"         { return MOD; }
"=="        { return EQ; }
"!="        { return NE; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }

"#"[^\n]*   { }

[ \t]       { }
"\n"        { line_num++; }

.           { }

%%

int yywrap() { return 1; }

