#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "markov_chain.h"
#define MAX_WORD_SIZE 100
#define MAX_LINE_SIZE 1000
#define READ_ALL_FILE (-100)
#define FILE_ERR "Error: Failed to open file, make sure you use absolute path."
#define DECIMAL 10
#define MAX_TWEET_LEN 20
#define TWEET_COUNT_FORMAT "Tweet %i:"
#define USAGE_ERR "Usage: The program require the following parameters\n\
1) Seed number.\n\
2) Number of lines to generate. \n\
3) Absolute path to file. \n\
4) Optional - Number of lines to read from file. If no value is given" \
" read the entire file.\n"



void fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
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
            for( ;i< strlen(buffer_line) && buffer_line[i] == ' '; i++){}
            if (strlen(buffer_word) > 0)
            {
                Node *word_node = add_to_database(markov_chain, buffer_word);
                add_node_to_counter_list(prev_markov, word_node->data);
                prev_markov = word_node->data;
            }
        }
    }
}

int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        printf("%s", USAGE_ERR);
        return EXIT_FAILURE;
    }
    int seed = strtol(argv[1], NULL, DECIMAL);
    int num_of_tweets = strtol(argv[2], NULL, DECIMAL);
    char * path_to_corpus = argv[3];
    int to_reads = (argc > 4) ? strtol(argv[4], NULL, DECIMAL) : READ_ALL_FILE;
    srand(seed);
    FILE * file = fopen(path_to_corpus, "r");
    if (file == NULL)
    {
        printf("%s", FILE_ERR);
        return EXIT_FAILURE;
    }
    MarkovChain * markov_chain = malloc(sizeof (*markov_chain));
    LinkedList *linked_list = malloc(sizeof (*linked_list));
    linked_list->first= NULL;
    linked_list->last= NULL;
    linked_list->size= 0;
    markov_chain->database = linked_list;
    fill_database(file, to_reads,markov_chain);
    for (int k = 1; k <= num_of_tweets; k++)
    {
        MarkovNode* first_rand = get_first_random_node(markov_chain);
        if(first_rand == NULL)
        {
            free_markov_chain(&markov_chain);
            return EXIT_FAILURE;
        }
        printf(TWEET_COUNT_FORMAT, k);
        generate_random_sequence(markov_chain, first_rand, MAX_TWEET_LEN);
        printf("\n");
    }
    free_markov_chain(&markov_chain);
    fclose(file);
}
