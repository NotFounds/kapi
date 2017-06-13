#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "sc.h"

FILE *tmpfile()
{
    FILE *tmp;
    char tmpfile[BUFSIZ];

    strcpy(tmpfile, "/tmp/scXXXXXX");
    mkstemp(tmpfile);
    tmp = fopen(tmpfile, "w+");
    unlink(tmpfile);
    return tmp;
}

int m_option = 0;
int main(int argc, char *argv[])
{
    FILE *in;
    FILE *out;
    FILE *tmp;

    /*                               READ ME

    I think this algorithm that how to resolve the test19a is NOT GENERAL.
    So if you think "I don't want to write like this code :(", I recommend that you rewrite the code!

    Or, Please Change the variable name and(or) code indents.

    And MUST REMOVE this comment!

    */

    /* unistd.h:
     *     int getopt(int argc, char * const argv[], const char *optstring);
     *     extern char *optstring;
     *     extern int optind, opterr, optopt;
     * */

    int opt;
    int idx = 0;
    while ((opt = getopt(argc, argv, "m")) != -1)
    {
        switch (opt)
        {
            case 'm':
                m_option = 1;
                break;
        }
        ++idx;
    }

    if (argc == (3 + idx))
    {
        if ((in = fopen(argv[idx + 1], "r")) == NULL)
        {
            perror("fopen for input");
            exit(1);
        }
        if ((out = fopen(argv[idx + 2], "w")) == NULL)
        {
            perror("fopen for output");
            exit(1);
        }
    }
    else if (argc == (2 + idx))
    {
        if ((in = fopen(argv[idx + 1], "r")) == NULL)
        {
            perror("fopen for input");
            exit(1);
        }
        out = stdout;
    }
    else
    {
        fprintf(stderr, "use: %s input [output]\n", argv[0]);
        exit(1);
    }

    if ((tmp = tmpfile()) == NULL)
    {
        perror("fopen for tmpfile");
        exit(1);
    }

    convert(in, tmp);
    fflush(tmp);
    rewind(tmp);
    convert2(tmp, out);

    fclose(in);
    fclose(out);
    fclose(tmp);
    return 0;
}
