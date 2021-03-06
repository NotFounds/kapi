#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sc.h"

static int error_count = 0;

void error(int type, char *hint, int lineno)
{
    ++error_count;
    if (error_count > MAX_ERRORCOUNT)
    {
        exit(1);
    }

    if (type == ERROR_OVERFLOW)
    {
        fprintf(stderr, "[%s] overflowed.\n", hint);
    }
    else if (type == ERROR_SYNTAX)
    {
        fprintf(stderr, "syntax error at [%d] lexeme[%s].\n", lineno, hint);
    }
    else if (type == ERROR_UNDEFINED)
    {
        fprintf(stderr, "symbol[%s] undefined.\n", hint);
    }
    else if (type == ERROR_DOUBLE)
    {
        fprintf(stderr, "symbol[%s] double defined.\n", hint);
    }
    else if (type == ERROR_TYPE)
    {
        fprintf(stderr, "symbol[%s] type mismatch.\n", hint);
    }
    else if (type == ERROR_INTERNAL)
    {
        fprintf(stderr, "internal error at [%s].\n", hint);
    }
    else
    {
        fprintf(stderr, "error: Unknown error.\n");
    }
}

int char_pos(char *s, char c)
{
    int i;

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == c)
        {
            return i;
        }
    }
    return -1;
}

