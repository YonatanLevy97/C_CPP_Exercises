#include <string.h>
#include "tests.h"

#define K_1 3
#define K_2 2
#define K_3 (-1)
#define K_4 (-3)
#define K_5 29

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "cpp";
  char out[] = "fss";
  encode (in, K_1);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
    char in[] = "y.z z,y";
    char out[] = "a.b b,a";
    encode (in, K_2);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
    char in[] = "Hello, world...";
    char out[] = "Gdkkn, vnqkc...";
    encode (in, K_3);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
    char in[] = "aba";
    char out[] = "xyx";
    encode (in, K_4);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
    char in[] = "ZYXZYZ";
    char out[] = "CBACBC";
    encode (in, K_5);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "plus";
  char out[] = "mirp";
  decode (in, K_1);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
    char in[] = "ABa, Like C";
    char out[] = "YZy, Jgic A";
    decode (in, K_2);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
    char in[] = "I.like,CPP";
    char out[] = "J.mjlf,DQQ";
    decode (in, K_3);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
    char in[] = "zeoxy";
    char out[] = "chrab";
    decode (in, K_4);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
    char in[] = "CABAMBA";
    char out[] = "ZXYXJYX";
    decode (in, K_5);
    return strcmp (in, out) != 0;
}
