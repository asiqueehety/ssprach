
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_VAL = 258,
     DOUBLE_VAL = 259,
     STRING_VAL = 260,
     ID = 261,
     GANZ = 262,
     KOMM = 263,
     TEXT = 264,
     ZEICHEN = 265,
     WENN = 266,
     SONST = 267,
     FUR = 268,
     WAEHREND = 269,
     WAHL = 270,
     FALL = 271,
     STANDARD = 272,
     LEERE = 273,
     UND = 274,
     ODER = 275,
     NICHT = 276,
     LESEN = 277,
     GEBEN = 278,
     HINZUFUEGEN = 279,
     ENTFERNEN = 280,
     SORTIEREN = 281,
     MATHE_SIN = 282,
     MATHE_COS = 283,
     MATHE_TAN = 284,
     QUADRAT_WURZEL = 285,
     MATHE_LOG = 286,
     LPAREN = 287,
     RPAREN = 288,
     LBRACE = 289,
     RBRACE = 290,
     LBRACKET = 291,
     RBRACKET = 292,
     SEMICOLON = 293,
     COMMA = 294,
     ASSIGN = 295,
     COLON = 296,
     GE = 297,
     GT = 298,
     LE = 299,
     LT = 300,
     NE = 301,
     EQ = 302,
     MINUS = 303,
     PLUS = 304,
     MOD = 305,
     DIV = 306,
     MUL = 307,
     NOT = 308,
     UMINUS = 309
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 44 "simple_sprach.y"
 
    int intval;
    double doubleval;
    char *strval;
    double result;



/* Line 1676 of yacc.c  */
#line 115 "simple_sprach.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


