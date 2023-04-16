#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cipher.h"
#include "tests.h"

#define READING_SIZE 10
#define MIN_ARGC 2
#define MAX_ARGC 5

#define NUM_OF_ARGS_ERR "The program receives 1 or 4 arguments only.\n"
#define ARG_SHOULD_BE_TEST_ERR "Usage: cipher test\n"
#define COMMAND_ARG_ERR "The given command is invalid.\n"
#define K_ARG_ERR "The given shift value is invalid.\n"
#define FILE_ARG_ERR "The given file is invalid.\n"

enum AsciiNums
{
    MIN_NUM = 48, // 0 in Ascii
    MAX_NUM = 57, // 9 in Ascii
    MINUS = 45 // '-'
 };

///this func checks if argc is between 1 to 4 (without the path to program)
/// return 0 in success
int argc_validation(int argc)
{
    if (argc != MAX_ARGC && argc != MIN_ARGC)
    {
        fprintf(stderr, NUM_OF_ARGS_ERR);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/// this function run all tests.
/// return 0 in success
int tests(char command[])
{
    if(strcmp(command, "test") != 0)
    {
        fprintf(stderr, ARG_SHOULD_BE_TEST_ERR);
        return EXIT_FAILURE;
    }
    _Bool test1 = test_encode_non_cyclic_lower_case_positive_k() == 0;

    _Bool test2 = \
                test_encode_cyclic_lower_case_special_char_positive_k() == 0;

    _Bool test3 = \
            test_encode_non_cyclic_lower_case_special_char_negative_k() == 0;

    _Bool test4 = test_encode_cyclic_lower_case_negative_k() == 0;
    _Bool test5 = test_encode_cyclic_upper_case_positive_k() == 0;

    if (!(test1 && test2 && test3 && test4 && test5))
    {
        return EXIT_FAILURE;
    }
    _Bool test6 = test_decode_non_cyclic_lower_case_positive_k() == 0;

    _Bool test7 = \
                test_decode_cyclic_lower_case_special_char_positive_k() == 0;

    _Bool test8 = \
        test_decode_non_cyclic_lower_case_special_char_negative_k() == 0;

    _Bool test9 = test_decode_cyclic_lower_case_negative_k() == 0;
    _Bool test10 = test_decode_cyclic_upper_case_positive_k() == 0;

    if (!(test6 && test7 && test8 && test9 && test10))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int check_command_arg(const char* command)
{
    if ((strcmp(command, "decode") != 0) && (strcmp(command, "encode") != 0))
    {
        fprintf(stderr, COMMAND_ARG_ERR);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int check_k_arg(const char* k){
    for(int i=0; i < (int) strlen(k); i++)
    {
        char c = k[i];
        _Bool is_not_num = (c < MIN_NUM || c > MAX_NUM);
        if (is_not_num)
        {
            if (i == 0 && c == MINUS)
            {
                continue;
            }
            fprintf(stderr, K_ARG_ERR);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void run_cipher(const char* command, long k, FILE* in, FILE* out)
{
    char line[READING_SIZE];
    while (fgets(line, READING_SIZE, in) != NULL)
    {
        if (strcmp(command, "encode") == 0) {
            encode(line, k);
        }
        else
        {
            decode(line, k);
        }
        fprintf(out, "%s", line);
    }
    fclose(in);
    fclose(out);
}



int main (int argc, char *argv[])
{
    if (argc_validation(argc))
    {
        return EXIT_FAILURE;
    }
    if(argc == MIN_ARGC) // case: 1 arg
    {
        if (tests(argv[1]))
        {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if(check_command_arg(argv[1]))
    {
        return EXIT_FAILURE;
    }
    if(check_k_arg(argv[2]))
    {
        return EXIT_FAILURE;
    }
    // checking given files here, because
    // we didn't learn how to return files from functions.
    FILE* in_file = fopen(argv[3],"r");
    if (in_file == NULL)                             // checks arg: input file
    {
        fprintf(stderr, FILE_ARG_ERR);
        return EXIT_FAILURE;
    }
    FILE* out_file = fopen(argv[4],"w");
    if (out_file == NULL)                           // checks arg: output file
    {
        fclose(in_file);
        fprintf(stderr, FILE_ARG_ERR);
        return EXIT_FAILURE;
    }
    // If we here, all 4 args is valid.
    long k = strtol(argv[2], NULL, READING_SIZE);
    run_cipher(argv[1], k, in_file, out_file);
    return EXIT_SUCCESS;
}
