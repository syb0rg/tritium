#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "tritium.h"

static void compregex_usage()
{
    printf("compregex: compile regexes into C structures\n");
    printf("usage: compregex name regex\n"
           "  Compiles regex into a C structure called name\n");
    exit(0);
}

static void regex_to_C(const char *name, const cst_regex *rgx)
{
    int i;

    printf("static const unsigned char %s_rxprog[] = {\n   ",name);
    for (i=0; i<rgx->regsize; i++)
    {
        printf("%d, ", (unsigned char)rgx->program[i]);
        if (i%16 == 15)
            printf("\n   ");
    }
    printf("\n};\n");
    printf("static const cst_regex %s_rx = {\n   ",name);
    printf("%d, ",rgx->regstart);
    printf("%d, ",rgx->reganch);
    if (rgx->regmust == NULL)
        printf("NULL, ");
    else
        printf("%s_rxprog + %d, ", name, rgx->regmust - rgx->program);
    printf("%d, ",rgx->regmlen);
    printf("%d,\n   ",rgx->regsize);
    printf("(char *)%s_rxprog\n",name);
    printf("};\n");

    printf("const cst_regex * const %s = &%s_rx;\n\n",name, name);
}

int main(int argc, char **argv)
{
    cst_regex *rgx;

    if (argc != 3)
        compregex_usage();

    rgx = new_cst_regex(argv[2]);
    regex_to_C(argv[1],rgx);
    delete_cst_regex(rgx);
    
    return 0;
}
