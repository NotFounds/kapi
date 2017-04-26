#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sc.h"

FILE *lexin;
FILE *lexout;
char c;

void get_char()
{
    c = fgetc(lexin);
    return;
}


int end_of_lexeme;
char lexeme[BUFSIZ];

void save_char(char c)
{
    lexeme[end_of_lexeme++] = c;
    lexeme[end_of_lexeme] = '\0';
}

int token;

void get_token()
{
    int i;

state0:
    end_of_lexeme = 0;
    if (char_pos(WHITESPACE, c) >= 0)
    {
        get_char();
        goto state0;
    }
    else if (c == '(')
    {
        save_char(c);
        get_char();
        goto state1;
    }
    else if (char_pos(ALPHABET, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state4;
    }
    else if (c == '0')
    {
        save_char(c);
        get_char();
        goto state5;
    }
    else if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    else if (c == '\'')
    {
        save_char(c);
        get_char();
        goto state9;
    }
    else if (c == ':')
    {
        save_char(c);
        get_char();
        goto state12;
    }
    else if (c == '<')
    {
        save_char(c);
        get_char();
        goto state13;
    }
    else if (c == '>')
    {
        save_char(c);
        get_char();
        goto state14;
    }
    else if (c == '=')
    {
        save_char(c);
        get_char();
        goto state15;
    }
    else if (c == ')')
    {
        save_char(c);
        get_char();
        token = TOKEN_RPAR;
        goto final;
    }
    else if (c == '+')
    {
        save_char(c);
        get_char();
        token = TOKEN_PLUS;
        goto final;
    }
    else if (c == '-')
    {
        save_char(c);
        get_char();
        token = TOKEN_MINUS;
        goto final;
    }
    else if (c == '*')
    {
        save_char(c);
        get_char();
        token = TOKEN_ASTER;
        goto final;
    }
    else if (c == '/')
    {
        save_char(c);
        get_char();
        token = TOKEN_SLASH;
        goto final;
    }
    else if (c == ';')
    {
        save_char(c);
        get_char();
        token = TOKEN_SEMICOL;
        goto final;
    }
    else if (c == EOF)
    {
        save_char(c);
        token = TOKEN_EOF;
        goto final;
    }
    else
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
state1:
    if (c == '*')
    {
        get_char();
        goto state2;
    }
    else
    {
        token = TOKEN_LPAR;
        goto final;
    }
state2:
    if (c == '*')
    {
        get_char();
        goto state3;
    }
    else if (c == EOF)
    {
        goto state0;
    }
    else
    {
        get_char();
        goto state2;
    }
state3:
    if (c == '*')
    {
        get_char();
        goto state3;
    }
    else if (c == ')')
    {
        get_char();
        goto state0;
    }
    else if (c == EOF)
    {
        goto state0;
    }
    else
    {
        get_char();
        goto state2;
    }
state4:
    if (char_pos(ALPHABET, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state4;
    }
    else if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state4;
    }
    else
    {
        if ((i = find_keywords(lexeme)) >= 0)
        {
            token = keywords[i].value;
        }
        else
        {
            token = TOKEN_ID;
        }
        goto final;
    }
state5:
    if (c == 'x')
    {
        save_char(c);
        get_char();
        goto state6;
    }
    else if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    else
    {
        token = TOKEN_NUM;
        goto final;
    }
state6:
    if (char_pos(HEXDIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state7;
    }
    else
    {
        token = TOKEN_ERROR;
        goto final;
    }
state7:
    if (char_pos(HEXDIGIT, tolower(c)) >= 0)
    {
        save_char(c);
        get_char();
        goto state7;
    }
    else
    {
        token = TOKEN_NUM;
        goto final;
    }
state8:
    if (char_pos(DIGIT, c) >= 0)
    {
        save_char(c);
        get_char();
        goto state8;
    }
    else
    {
        token = TOKEN_NUM;
        goto final;
    }
state9:
    if (c == '\\')
    {
        save_char(c);
        get_char();
        goto state10;
    }
    else if (c == '\'' || c == '\"')
    {
        save_char(c);
        get_char();
        token = TOKEN_ERROR;
        goto final;
    }
    else
    {
        save_char(c);
        get_char();
        goto state11;
    }
state10:
    if (c == 'n')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else if (c == 't')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else if (c == '0')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else if (c == '\'')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else if (c == '\"')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else if (c == '\\')
    {
        save_char(c);
        get_char();
        goto state11;
    }
    else
    {
        token = TOKEN_ERROR;
        goto final;
    }
state11:
    if (c == '\'')
    {
        save_char(c);
        get_char();
        token = TOKEN_NUM;
        goto final;
    }
    else
    {
        token = TOKEN_ERROR;
        goto final;
    }
state12:
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_COLEQ;
        goto final;
    }
    else
    {
        token = TOKEN_COL;
        goto final;
    }
state13:
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_LE;
        goto final;
    }
    else if (c == '>')
    {
        save_char(c);
        get_char();
        token = TOKEN_NE;
        goto final;
    }
    else
    {
        token = TOKEN_LT;
        goto final;
    }
state14:
    if (c == '=')
    {
        save_char(c);
        get_char();
        token = TOKEN_GE;
        goto final;
    }
    else if (c == '<')
    {
        save_char(c);
        get_char();
        token = TOKEN_NE;
        goto final;
    }
    else
    {
        token = TOKEN_GT;
        goto final;
    }
state15:
    if (c == '>')
    {
        save_char(c);
        get_char();
        token = TOKEN_GE;
        goto final;
    }
    else if (c == '<')
    {
        save_char(c);
        get_char();
        token = TOKEN_LE;
        goto final;
    }
    else
    {
        token = TOKEN_EQ;
        goto final;
    }
final:
    return;
}

