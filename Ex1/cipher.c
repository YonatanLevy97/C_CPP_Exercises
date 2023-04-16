#include "cipher.h"
#include <string.h>

#define FULL_CYCLE 26
/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.
enum AsciiLetters
{
    MIN_UPPER = 65,
    MAX_UPPER = 90,

    MIN_LOWER = 97,
    MAX_LOWER = 122,
};

int get_tabbed_char_backward(char s, int k);
int get_tabbed_char_forward(char s, int k);

int get_tabbed_char_backward(char s, int k)
{
    int k_after_cycles = k % FULL_CYCLE;
    int sub_s_k = s - k_after_cycles;

    if (MIN_UPPER <=s && s <= MAX_UPPER)
    {
        _Bool is_cycle = (sub_s_k < MIN_UPPER);
        return is_cycle ? MAX_UPPER+1 - (MIN_UPPER - sub_s_k) : sub_s_k;
    }
    else if (MIN_LOWER <= s && s <= MAX_LOWER)
    {
        _Bool is_cycle = (sub_s_k < MIN_LOWER);
        return is_cycle ? MAX_LOWER+1 - (MIN_LOWER - sub_s_k) : sub_s_k;
    }
    else
    {
        return s;
    }
}

int get_tabbed_char_forward(char s, int k)
{
    int k_after_cycles = k % FULL_CYCLE;
    int sum_s_k = s + k_after_cycles;

    if (MIN_UPPER <=s && s <= MAX_UPPER)
    {
        _Bool is_cycle = (sum_s_k > MAX_UPPER);
        return is_cycle ? MIN_UPPER-1 + (sum_s_k % MAX_UPPER) : sum_s_k;
    }
    else if ( MIN_LOWER <= s && s <= MAX_LOWER)
    {
        _Bool is_cycle = (sum_s_k > MAX_LOWER);
        return is_cycle ? MIN_LOWER-1 + (sum_s_k % MAX_LOWER) : sum_s_k;
    }
    else
    {
        return s;
    }
}



// See full documentation in header file
void encode (char s[], int k)
{
    _Bool is_k_positive = (k >= 0);
    for (int i=0; i < (int) strlen(s); i++)
    {
        if (is_k_positive)
        {
            s[i] = (char) (get_tabbed_char_forward(s[i], k));
        }
        else
        {
            s[i] = (char) get_tabbed_char_backward(s[i], (-k));
        }
    }
}

// See full documentation in header file
void decode (char s[], int k)
{
    _Bool is_k_positive = (k >= 0);
    for(int i=0; i < (int) strlen(s); i++)
    {
        if(is_k_positive)
        {
            s[i] = (char) get_tabbed_char_backward(s[i], k);
        }
        else
        {
            s[i] = (char) get_tabbed_char_forward(s[i], (-k));
        }
    }
}
