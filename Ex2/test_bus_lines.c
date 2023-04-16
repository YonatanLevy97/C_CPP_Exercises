#include "test_bus_lines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define TEST_1_FAILED "TEST 1 FAILED: THE LIST IS NOT SORTED BY DISTANCE\n"
#define TEST_1_PASSED "TEST 1 PASSED: THE LIST IS SORTED BY DISTANCE\n"

#define TEST_2_FAILED "TEST 2 FAILED: THE LISTS DON'T HAVE THE SAME LINES\n"
#define TEST_2_PASSED "TEST 2 PASSED: BOTH LISTS HAVE THE SAME LINES\n"

#define TEST_3_FAILED "TEST 3 FAILED: THE LIST IS NOT SORTED BY DURATION\n"
#define TEST_3_PASSED "TEST 3 PASSED: THE LIST IS SORTED BY DURATION\n"

#define TEST_4_FAILED "TEST 4 FAILED: THE LISTS DON'T HAVE THE SAME LINES\n"
#define TEST_4_PASSED "TEST 4 PASSED: BOTH LISTS HAVE THE SAME LINES\n"

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
    BusLine *traveler_iter = start;
    int traveler_distance = start->distance;
    for ( ; traveler_iter != end; traveler_iter++)
    {
        if(traveler_iter->distance < traveler_distance)
        {
            return 0;
        }
        traveler_distance = traveler_iter->distance;
    }
    return 1;
}


int is_equal(BusLine *start_sorted, BusLine
*end_sorted, BusLine *start_original, BusLine
             *end_original)
{
    long num_original = (end_original - start_original) + 1;
    long num_copy = (end_sorted - start_sorted) + 1;
    if (num_original != num_copy)
    {
        return 0;
    }
    int num_of_equals = 0;
    for (int i = 0; i < num_original; i++)
    {
        int line_number = (start_original+i)->line_number;
        int distance = (start_original+i)->distance;
        int duration = (start_original+i)->duration;
        for (int j = 0; j < num_original; j++)
        {
            int line_num_eq = (line_number == (start_sorted+j)->line_number);
            int distance_eq = (distance == (start_sorted + j)->distance);
            int duration_eq = (duration == (start_sorted + j)->duration);
            if (line_num_eq && distance_eq && duration_eq)
            {
                num_of_equals++;
            }
        }
    }
    return (num_of_equals == num_original);
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
    BusLine *traveler_iter = start;
    int traveler_duration = start->duration;
    for ( ; traveler_iter != end; traveler_iter++)
    {
        if(traveler_iter->duration < traveler_duration)
        {
            return 0;
        }
        traveler_duration = traveler_iter->duration;
    }
    return 1;
}

void print_tests_results(int test1, int test2, int test3, int test4)
{
    if(test1)
    {
        fputs(TEST_1_PASSED, stdout);
    }
    else
    {
        fputs(TEST_1_FAILED, stdout);
    }
    if(test2)
    {
        fputs(TEST_2_PASSED, stdout);
    }
    else
    {
        fputs(TEST_2_FAILED, stdout);
    }
    if(test3)
    {
        fputs(TEST_3_PASSED, stdout);
    }
    else
    {
        fputs(TEST_3_FAILED, stdout);
    }
    if(test4)
    {
        fputs(TEST_4_PASSED, stdout);
    }
    else
    {
        fputs(TEST_4_FAILED, stdout);
    }
}


int handle_tests(BusLine *start, BusLine *end)
{
    long num_of_lines = (end - start);
    BusLine * copy_busses = malloc((num_of_lines + 1) * sizeof (BusLine));
    if(!copy_busses)
    {
        return 0;
    }
    memcpy(copy_busses, start, (num_of_lines + 1) * sizeof (*copy_busses));
    BusLine * end_copy_busses = (copy_busses + num_of_lines);
    bubble_sort(start,end);

    int test1 = (is_sorted_by_distance(start, end) == 1);
    int test2 = is_equal(start, end, copy_busses,end_copy_busses) == 1;
    quick_sort(start,end);
    int test3 = (is_sorted_by_duration(start, end) == 1);
    int test4 = is_equal(start, end, copy_busses, end_copy_busses) == 1;
    print_tests_results(test1, test2, test3, test4);
    free(copy_busses);
    return test1 && test2 && test3 && test4;
}

