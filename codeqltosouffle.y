/* Parser for codeql subset */
%{
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "helper/symbolStore.h"
#include "helper/populateMainDL.h"
#include "helper/utils.h"
#include "helper/nodetype.h"
#include "helper/ASTNode.h"
#include "helper/translate.h"
  
/* from the lexer */
extern int yylex();
%}

%union {
        struct ast *a;
        char *strval;
        char *subtok;
        int intval;
}

%token <strval> IMPORT
%token <strval> JAVA
%token <strval> GO
%token <strval> CSHARP
%token <strval> CPP
%token <strval> PYTHON
%token <strval> JAVASCRIPT
%token <strval> FROM
%token <strval> WHERE
%token <strval> OR
%token <strval> AND
%token <strval> IMPLIES
%token <strval> IF
%token <strval> ELSE
%token <strval> THEN
%token <strval> NOT
%token <strval> SELECT
%token <strval> AS
%token <strval> STRING_LITERAL
%token <strval> UPPER_ID
%token <strval> LOWER_ID
%token <strval> COMMA
%token <strval> LEFT_BRACKET
%token <strval> RIGHT_BRACKET
%token <strval> DOT
%token <strval> UNDERSCORE
%token <subtok> COMPARISON
%token <strval> RANGE
%token <strval> IS

%type <a> stmt import_stmt select_stmt
%type <a> select_opts from_opts where_opts range_opts
%type <a> formula call primary expr
%type <strval> import_opts   

%start stmt_list

%%

stmt_list: stmt { eval($1); }
  | stmt_list stmt { eval($2); }
  ;

stmt: import_stmt { $$ = $1; }
  |   select_stmt { $$ = $1; }
  ;

import_stmt: IMPORT import_opts { $$ = newast(IMPORT_STMT_NODE, 1, newstringval($2)); } ;
import_opts: JAVA { $$ = "java"; }
  | GO { $$ = "go"; }
  | CSHARP { $$ = "c#"; }
  | CPP  { $$ = "cpp"; }
  | PYTHON { $$ = "python"; }
  | JAVASCRIPT { $$ = "javscript"; }
  ;

select_stmt: SELECT select_opts { $$ = newast(SELECT_STMT_NODE, 1, $2); }
  | FROM from_opts SELECT select_opts  { $$ = newast(SELECT_STMT_NODE, 2, $2, $4); }
  | FROM from_opts WHERE where_opts SELECT select_opts { $$ = newast(SELECT_STMT_NODE, 3, $2, $4, $6); }
  | FROM from_opts RANGE range_opts WHERE where_opts SELECT select_opts { $$ = newast(SELECT_STMT_NODE, 4, $2, $4, $6, $8); }
  ;

select_opts: expr { $$ = newast(SELECT_OPTS_NODE, 1, $1); }
  | expr AS LOWER_ID { $$ = newast(SELECT_OPTS_NODE, 1, newstringval($3)); }
  | select_opts COMMA expr { $$ = newast(SELECT_OPTS_NODE, 2, $3, $1); }
  | select_opts COMMA expr AS LOWER_ID  { $$ = newast(SELECT_OPTS_NODE, 2, newstringval($5), $1); }
  ;

from_opts: UPPER_ID LOWER_ID { $$ = newast(FROM_OPTS_NODE, 2, newstringval($1), newstringval($2)); }
  | UPPER_ID LOWER_ID COMMA from_opts { $$ = newast(FROM_OPTS_NODE, 3, newstringval($1), newstringval($2), $4); }
  ;

range_opts: LOWER_ID IS STRING_LITERAL { $$ = newast(RANGE_OPTS_NODE, 2, newstringval($1), newstringval($3)); }
  | LOWER_ID IS STRING_LITERAL COMMA range_opts { $$ = newast(RANGE_OPTS_NODE, 3, newstringval($1), newstringval($3), $5); }
  ;

where_opts: formula { $$ = $1; };
formula: LEFT_BRACKET formula RIGHT_BRACKET { $$ = $2; }
  |   formula OR formula { $$ = newast(OR_FORMULA_NODE, 2, $1, $3); }
  |   formula AND formula { $$ = newast(AND_FORMULA_NODE, 2, $1, $3); }
  |   formula IMPLIES formula { $$ = newast(IMPLIES_FORMULA_NODE, 2, $1, $3); }
  |   IF formula THEN formula ELSE formula { $$ = newast(IF_FORMULA_NODE, 3, $2, $4, $6); }
  |   NOT formula { $$ = newast(NOT_FORMULA_NODE, 1, $2); }
  |   primary COMPARISON primary { $$ = newast(COMPARISON_FORMULA_NODE, 3, $1, newstringval($2), $3); }
  |   call { $$ = $1; }
  ;

primary: LOWER_ID { $$ = newstringval($1); }
  | STRING_LITERAL { $$ = newstringval($1); }
  | call { $$ = $1; }
  ;
call: LOWER_ID DOT LOWER_ID LEFT_BRACKET RIGHT_BRACKET { $$ = newast(CALL_NODE, 2, newstringval($1), newstringval($3)); }
  | LOWER_ID DOT LOWER_ID LEFT_BRACKET STRING_LITERAL RIGHT_BRACKET { $$ = newast(CALL_NODE, 3, newstringval($1), newstringval($3), newstringval($5)); }
  ;
expr: UNDERSCORE { $$ = newstringval($1); }
  | primary { $$ = $1; }
  ;
%%

int main(int ac, char **av)
{
  // initialize the symbol table
  initializeSymbolTable();

  extern FILE *yyin;
  if(ac > 1 && (yyin = fopen(av[1], "r")) == NULL) {
    perror(av[1]);
    exit(1);
  }
  if(yyparse())
    printf("CodeQL parse failed\n");
}
