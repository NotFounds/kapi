%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    #include "sc.h"
    char label1[BUFSIZ];
    char label2[BUFSIZ];
    int args;
    int i;
%}

%union {
    char *lexeme;
    char *lexvalue;
}

%start program
%token TOKEN_EOF TOKEN_ERROR TOKEN_ID TOKEN_NUM TOKEN_COL
%token TOKEN_SEMICOL TOKEN_COLEQ TOKEN_EQ TOKEN_NE TOKEN_LT
%token TOKEN_LE TOKEN_GT TOKEN_GE TOKEN_LPAR TOKEN_RPAR
%token TOKEN_PLUS TOKEN_MINUS TOKEN_ASTER TOKEN_SLASH
%token TOKEN_PERCENT TOKEN_COMMA TOKEN_SHIFTL TOKEN_SHIFTR
%token TOKEN_LBRA TOKEN_RBRA
%token TOKEN_IF TOKEN_THEN TOKEN_ELSE TOKEN_WHILE TOKEN_DO
%token TOKEN_REPEAT TOKEN_UNTIL TOKEN_GOTO TOKEN_BEGIN
%token TOKEN_END TOKEN_LONG TOKEN_WORD TOKEN_BYTE TOKEN_AND
%token TOKEN_OR TOKEN_NOT TOKEN_RETURN TOKEN_FUNCTION
%type <lexeme> TOKEN_ID
%type <lexvalue> TOKEN_NUM

%%
factor
    : TOKEN_ID  { gen_code("load_id", $1); }
    | TOKEN_ID  { args = 0; }
    TOKEN_LPAR arglist_opt TOKEN_RPAR
                { gen_code("load_num", "0");
                  gen_code("store_arg", "-");
                  gen_code("call", $1);
                  gen_code("load_arg", "-");
                  for (i = 0; i < args; i++) {
                    gen_code("cancel_arg", "-");
                  }
                }
    | TOKEN_ID  { gen_code("load_adr", $1); }
      TOKEN_LBRA expression TOKEN_RBRA
                { gen_code("add_index", $1);
                  gen_code("load_ind", $1);
                }
    | TOKEN_NUM { gen_code("load_num", $1); }
    | TOKEN_LPAR expression TOKEN_RPAR
    | TOKEN_NOT factor { gen_code("not", "-"); }
    ;

arglist_opt
    : /* empty */
    | arglist
    ;

arglist
    : expression{ gen_code("store_arg", "-");
                  args += 1;
                }
    | arglist TOKEN_COMMA expression
                { gen_code("store_arg", "-");
                  args += 1;
                }
    ;

term
    : factor factorlist
    ;

factorlist
    : /* empty */
    | factorlist TOKEN_ASTER factor
                { gen_code("mul", "-"); }
    | factorlist TOKEN_SLASH factor
                { gen_code("div", "-"); }
    | factorlist TOKEN_PERCENT factor
                { gen_code("mod", "-"); }
    | factorlist TOKEN_SHIFTL factor
                { gen_code("shiftl", "-"); }
    | factorlist TOKEN_SHIFTR factor
                { gen_code("shiftr", "-"); }
    | factorlist TOKEN_AND factor
                { gen_code("and", "-"); }
    ;

polynomial
    : term termlist
    | TOKEN_PLUS term termlist
    | TOKEN_MINUS term
                { gen_code("neg", "-"); }
      termlist
    ;

termlist
    : /* empty */
    | termlist TOKEN_PLUS term
                { gen_code("add", "-"); }
    | termlist TOKEN_MINUS term
                { gen_code("sub", "-"); }
    | termlist TOKEN_OR term
                { gen_code("or", "-"); }
    ;

expression
    : polynomial
    | polynomial TOKEN_EQ polynomial
                { gen_code("eq", "-"); }
    | polynomial TOKEN_NE polynomial
                { gen_code("ne", "-"); }
    | polynomial TOKEN_LT polynomial
                { gen_code("lt", "-"); }
    | polynomial TOKEN_LE polynomial
                { gen_code("le", "-"); }
    | polynomial TOKEN_GT polynomial
                { gen_code("gt", "-"); }
    | polynomial TOKEN_GE polynomial
                { gen_code("ge", "-"); }
    ;