void print_token()
{
    if (token == TOKEN_EOF) {
        fprintf(lexout, "EOF\n");
    } else if (token == TOKEN_ERROR) {
        fprintf(lexout, "ERROR\t[%s]\n", lexeme);
    } else if (token == TOKEN_ID) {
        fprintf(lexout, "ID\t[%s]\n", lexeme);
    } else if (token == TOKEN_NUM) {
        fprintf(lexout, "NUM\t[%s]\n", lexeme);
    } else if (token == TOKEN_COL) {
        fprintf(lexout, "COL\t[%s]\n", lexeme);
    } else if (token == TOKEN_SEMICOL) {
        fprintf(lexout, "SEMICOL\t[%s]\n", lexeme);
    } else if (token == TOKEN_COLEQ) {
        fprintf(lexout, "COLEQ\t[%s]\n", lexeme);
    } else if (token == TOKEN_EQ) {
        fprintf(lexout, "EQ\t[%s]\n", lexeme);
    } else if (token == TOKEN_NE) {
        fprintf(lexout, "NE\t[%s]\n", lexeme);
    } else if (token == TOKEN_LT) {
        fprintf(lexout, "LT\t[%s]\n", lexeme);
    } else if (token == TOKEN_LE) {
        fprintf(lexout, "LE\t[%s]\n", lexeme);
    } else if (token == TOKEN_GT) {
        fprintf(lexout, "GT\t[%s]\n", lexeme);
    } else if (token == TOKEN_GE) {
        fprintf(lexout, "GE\t[%s]\n", lexeme);
    } else if (token == TOKEN_LPAR) {
        fprintf(lexout, "LPAR\t[%s]\n", lexeme);
    } else if (token == TOKEN_RPAR) {
        fprintf(lexout, "RPAR\t[%s]\n", lexeme);
    } else if (token == TOKEN_PLUS) {
        fprintf(lexout, "PLUS\t[%s]\n", lexeme);
    } else if (token == TOKEN_MINUS) {
        fprintf(lexout, "MINUS\t[%s]\n", lexeme);
    } else if (token == TOKEN_ASTER) {
        fprintf(lexout, "ASTER\t[%s]\n", lexeme);
    } else if (token == TOKEN_SLASH) {
        fprintf(lexout, "SLASH\t[%s]\n", lexeme);
    } else if (token == TOKEN_IF) {
        fprintf(lexout, "IF\t[%s]\n", lexeme);
    } else if (token == TOKEN_THEN) {
        fprintf(lexout, "THEN\t[%s]\n", lexeme);
    } else if (token == TOKEN_ELSE) {
        fprintf(lexout, "ELSE\t[%s]\n", lexeme);
    } else if (token == TOKEN_WHILE) {
        fprintf(lexout, "WHILE\t[%s]\n", lexeme);
    } else if (token == TOKEN_DO) {
        fprintf(lexout, "DO\t[%s]\n", lexeme);
    } else if (token == TOKEN_REPEAT) {
        fprintf(lexout, "REPEAT\t[%s]\n", lexeme);
    } else if (token == TOKEN_UNTIL) {
        fprintf(lexout, "UNTIL\t[%s]\n", lexeme);
    } else if (token == TOKEN_GOTO) {
        fprintf(lexout, "GOTO\t[%s]\n", lexeme);
    } else if (token == TOKEN_BEGIN) {
        fprintf(lexout, "BEGIN\t[%s]\n", lexeme);
    } else if (token == TOKEN_END) {
        fprintf(lexout, "END\t[%s]\n", lexeme);
    } else if (token == TOKEN_LONG) {
        fprintf(lexout, "LONG\t[%s]\n", lexeme);
    } else if (token == TOKEN_WORD) {
        fprintf(lexout, "WORD\t[%s]\n", lexeme);
    } else if (token == TOKEN_BYTE) {
        fprintf(lexout, "BYTE\t[%s]\n", lexeme);
    } else if (token == TOKEN_AND) {
        fprintf(lexout, "AND\t[%s]\n", lexeme);
    } else if (token == TOKEN_OR) {
        fprintf(lexout, "OR\t[%s]\n", lexeme);
    } else if (token == TOKEN_NOT) {
        fprintf(lexout, "NOT\t[%s]\n", lexeme);
    } else {
        fprintf(lexout, "print_token: Unknown token.\n");
    }
}

void convert(FILE *in, FILE *out)
{
    lexin = in;
    lexout = out;
    setup_keywords();
    get_char();
    get_token();
    while (token != TOKEN_EOF) {
        print_token();
        get_token();
    }
}
