#include <stdio.h>
#include <string.h>
#include "sc.h"

void parse_expression();

void parse_factor()
{
    char id[BUFSIZ];
    int args;
    int i;
    if (token == TOKEN_ID)
    {
        strcpy(id, lexeme);
        get_token();
        if (token == TOKEN_LPAR)
        {
            args = 0;
            get_token();
            if (token != TOKEN_RPAR)
            {
                parse_expression();
                gen_code("store_arg", "-");
                ++args;
                while (token == TOKEN_COMMA)
                {
                    gen_code("store_arg", "-");
                    ++args;
                    parse_expression();
                }
            }

            if (token == TOKEN_RPAR)
            {
                gen_code("load_num", "0");
                gen_code("store_arg", "-");
                gen_code("call", id);
                gen_code("load_arg", "-");
                for (i = 0; i < args; ++i)
                    gen_code("cancel_arg", "-");
                get_token();
            }
            else
            {
                puts("2");
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
        else if (token == TOKEN_LBRA)
        {
            get_token();
            gen_code("load_adr", id);
            parse_expression();
            gen_code("add_index", id);
            gen_code("load_ind", id);
            if (token == TOKEN_RBRA)
            {
                get_token();
            }
            else
            {
                puts("1");
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
        else
        {
            gen_code("load_id", id);
        }
    }
    else if (token == TOKEN_NUM)
    {
        gen_code("load_num", lexvalue);
        get_token();
    }
    else if (token == TOKEN_LPAR)
    {
        get_token();
        parse_expression();
        if (token == TOKEN_RPAR)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_NOT)
    {
        get_token();
        parse_factor();
        gen_code("not", "-");
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
}

void parse_term()
{
    parse_factor();
    while (1)
    {
        if (token == TOKEN_ASTER)
        {
            get_token();
            parse_factor();
            gen_code("mul", "-");
        }
        else if (token == TOKEN_SLASH)
        {
            get_token();
            parse_factor();
            gen_code("div", "-");
        }
        else if (token == TOKEN_AND)
        {
            get_token();
            parse_factor();
            gen_code("and", "-");
        }
        else if (token == TOKEN_PERCENT)
        {
            get_token();
            parse_factor();
            gen_code("mod", "-");
        }
        else if (token == TOKEN_SHIFTL)
        {
            get_token();
            parse_factor();
            gen_code("shiftl", "-");
        }
        else if (token == TOKEN_SHIFTR)
        {
            get_token();
            parse_factor();
            gen_code("shiftr", "-");
        }
        else
        {
            break;
        }
    }
}

void parse_polynomial()
{
    if (token == TOKEN_PLUS)
    {
        get_token();
        parse_term();
    }
    else if (token == TOKEN_MINUS)
    {
        get_token();
        parse_term();
        gen_code("neg", "-");
    }
    else
    {
        parse_term();
    }

    while (1)
    {
        if (token == TOKEN_PLUS)
        {
            get_token();
            parse_term();
            gen_code("add", "-");
        }
        else if (token == TOKEN_MINUS)
        {
            get_token();
            parse_term();
            gen_code("sub", "-");
        }
        else if (token == TOKEN_OR)
        {
            get_token();
            parse_term();
            gen_code("or", "-");
        }
        else
        {
            break;
        }
    }
}

void parse_expression()
{
    parse_polynomial();
    if (token == TOKEN_EQ)
    {
        get_token();
        parse_polynomial();
        gen_code("eq", "-");
    }
    else if (token == TOKEN_NE)
    {
        get_token();
        parse_polynomial();
        gen_code("ne", "-");
    }
    else if (token == TOKEN_LT)
    {
        get_token();
        parse_polynomial();
        gen_code("lt", "-");
    }
    else if (token == TOKEN_LE)
    {
        get_token();
        parse_polynomial();
        gen_code("le", "-");
    }
    else if (token == TOKEN_GT)
    {
        get_token();
        parse_polynomial();
        gen_code("gt", "-");
    }
    else if (token == TOKEN_GE)
    {
        get_token();
        parse_polynomial();
        gen_code("ge", "-");
    }
}

void parse_statement()
{
    char id[BUFSIZ];
    char label1[BUFSIZ];
    char label2[BUFSIZ];
    int args;
    int i;
    if (token == TOKEN_ID)
    {
        strcpy(id, lexeme);
        get_token();
        if (token == TOKEN_COLEQ)
        {
            get_token();
            parse_expression();
            gen_code("store_id", id);
        }
        else if (token == TOKEN_COL)
        {
            gen_code("label", id);
            get_token();
            parse_statement();
        }
        else if (token == TOKEN_LPAR)
        {
            args = 0;
            get_token();
            if (token != TOKEN_RPAR)
            {
                parse_expression();
                gen_code("store_arg", "-");
                ++args;
                while (token == TOKEN_COMMA)
                {
                    get_token();
                    parse_expression();
                    gen_code("store_arg", "-");
                    ++args;
                }
            }

            if (token == TOKEN_RPAR)
            {
                gen_code("load_num", "0");
                gen_code("store_arg", "-");
                gen_code("call", id);
                gen_code("cancel_arg", "-");
                for (i = 0; i < args; ++i)
                    gen_code("cancel_arg", "-");
                get_token();
            }
            else
            {
                puts("a");
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
        else if (token == TOKEN_LBRA)
        {
            gen_code("load_adr", id);
            get_token();
            parse_expression();
            gen_code("add_index", id);
            if (token == TOKEN_RBRA)
            {
                get_token();
                if (token == TOKEN_COLEQ)
                {
                    get_token();
                    parse_expression();
                    gen_code("store_ind", id);
                }
            }
            else
            {
                puts("4");
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
        else
        {
                puts("0");
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_IF)
    {
        get_token();
        parse_expression();
        gen_code("tst", "-");
        strcpy(label1, new_label());
        gen_code("jeq", label1);
        if (token == TOKEN_THEN)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_statement();
        if (token == TOKEN_ELSE)
        {
            strcpy(label2, new_label());
            gen_code("jmp", label2);
            gen_code("label", label1);
            get_token();
            parse_statement();
            gen_code("label", label2);
        }
        else
        {
            gen_code("label", label1);
        }
    }
    else if (token == TOKEN_WHILE)
    {
        get_token();
        strcpy(label1, new_label());
        gen_code("label", label1);
        parse_expression();
        gen_code("tst", "-");
        strcpy(label2, new_label());
        gen_code("jeq", label2);
        if (token == TOKEN_DO)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_statement();
        gen_code("jmp", label1);
        gen_code("label", label2);
    }
    else if (token == TOKEN_REPEAT)
    {
        get_token();
        strcpy(label1, new_label());
        gen_code("label", label1);
        parse_statement();
        if (token == TOKEN_UNTIL)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        parse_expression();
        gen_code("tst", "-");
        gen_code("jeq", label1);
    }
    else if (token == TOKEN_GOTO)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("jmp", lexeme);
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_BEGIN)
    {
        get_token();
        parse_statement();
        while (token == TOKEN_SEMICOL)
        {
            get_token();
            parse_statement();
        }
        if (token == TOKEN_END)
        {
            get_token();
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_RETURN)
    {
        get_token();
        parse_expression();
        gen_code("store_id", "ans");
        gen_code("return", "-");
    }
    else
    {
        ;
    }
}

void parse_variable()
{
    if (token == TOKEN_LONG)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_long", lexeme);
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_WORD)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_word", lexeme);
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else if (token == TOKEN_BYTE)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("var_byte", lexeme);
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    get_token();
    if (token == TOKEN_LBRA)
    {
        get_token();
        if (token == TOKEN_NUM)
        {
            gen_code("set_size", lexvalue);
            get_token();
            if (token == TOKEN_RBRA)
            {
                get_token();
            }
            else
            {
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
    }
}

void parse_function()
{
    if (token == TOKEN_FUNCTION)
    {
        get_token();
        if (token == TOKEN_ID)
        {
            gen_code("start_func", lexeme);
            get_token();
            if (token == TOKEN_LPAR)
            {
                get_token();
                if (token != TOKEN_RPAR)
                {
                    parse_variable();
                    while (token == TOKEN_COMMA)
                    {
                        get_token();
                        parse_variable();
                    }
                }

                if (token == TOKEN_RPAR)
                {
                    gen_code("start_prologue", "-");
                    get_token();
                }
                else
                {
                    error(ERROR_SYNTAX, lexeme, lineno);
                }

                while (token == TOKEN_LONG || token == TOKEN_WORD || token == TOKEN_BYTE)
                {
                    parse_variable();
                    if (token != TOKEN_SEMICOL)
                    {
                        error(ERROR_SYNTAX, lexeme, lineno);
                    }
                    get_token();
                }
                gen_code("end_prologue", "-");
                parse_statement();
                gen_code("end_func", "-");
            }
            else
            {
                error(ERROR_SYNTAX, lexeme, lineno);
            }
        }
        else
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
    }
    else
    {
        error(ERROR_SYNTAX, lexeme, lineno);
    }
}

void parse_program()
{
    gen_code("start", "main");
    while (token == TOKEN_LONG || token == TOKEN_WORD || token == TOKEN_BYTE || token == TOKEN_FUNCTION)
    {
        if (token == TOKEN_FUNCTION)
        {
            parse_function();
            if (token != TOKEN_SEMICOL)
            {
                error(ERROR_SYNTAX, lexeme, lineno);
            }
            get_token();
            continue;
        }

        parse_variable();
        if (token == TOKEN_COL)
        {
            get_token();
            if (token == TOKEN_NUM)
            {
                gen_code("set_address", lexvalue);
            }
            else
            {
                error(ERROR_SYNTAX, lexeme, lineno);
            }
            get_token();
        }
        if (token != TOKEN_SEMICOL)
        {
            error(ERROR_SYNTAX, lexeme, lineno);
        }
        get_token();
    }
    gen_code("label", "main");
    parse_statement();
    gen_code("end", "-");
}

void parse()
{
    parse_program();
}
