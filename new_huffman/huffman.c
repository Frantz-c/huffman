#include <stdlib.h>
#include "huffman.h"

void     display_help_and_exit(char *name)
{
    ft_printf("%s -[cdv?] -[\"input file\"] [\"output file\"]\n", name);
    exit (-1);
}

t_cli   *get_parameters(int argc, char *argv[])
{
    const char  *expected_args[] = {
        "-c|--compress(bool)",
        "-d|--decompress(bool)",
        "-v|--verbose(bool)",
        "(string)",
        "(string)",
        NULL
    };
    t_cli       *args;
    int         action;

    if (argc < 3)
        display_help_and_exit(argc == 0 ? "huffman" : *argv));
    args = get_args_from_cli(expected_args, argc - 1, argv + 1);
    if (args == NULL)
    {
        cli_print_error();
        exit(-1);
    }
    action = (args[COMPRESS]->value != 0) + (args[DECOMP]->value != 0);
    if (action == 2 || action == 0 || !args[ORG]->value || !args[DEST]->value)
        display_help_and_exit(*argv);
    return (args);
}

int     main(int argc, char *argv[])
{
    t_cli   *args;

    args = get_parameters(argc, argv);
    ((args[COMPRESS]->value) ?
        compression(args[ORG]->value) :
        decompression(args[DEST]->value));
    return (0);
}
