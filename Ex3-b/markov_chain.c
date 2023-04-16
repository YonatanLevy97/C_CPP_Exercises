#include "markov_chain.h"
#include <string.h>

#define NONE_INDEX -1

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}


void free_markov_chain(MarkovChain ** ptr_chain)
{
    if (ptr_chain == NULL)
    {
        return;
    }
    if ((*ptr_chain)->database == NULL)
    {
        free(*ptr_chain);
        *ptr_chain = NULL;
        return;
    }
    Node * nodes_traveler = (*ptr_chain)->database->first;
    while (nodes_traveler != NULL)
    {
        Node * next = nodes_traveler->next;
        if(nodes_traveler->data != NULL)
        {
            if (nodes_traveler->data->counter_list != NULL)
            {
                free(nodes_traveler->data->counter_list);
            }
            if(nodes_traveler->data->data != NULL)
            {
                (*ptr_chain)->free_data(nodes_traveler->data->data);
            }
            free(nodes_traveler->data);
        }
        free(nodes_traveler);
        nodes_traveler = next;
    }
    free((*ptr_chain)->database);
    free((*ptr_chain));
    *ptr_chain = NULL;
}


MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    if (markov_chain == NULL || markov_chain->database == NULL ||
    markov_chain->database->first == NULL)
    {
        return NULL;
    }
    Node * traveler;
    do
    {
        int rand_num = get_random_number(markov_chain->database->size);
        traveler = markov_chain->database->first;
        for(int i = 0; i < rand_num && traveler != NULL; i++)
        {
            traveler = traveler->next;
        }
    }
    while (traveler != NULL && traveler->data != NULL &&
    markov_chain->is_last(traveler->data->data));

    return traveler->data;
}


MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
    if(state_struct_ptr == NULL || state_struct_ptr->counter_list == NULL)
    {
        return NULL;
    }
    int max_size = state_struct_ptr->size;
    NextNodeCounter * counter_list_p = state_struct_ptr->counter_list;
    //~~~~~~~~~~~~~~~~~~~~~total appearance~~~~~~~~~~~~
    int all_freq = 0;
    for(int j=0; j < max_size; j++)
    {
        all_freq += counter_list_p[j].frequency;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int rand_num = get_random_number(all_freq);
    NextNodeCounter * traveler;
    int i = 0;
    while (rand_num >= 0)
    {
        traveler = &state_struct_ptr->counter_list[i++];
        rand_num -= traveler->frequency;
    }
    return traveler->markov_node;
}

Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
    if (markov_chain == NULL || markov_chain->database == NULL
    || markov_chain->database->first == NULL)
    {
        return NULL;
    }
    Node * traveler = markov_chain->database->first;
    for(int i=0; i < markov_chain->database->size; i++)
    {
        if (traveler == NULL || traveler->data == NULL ||
        traveler->data->data == NULL)
        {
            continue;
        }
        if (markov_chain->comp_func(traveler->data->data, data_ptr) == 0)
        {
            return traveler;
        }
        traveler = traveler->next;
    }
    return traveler;
}

bool malloc_or_realloc_counter_list(MarkovNode *markov_node)
{
    if (markov_node == NULL || markov_node->data == NULL)
    {
        return false;
    }
    NextNodeCounter * tmp_counter_list;
    tmp_counter_list = realloc(markov_node->counter_list,
                       sizeof(NextNodeCounter) * (markov_node->size +1));
    if (tmp_counter_list == NULL)
    {
        return false;
    }
    markov_node->counter_list = tmp_counter_list;
    return true;
}


int is_node_in_counter_list(MarkovNode *first_node, MarkovNode *second_node,
                            MarkovChain *markov_chain)
{
    if (first_node == NULL || first_node->counter_list == NULL)
    {
        return NONE_INDEX;
    }
    for (int i = 0; i < first_node->size; i++)
    {
        if (&first_node->counter_list[i] == NULL ||
                        first_node->counter_list[i].markov_node == NULL)
        {
            return NONE_INDEX;
        }
        char* first_node_data = first_node->counter_list[i].markov_node->data;
        if(markov_chain->comp_func(first_node_data, second_node->data) == 0)
        {
            return i;
        }
    }
    return NONE_INDEX;
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node,
                              MarkovChain* markovChain)
{
    if(first_node == NULL)
    {
        return false;
    }
    int index = is_node_in_counter_list(first_node,second_node, markovChain);
    if (index != NONE_INDEX && first_node->counter_list != NULL)
    {
        first_node->counter_list[index].frequency++;
        return true;
    }
    else if (malloc_or_realloc_counter_list(first_node))
    {
        first_node->size++;
        first_node->counter_list[(first_node->size)-1] =
                        (NextNodeCounter){second_node,1};
        return true;
    }
    return false;
}

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, void* data_ptr)
{
    Node * node_from_database = get_node_from_database(markov_chain, data_ptr);
    if (node_from_database != NULL)
    {
        return node_from_database;
    }
    else
    {
        MarkovNode* markov_node_word = malloc(sizeof (MarkovNode));
        if (markov_node_word == NULL)
        {
            return NULL;
        }

        void* tmp = markov_chain->copy_func(data_ptr);
        if(tmp == NULL)
        {
            return NULL;
        }
        *markov_node_word = ((MarkovNode) {tmp, NULL, 0});
        int add_result = add(markov_chain->database, markov_node_word);
        Node* node_res = get_node_from_database(markov_chain, data_ptr);
        return add_result == EXIT_SUCCESS ? node_res : NULL;
    }
}


void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
                                                first_node, int max_length)
{
    if (markov_chain == NULL)
    {
        return;
    }
    markov_chain->print_func(first_node->data);
    MarkovNode* next_word = get_next_random_node(first_node);
    int tweets_num = 2;
    markov_chain->print_func(next_word->data);
    while(tweets_num < max_length && (!markov_chain->is_last(next_word->data)))
    {
        next_word = get_next_random_node(next_word);
        if (next_word == NULL)
        {
            return;
        }
        markov_chain->print_func(next_word->data);
        tweets_num++;
    }
}
