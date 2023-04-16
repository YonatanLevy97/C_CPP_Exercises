#include "sort_bus_lines.h"
#include "test_bus_lines.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_NUM 999
#define MAX_DISTANCE_NUM 1000
#define MAX_DURATION 100
#define MIN_DURATION 10
#define DECIMAL 10
#define MAX_INPUT_SIZE 60

#define INPUT_NUM_OF_LINES "Enter number of lines. Then enter\n"
#define INPUT_LINE_INFO "Enter line info. Then enter\n"

#define USAGE_ERR "USAGE: Program arguments are 'bubble', 'test', or" \
                                        " 'quick'. Please enter one argument\n"
#define NUM_OF_LINES_ERR "ERROR: Number of lines should be an integer " \
                                                          "greater than 1\n"
#define LINE_NUM_ERR "ERROR: Line number should be an integer " \
                                              "between 1 and 999(includes)\n"
#define DURATION_ERR "ERROR: Duration should be an integer " \
                                            "between 10 and 100(includes)\n"
#define DISTANCE_ERR "ERROR: Distance should be an integer " \
                                            "between 0 and 1000(includes)\n"


/**
 * this function checks the validation of inputs: 'line number', 'distance'
 * and 'duration', and prints the error msg, if needed.
 * Finally, returns true in case the data is ok, and false otherwise.
 */
int is_valid_line_info(int line_num, int distance, int duration);


/**
 * this function get the inputs: 'line number', 'distance' and 'duration'.
 * Finally, storage the data in the vars: 'line_num', 'distance'
 * and 'duration'.
 */
void input_line_info(int *line_num, int *distance, int *duration);


/**
 * this function storage the vars: 'line number', 'distance' and 'duration'
 * in BusLine *line vars respectively.
 */
void set_line_input(BusLine *line, const int *line_num,
                    const int *distance, const int *duration);


/**
 * this function get the number of all lines input,
 * and checks if valid. Finally, if valid, it's return the data,
 * otherwise, keep asking for data from the user, until it gets valid input.
 */
long input_num_of_lines();


/**
 * this function checks the validation of argv[1], return true in case is equal
 * to one of: 'test', 'quick' or 'bubble'. Otherwise: false.
 */
int is_valid_command(const char * command);


/**
 * this function gets array of line busses and for each bus line it's print
 * 'line number', 'distance' and 'duration' to stdout.
 */
void print_all_lines(BusLine *line, long num_of_lines);


/**
 * this function used 'input_line_info' func to gets 'line number', 'distance'
 * and 'duration'. Then checks validation.
 * Finally, if valid inputs, it's used 'set_line_input' func to storage the
 * vars in the struct line bus.
 * otherwise, keep asking for vars, until it's valid.
 */
void set_input_valid_line(BusLine * busses, int index);


/**
 * this function start the program {tests, bubble sort or quick wort}
 * with the right command.
 */
int handle_command(const char * command, BusLine * busses, long num_lines,
                    int *print_lines);


int is_valid_line_info(int line_num, int distance, int duration)
{
    int is_valid = 1;
    if ((line_num < 1) || (line_num > MAX_LINE_NUM))
    {
        fputs(LINE_NUM_ERR, stdout);
        is_valid = 0;
    }
    else if ((distance < 0) || (distance > MAX_DISTANCE_NUM))
    {
        fputs(DISTANCE_ERR, stdout);
        is_valid = 0;
    }
    else if ((duration < MIN_DURATION) || (duration > MAX_DURATION))
    {
        fputs(DURATION_ERR, stdout);
        is_valid = 0;
    }
    return is_valid;
}

void input_line_info(int *line_num, int *distance, int *duration)
{
    fputs(INPUT_LINE_INFO, stdout);
    char line_info[MAX_INPUT_SIZE];
    fgets(line_info, MAX_INPUT_SIZE ,stdin);
    sscanf(line_info, "%i,%i,%i",line_num,distance,duration);
}

void set_line_input(BusLine *line, const int *line_num,
                    const int *distance, const int *duration)
{
    line->line_number = *line_num;
    line->distance = *distance;
    line->duration = *duration;
}

long input_num_of_lines()
{
    long num_of_lines = 0;
    do
    {
        fputs(INPUT_NUM_OF_LINES, stdout);
        char num_of_lines_str[MAX_INPUT_SIZE];
        fgets(num_of_lines_str, MAX_INPUT_SIZE ,stdin);
        num_of_lines = strtol(num_of_lines_str, NULL,DECIMAL);
        if (num_of_lines <= 0)
        {
            fputs(NUM_OF_LINES_ERR, stdout);
        }
    }
    while (num_of_lines <= 0);
    return num_of_lines;
}


int is_valid_command(const char * command)
{
    int is_test = strcmp(command, "test") == 0;
    int is_quick_sort = strcmp(command, "quick") == 0;
    int is_bubble_sort = strcmp(command, "bubble") == 0;
    return ((is_test) || (is_quick_sort) || (is_bubble_sort));
}

void print_all_lines(BusLine *lines, long num_of_lines){
    for(int i=0; i < num_of_lines; i++)
    {
        char bus_info[MAX_INPUT_SIZE];
        int num = (lines+i)->line_number;
        int distance = (lines+i)->distance;
        int duration = (lines+i)->duration;
        sprintf(bus_info, "%i,%i,%i\n", num, distance, duration);
        fputs(bus_info, stdout);
    }
}

int handle_command(const char * command, BusLine * busses, long num_lines,
                    int *print_lines)
{
    if (strcmp(command, "test") == 0)
    {
        *print_lines = 0;
        return handle_tests(busses, (busses + num_lines - 1));
    }
    else if(strcmp(command, "quick") == 0)
    {
        quick_sort(busses, (busses + num_lines - 1));
    }
    else
    {
        bubble_sort(busses, (busses + num_lines - 1));
    }
    return 1;
}

void set_input_valid_line(BusLine * busses, int index)
{
    int line_num, distance, duration;
    do
    {
        input_line_info(&line_num, &distance, &duration);
    }
    while (!is_valid_line_info(line_num,distance,duration));
    set_line_input((busses+index), &line_num, &distance, &duration);
}

int main (int argc, char *argv[])
{
    if (argc != 2 || !(is_valid_command(*(argv+1))))
    {
        fputs(USAGE_ERR, stdout);
        return EXIT_FAILURE;
    }
    char * command = *(argv+1);
    long num_of_lines = input_num_of_lines();
    BusLine * all_busses = calloc(num_of_lines, sizeof (BusLine));
    if (!all_busses)
    {
        return EXIT_FAILURE;
    }
    for(int i = 0; i < num_of_lines; i++)
    {
        set_input_valid_line(all_busses,i);
    }
    int print_lines = 1; // true
    int exit_code = handle_command(
                        command, all_busses, num_of_lines,&print_lines);
    if(print_lines)
    {
        print_all_lines(all_busses, num_of_lines);
    }
    free(all_busses);
    return exit_code ?  EXIT_SUCCESS : EXIT_FAILURE;
}
