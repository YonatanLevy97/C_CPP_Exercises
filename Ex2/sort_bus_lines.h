#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

/**
BusLine struct is represent a bus line.
every bus line struct have: line number, distance and duration.
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;


/**
* this function gets pointers to start and end of Struct BusLine array, and
* sorts the array by line distance (with bubble sort algorithm)
*/
void bubble_sort (BusLine *start, BusLine *end);

/**
 * this function gets pointers to start and end of Struct BusLine array, and
 * sorts the array by line duration (with quick sort algorithm)
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * quick sort helper: take one element from array and put it in the right place
 * in the array.
 */
BusLine *partition (BusLine *start, BusLine *end);

/**
 * this function gets two pointers to struct BusLine and swap they value;
*/
void swap_busses(BusLine * bus_a, BusLine * bus_b);

#endif //EX2_REPO_SORTBUSLINES_H
