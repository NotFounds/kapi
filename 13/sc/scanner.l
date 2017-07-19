%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <string.h>
    #include "parser.h"
    #include "sc.h"
    #ifdef DEBUG
        #ifdef ECHO
            #undef ECHO
        #endif
        #define ECHO fprintf(stderr, "[%s]\n", yytext)
    #else
        #ifdef ECHO
            #undef ECHO
        #endif
        #define ECHO
    #endif

    int stoi(char *s, int radix);
    static FILE *lexout;
    static char value[BUFSIZ];
    static int lineno = 1;
    static int i;
%}

%%
[ \t]+ {;}
\n { lineno++; }
"(*".*"*)" {;}
[_a-zA-Z][_a-zA-Z0-9]* { ECHO;
    if ((i = find_keywords(yytext)) >= 0) {
        return keywords[i].value;
    } else {
        yylval.lexeme = strdup(yytext);
        return TOKEN_ID;
    }
}
[0-9]+ { ECHO;
    sprintf(value, "%d", stoi(yytext, 10));
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"0b"[01]+ { ECHO;
    sprintf(value, "%d", stoi(yytext + 2, 2));
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"0x"[0-9a-fA-F]+ { ECHO;
    sprintf(value, "%d", stoi(yytext + 2, 16));
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"'\\n'" { ECHO;
    sprintf(value, "%d", '\n');
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"'\\t'" { ECHO;
    sprintf(value, "%d", '\t');
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"'\\0'" { ECHO;
    sprintf(value, "%d", '\0');
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"'\\''" { ECHO;
    sprintf(value, "%d", '\'');
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
"'\\\\'" { ECHO;
    sprintf(value, "%d", '\\');
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
\'.\' { ECHO;
    sprintf(value, "%d", yytext[1]);
    yylval.lexvalue = strdup(value);
    return TOKEN_NUM;
}
":=" { ECHO; return TOKEN_COLEQ; }
"=" { ECHO; return TOKEN_EQ; }
"<>"|"><" { ECHO; return TOKEN_NE; }
"<" { ECHO; return TOKEN_LT; }
"<="|"=<" { ECHO; return TOKEN_LE; }
">" { ECHO; return TOKEN_GT; }
">="|"=>" { ECHO; return TOKEN_GE; }
"+" { ECHO; return TOKEN_PLUS; }
"-" { ECHO; return TOKEN_MINUS; }
"*" { ECHO; return TOKEN_ASTER; }
"/" { ECHO; return TOKEN_SLASH; }
"(" { ECHO; return TOKEN_LPAR; }
")" { ECHO; return TOKEN_RPAR; }
";" { ECHO; return TOKEN_SEMICOL; }
":" { ECHO; return TOKEN_COL; }
"%" { ECHO; return TOKEN_PERCENT; }
"<<" { ECHO; return TOKEN_SHIFTL; }
">>" { ECHO; return TOKEN_SHIFTR; }
"," { ECHO; return TOKEN_COMMA; }
"[" { ECHO; return TOKEN_LBRA; }
"]" { ECHO; return TOKEN_RBRA; }
. { ECHO; return TOKEN_ERROR; }
%%

void yyerror()
{
    error(ERROR_SYNTAX, yytext, lineno);
}

int stoi(char *s, int radix)
{
    int ans;
    int i;

    ans = 0;
    while (*s != '\0') {
        i = char_pos("0123456789abcdef", tolower(*s));
        if (i < 0 || radix <= i) {
            error(ERROR_INTERNAL, "stoi()", 0);
            return -1;
        }
        ans = ans * radix + i;
        s++;
    }
    return ans;
}

void gen_code(char *op, char *opr)
{
    fprintf(lexout, "%-16s\t%s\n", op, opr);
}

void convert(FILE *in, FILE *out)
{
    yyin = in;
    lexout = out;
    setup_keywords();
    yyparse();
}

