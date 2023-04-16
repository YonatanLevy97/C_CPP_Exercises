#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "markov_chain.h"

#define LAST_INDEX(s) (strlen(s) - 1)
#define MAX_WORD_SIZE 100
#define MAX_LINE_SIZE 1000
#define READ_ALL_FILE -100
#define FILE_ERR "Error: Failed to open file, make sure you use absolute path."
#define DECIMAL 10
#define MAX_TWEET_LEN 20
#define TWEET_COUNT_FORMAT "Tweet %li:"
#define USAGE_ERR "Usage: The program require the following parameters\n\
1) Seed number.\n\
2) Number of lines to generate. \n\
3) Absolute path to file. \n\
4) Optional - Number of lines to read from file. If no value is given" \
" read the entire file.\n"

static int str_cmp(void* cmp1, void* cmp2)
{
    return strcmp((char *) cmp1, (char *) cmp2);
}

static void print_func(void* to_print) { printf(" %s",(char *) to_print); }

static void free_data(void* to_free) { free(to_free); }

static bool is_last(void* to_check_if_last)
{
    char * str_data = (char *) (to_check_if_last);
    return strcmp((char *) (str_data + LAST_INDEX(str_data)), ".") == 0;
}

static void* copy_func(void* to_copy)
{
    char * to_copy_str = (char *) to_copy;
    char * the_copied_str = malloc(strlen(to_copy_str)+1);
    return strcpy(the_copied_str,to_copy_str);
}

static void* safe_calloc_tw(size_t size, FILE* to_close, void* to_free)
{
    void* to_alloc = calloc(1, size);
    if (to_alloc == NULL)
    {
        if (to_close != NULL) { fclose(to_close);}
        if (to_free != NULL) { free(to_free);}
        printf(ALLOCATION_ERROR_MASSAGE);
    }
    return to_alloc;
}

static int fill_database (FILE *fp,int words_to_read,MarkovChain* markov_chain)
{
    char buffer_line[MAX_LINE_SIZE], buffer_word[MAX_WORD_SIZE];
    int reads_words = 0;
    MarkovNode * prev_markov = NULL;
    while(fgets(buffer_line, MAX_LINE_SIZE, fp) &&
    ((reads_words < words_to_read) || (words_to_read == READ_ALL_FILE)))
    {
        for (size_t i = 0; (i < strlen(buffer_line)) &&
        ((reads_words < words_to_read) || (words_to_read == READ_ALL_FILE));
        reads_words++)
        {
            sscanf(buffer_line + i, "%s ", buffer_word);
            for( ; i< strlen(buffer_line) && buffer_line[i] != ' '; i++){}
            for( ; i< strlen(buffer_line) &&  buffer_line[i] == ' '; i++){}
            if (strlen(buffer_word) > 0)
            {
                Node *word_node = add_to_database(markov_chain, buffer_word);
                bool is_added = add_node_to_counter_list(prev_markov,
                                         word_node->data, markov_chain);
                if (word_node == NULL || (reads_words != 0 && !is_added))
                {
                    return EXIT_FAILURE;
                }
                prev_markov = word_node->data;
            }
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        printf("%s", USAGE_ERR);
        return EXIT_FAILURE;
    }
    int seed = strtol(argv[1], NULL, DECIMAL);
    long num_of_tweets = strtol(argv[2], NULL, DECIMAL);
    char * path_to_corpus = argv[3];
    int to_reads = (argc > 4) ? strtol(argv[4], NULL, DECIMAL) : READ_ALL_FILE;
    srand(seed);
    FILE * file = fopen(path_to_corpus, "r");
    if (file == NULL)
    {
        printf(FILE_ERR);
        return EXIT_FAILURE;
    }
    MarkovChain* markov_chain = safe_calloc_tw(sizeof(MarkovChain),file, NULL);
    if(markov_chain == NULL) { return EXIT_FAILURE;}
    LinkedList* database=safe_calloc_tw(sizeof(LinkedList), file,markov_chain);
    if(database == NULL) { return EXIT_FAILURE; }
    *database = (LinkedList) {NULL, NULL, 0};
    *markov_chain = (MarkovChain) {
        database, print_func, str_cmp, free_data, copy_func, is_last};
    if (fill_database(file,  to_reads, markov_chain) == EXIT_FAILURE)
    {
        free_markov_chain(&markov_chain);
        fclose(file);
    }
    for (long k = 1; k <= num_of_tweets; k++)
    {
        MarkovNode* first_rand = get_first_random_node(markov_chain);
        printf(TWEET_COUNT_FORMAT, k);
        generate_random_sequence(markov_chain, first_rand, MAX_TWEET_LEN);
        printf("\n");
    }
    free_markov_chain(&markov_chain);
    fclose(file);
    return EXIT_SUCCESS;
}