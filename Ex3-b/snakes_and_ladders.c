#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"
#define USAGE_ERR "Usage: The program require the following parameters\n\
1) Seed number.\n\
2) Number of sentences to generate. \n"

#define COUNT_FORMAT "Random Walk %u: "

#define DECIMAL 10

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

static void* safe_calloc_sn(size_t size, void* to_free)
{
    void* to_allocate = calloc(1, size);
    if (to_allocate == NULL)
    {
        if (to_free != NULL) { free(to_free); }
        printf(ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    return to_allocate;
}

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of
                    // the ladder in case there is one from this square
    int snake_to;  // snake_to represents the jump of
                    // the snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

static int cmp_cells(void * first, void * second)
{
    Cell *a = (Cell *) first, *b = (Cell *) second;
    return (a->number - b->number);
}

static void print_cell(void* to_print)
{
    Cell* to_print_cell = (Cell *) to_print;
    printf("[%d]", to_print_cell->number);
    if(to_print_cell->number == BOARD_SIZE){ return; }
    if(to_print_cell->ladder_to != -1)
    {
        printf("-ladder to %d -> ", to_print_cell->ladder_to);
        return;
    }
    if(to_print_cell->snake_to != -1)
    {
        printf("-snake to %d -> ", to_print_cell->snake_to);
        return;
    }
    printf(" -> ");
}

static void free_cell(void* to_free)
{
    Cell * to_free_cell = (Cell *) to_free;
    free(to_free_cell);
}

static void* copy_func(void* to_copy)
{
   Cell* to_copy_cell = (Cell *) to_copy;
   Cell* copied_cell = malloc(sizeof (Cell));
   memcpy(copied_cell, to_copy_cell, sizeof (Cell));
   return copied_cell;
}

static bool is_last(void* to_check_last)
{
    Cell* check_cell = (Cell *) to_check_last;
    return check_cell->number == BOARD_SIZE;
}


/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("%s", USAGE_ERR);
        return EXIT_FAILURE;
    }
    srand(strtol(argv[1], NULL, DECIMAL));
    unsigned int num_of_sen = strtol(argv[2],NULL,DECIMAL);
    LinkedList* database = safe_calloc_sn(sizeof (LinkedList), NULL);
    if(database == NULL) { return EXIT_FAILURE; }
    MarkovChain* markov_chain = safe_calloc_sn(sizeof (MarkovChain), database);
    if(markov_chain == NULL) { return EXIT_FAILURE; }
    *markov_chain = (MarkovChain) { database,
         print_cell, cmp_cells, free_cell, copy_func, is_last };
    fill_database(markov_chain);
    for (unsigned int k = 1; k <= num_of_sen; k++)
    {
        printf(COUNT_FORMAT, k);
        generate_random_sequence(markov_chain,
         markov_chain->database->first->data, MAX_GENERATION_LENGTH);
        printf("\n");
    }
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}
