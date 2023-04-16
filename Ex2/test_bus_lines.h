#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

/**
 * this function gets pointers to the start and the end of an BusLine array
 * and return 1 (true) if the array is sorted by line's distance
 * or 0 (false) otherwise.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);


/**
 * this function gets pointers to the start and the end of an BusLine array
 * and return 1 (true) if the array is sorted by line's duration
 * or 0 (false) otherwise.
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);


/**
 * this function gets 2 sets of pointers: 1.) start and end of BusLine array
 * and 2.) start and end of sorted copy of BusLine array.
 * Finally, the func return 1 (true) if the copy array have the exact elements
 * as the original array (exact = all the elements and no more or less).
 * and return 0 (false) otherwise.
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);


/**
 * this function gets pointers to the start and the end of an BusLine array,
 * copy the array (allocate memory, but frees its memory at the end),
 * then handle all the tests calling.
 * Finally, return 1 (true) in case all tests passed
 * and 0 (false) otherwise.
 */
int handle_tests(BusLine *start, BusLine *end);

/**
 * this function gets 4 tests results,
 * for each test: checks if the result is passed or failed
 * and then, prints the result to stdout.
 */
void print_tests_results(int test1, int test2, int test3, int test4);


#endif //EX2_REPO_TESTBUSLINES_H
