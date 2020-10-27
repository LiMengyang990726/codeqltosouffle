/* A Bison parser, made by GNU Bison 3.7.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CODEQLTOSOUFFLE_TAB_H_INCLUDED
# define YY_YY_CODEQLTOSOUFFLE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IMPORT = 258,                  /* IMPORT  */
    JAVA = 259,                    /* JAVA  */
    GO = 260,                      /* GO  */
    CSHARP = 261,                  /* CSHARP  */
    CPP = 262,                     /* CPP  */
    PYTHON = 263,                  /* PYTHON  */
    JAVASCRIPT = 264,              /* JAVASCRIPT  */
    FROM = 265,                    /* FROM  */
    WHERE = 266,                   /* WHERE  */
    OR = 267,                      /* OR  */
    AND = 268,                     /* AND  */
    IMPLIES = 269,                 /* IMPLIES  */
    IF = 270,                      /* IF  */
    ELSE = 271,                    /* ELSE  */
    THEN = 272,                    /* THEN  */
    NOT = 273,                     /* NOT  */
    SELECT = 274,                  /* SELECT  */
    AS = 275,                      /* AS  */
    STRING_LITERAL = 276,          /* STRING_LITERAL  */
    UPPER_ID = 277,                /* UPPER_ID  */
    LOWER_ID = 278,                /* LOWER_ID  */
    COMMA = 279,                   /* COMMA  */
    LEFT_BRACKET = 280,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 281,           /* RIGHT_BRACKET  */
    DOT = 282,                     /* DOT  */
    UNDERSCORE = 283,              /* UNDERSCORE  */
    COMPARISON = 284               /* COMPARISON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 46 "codeqltosouffle.y"

        struct ast *a;
        char *strval;
        char *subtok;
        int intval;

#line 100 "codeqltosouffle.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CODEQLTOSOUFFLE_TAB_H_INCLUDED  */
