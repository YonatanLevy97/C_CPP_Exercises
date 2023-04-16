#include "sort_bus_lines.h"


void swap_busses(BusLine * bus_a, BusLine * bus_b)
{
    BusLine temp = *bus_a;
    *bus_a = *bus_b;
    *bus_b = temp;
}

void bubble_sort (BusLine *start, BusLine *end)
{
    long n = end - start;
    for (int i = 0; i <= n - 1; i++)
    {
        for (int j = 0; j <= n - i - 1; j++)
        {
            if (((start + j)->distance) > ((start + j + 1)->distance))
            {
                swap_busses((start + j), (start + j + 1));
            }
        }
    }
}


BusLine *partition (BusLine *start, BusLine *end)
{
    BusLine * pivot = end;
    int i = -1;
    for (int j = 0; j < (end - start); j++) {
        if (((start + j)->duration) <= (pivot->duration))
        {
            i++;
            swap_busses((start + i), (start+j));
        }
    }
    swap_busses((start + i + 1),end);
    return (start + i + 1);
}


void quick_sort (BusLine *start, BusLine *end)
{
    if (start < end)
    {
        BusLine * pi = partition(start,end);

        quick_sort(start, pi - 1);
        quick_sort(pi + 1, end);
    }
}