statement
    : /* empty */
    | TOKEN_ID TOKEN_COLEQ expression
                { gen_code("store_id", $1); }
    | TOKEN_ID TOKEN_COL
                { gen_code("label", $1); }
      statement
    | TOKEN_ID TOKEN_LPAR
                { args = 0; }
      arglist_opt
      TOKEN_RPAR{ gen_code("load_num", "0");
                  gen_code("store_arg", "-");
                  gen_code("call", $1);
                  gen_code("cancel_arg", "-");
                  for (i = 0; i < args; i++) {
                    gen_code("cancel_arg", "-");
                  }
      }
    | TOKEN_ID  { gen_code("load_adr", $1); }
      TOKEN_LBRA expression TOKEN_RBRA
                { gen_code("add_index", $1); }
      TOKEN_COLEQ expression
                { gen_code("store_ind", $1); }
    | TOKEN_IF expression TOKEN_THEN
                { gen_code("tst", "-");
                  strcpy(label1, new_label());
                  gen_code("jeq", label1);
                }
      statement else_opt
    | TOKEN_WHILE
                { strcpy(label1, new_label());
                  gen_code("label", label1);
                }
      expression TOKEN_DO { strcpy(label2, new_label());
                            gen_code("tst", "-");
                            gen_code("jeq", label2);
                          }
      statement { gen_code("jmp", label1);
                  gen_code("label", label2);
                }
    | TOKEN_REPEAT
                { strcpy(label1, new_label());
                  gen_code("label", label1);
                }
      statement TOKEN_UNTIL expression
                { gen_code("tst", "-");
                  gen_code("jeq", label1);
                }
    | TOKEN_GOTO TOKEN_ID { gen_code("jmp", $2); }
    | TOKEN_RETURN expression
                { gen_code("store_id", "ans");
                  gen_code("return", "-");
                }
    | TOKEN_BEGIN statementlist TOKEN_END
    ;

else_opt
    : /* empty */   { gen_code("label", label1); }
    | TOKEN_ELSE    { strcpy(label2, new_label());
                      gen_code("jmp", label2);
                      gen_code("label", label1);
                    }
      statement { gen_code("label", label2); }
    ;

statementlist
    : statement
    | statementlist TOKEN_SEMICOL statement
    ;

variable
    : TOKEN_LONG TOKEN_ID { gen_code("var_long", $2); }
    size_opt
    | TOKEN_WORD TOKEN_ID { gen_code("var_word", $2); }
    size_opt
    | TOKEN_BYTE TOKEN_ID { gen_code("var_byte", $2); }
    size_opt
    ;

size_opt
    : /* empty */
    | TOKEN_LBRA TOKEN_NUM TOKEN_RBRA
                          { gen_code("set_size", $2); }
    ;

function
    : TOKEN_FUNCTION TOKEN_ID
                          { gen_code("start_func", $2); }
      TOKEN_LPAR
      paramlist_opt
      TOKEN_RPAR          { gen_code("start_prologue", "-"); }
      variablelist        { gen_code("end_prologue", "-"); }
      statement           { gen_code("end_func", "-"); }
    ;

paramlist_opt
    : /* empty */
    | paramlist
    ;

paramlist
    : variable
    | paramlist TOKEN_COMMA variable
    ;

variablelist
    : /* empty */
    | variablelist variable TOKEN_SEMICOL
    ;

program
    :                     { gen_code("start", "main"); }
      decllist            { gen_code("label", "main"); }
      statement           { gen_code("end", "-"); }
    ;

decllist
    : /* empty */
    | decllist function TOKEN_SEMICOL
    | decllist variable addresspart TOKEN_SEMICOL
    ;

addresspart
    : /* empty */
    | TOKEN_COL TOKEN_NUM { gen_code("set_address", $2); }
    ;
%%
