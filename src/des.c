#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "des.h"

#define ARRAY_LEN(array_name) sizeof(array_name) / sizeof(array_name[0])

const uint8_t table_pc_1[] = 
{
    57,   49,    41,   33,    25,    17 ,   9,
    1,   58,    50,   42,    34,    26  , 18,
    10,    2,    59,   51,    43,    35  , 27,
    19,   11,     3,   60,    52,    44  , 36,
    63,   55,    47,   39,    31,    23 ,  15,
    7,   62,    54,   46,    38,   30  , 22,
    14,    6,    61,   53,    45,    37 ,  29,
    21,   13,     5,   28,    20 ,   12,    4
};

const uint8_t table_pc_2[] = 
{
    14,    17 ,  11,    24  ,   1,    5,
    3  ,  28  , 15  ,   6   , 21 ,  10,
    23  ,  19 ,  12  ,   4  ,  26 ,   8,
    16   ,  7 ,  27   , 20    ,13  ,  2,
    41    ,52 ,  31    ,37   , 47  , 55,
    30 ,   40 ,  51  ,  45  ,  33  , 48,
    44  ,  49 ,  39   , 56 ,   34  , 53,
    46   , 42 ,  50   , 36,    29  , 32
};

const uint8_t table_ip[] = 
{
    58,    50 ,  42,    34 ,   26,   18 ,   10,    2,
    60 ,   52 ,  44 ,   36  ,  28 ,  20 ,   12 ,   4,
    62  ,  54 ,  46  ,  38  ,  30  , 22 ,   14  ,  6,
    64   , 56 ,  48   , 40    ,32   ,24    ,16   , 8,
    57    ,49   ,41    ,33   , 25,   17   ,  9 ,   1,
    59,    51  , 43  ,  35  ,  27 ,  19  ,  11  ,  3,
    61 ,   53 ,  45  ,  37 ,   29  , 21 ,   13   , 5,
    63  ,  55,   47  ,  39,    31   ,23,    15   , 7
};

const uint8_t table_ip_reversed[] = 
{
   40 ,    8 ,  48,    16  ,  56,   24  ,  64,   32,
    39 ,    7,   47,    15 ,   55,   23 ,   63,   31,
    38 ,    6,   46 ,   14 ,   54 ,  22 ,   62 ,  30,
    37 ,    5,   45  ,  13 ,   53  , 21  ,  61  , 29,
    36 ,    4,   44   , 12   , 52 ,  20   , 60 ,  28,
    35 ,    3,   43 ,   11  ,  51 ,  19  ,  59 ,  27,
    34 ,    2 ,  42 ,   10 ,   50 ,  18 ,   58 ,  26,
    33  ,   1,   41 ,    9,    49 ,  17,    57 ,  25
};

const uint8_t table_p[] = 
{
    16 ,  7,  20,  21,
    29 , 12,  28,  17,
    1 , 15,  23,  26,
    5 , 18,  31,  10,
    2 ,  8,  24,  14,
    32,  27,   3,   9,
    19,  13,  30,   6,
    22,  11,   4,  25,
};

const uint8_t table_e[] = 
{
   32 ,    1 ,   2,     3  ,   4,    5,
    4 ,    5 ,   6 ,    7  ,   8 ,   9,
    8 ,    9 ,  10  ,  11  ,  12  , 13,
    12 ,   13,   14  ,  15 ,   16  , 17,
    16  ,  17,   18   , 19   , 20 ,  21,
    20  ,  21,   22    ,23  ,  24  , 25,
    24   , 25,   26  ,  27 ,   28   ,29,
    28    ,29,   30   , 31,    32   , 1
};

const uint8_t tables_s[8][64] =
{
    {
        14,  4 , 13,  1 ,  2, 15 , 11,  8 ,  3, 10  , 6, 12  , 5,  9 ,  0,  7,
        0, 15,   7,  4,  14,  2,  13,  1,  10,  6 , 12, 11 ,  9,  5,   3,  8,
        4,  1,  14 , 8,  13 , 6,   2, 11,  15, 12 ,  9,  7 ,  3, 10,   5 , 0,
        15, 12,   8 , 2,   4 , 9 ,  1 , 7,   5 ,11,   3, 14,  10,  0,   6 ,13
    },
    {
        15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10,
      3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5,
      0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6 ,  9,  3,   2, 15,
        13,  8,  10,  1,   3, 15,   4 , 2 , 11,  6 ,  7, 12 ,  0,  5 , 14,  9
    },
    {
        10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8,
        13,  7 ,  0,  9 ,  3,  4 ,  6, 10 ,  2 , 8 ,  5, 14 , 12 ,11 , 15,  1,
        13,  6,   4,  9,   8, 15,   3,  0,  11 , 1,   2, 12 ,  5 ,10 , 14,  7,
        1, 10,  13,  0 ,  6 , 9,   8 , 7 ,  4 ,15 , 14 , 3 , 11 , 5 ,  2, 12
    },
    {
        7, 13 , 14,  3 ,  0,  6 ,  9, 10  , 1,  2  , 8,  5 , 11, 12 ,  4, 15,
        13,  8,  11,  5,   6, 15 ,  0,  3 ,  4,  7 ,  2, 12,   1, 10,  14,  9,
        10,  6,   9,  0,  12, 11,   7, 13,  15,  1 ,  3, 14,   5,  2,   8 , 4,
        3, 15,   0 , 6,  10 , 1,  13 , 8,   9 , 4,   5, 11,  12 , 7,   2, 14
    },
    {
        2, 12  , 4,  1  , 7, 10  ,11,  6  , 8,  5  , 3, 15 , 13,  0 , 14,  9,
        14, 11 ,  2, 12 ,  4,  7 , 13,  1 ,  5,  0 , 15, 10 ,  3,  9 ,  8,  6,
         4 , 2 ,  1, 11 , 10, 13 ,  7 , 8 , 15,  9 , 12 , 5 ,  6 , 3 ,  0, 14,
         11,  8,  12 , 7,   1, 14,   2, 13,   6, 15,   0 , 9,  10 , 4,   5,  3
    },
    {
        12,  1 , 10, 15  , 9,  2 ,  6,  8 ,  0, 13 ,  3,  4 , 14,  7  , 5, 11,
         10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8,
         9, 14,  15 , 5,   2,  8,  12 , 3,   7,  0,   4, 10,   1, 13,  11 , 6,
        4 , 3,   2, 12,   9 , 5,  15, 10,  11, 14,   1 , 7,   6 , 0,   8, 13
    },
    {
        4, 11 ,  2, 14,  15,  0 ,  8, 13,   3, 12,   9,  7,   5, 10,   6,  1,
        13 , 0 , 11,  7 ,  4,  9 ,  1, 10,  14,  3 ,  5, 12 ,  2, 15 ,  8,  6,
         1,  4 , 11, 13,  12 , 3 ,  7, 14,  10, 15,   6,  8 ,  0 , 5,   9 , 2,
         6, 11,  13,  8 ,  1 , 4 , 10,  7,   9 , 5,   0, 15,  14 , 2 ,  3, 12
    },
    {
        13 , 2 ,  8,  4,   6, 15, 11 , 1 , 10,  9,   3 ,14 ,  5 , 0 , 12,  7,
          1 ,15 , 13,  8 , 10,  3 ,  7 , 4 , 12,  5 ,  6 ,11 ,  0, 14 ,  9 , 2,
         7, 11 ,  4 , 1 ,  9 ,12 , 14 , 2 ,  0 , 6,  10, 13 , 15,  3 ,  5,  8,
         2 , 1 , 14 , 7 ,  4, 10 ,  8, 13,  15, 12,   9 , 0,   3 , 5  , 6, 11
    }
};

const uint8_t table_shift[] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// get n-th bit of 64-bit number (starting from index 0)
static uint8_t get_nth_bit_64(const uint64_t num, const uint8_t index)
{
    return 1 & (num >> (64 - index - 1));
}

// get n-th bit of 32-bit number (starting from index 0)
static uint8_t get_nth_bit_32(const uint64_t num, const uint8_t index)
{
    return 1 & (num >> (32 - index - 1));
}

// set n-th bit of 64-bit number (starting from index 0)
static void set_nth_bit(uint64_t *num, const uint8_t index, const uint8_t bit_value)
{
    uint64_t mask = bit_value;
    mask <<= (64 - index - 1);
    *num |= mask;
}

// set n-th bit of 32-bit number (starting from index 0)
static void set_nth_bit_32(uint32_t *num, const uint8_t index, const uint8_t bit_value)
{
    uint64_t mask = bit_value;
    mask <<= (32 - index - 1);
    *num |= mask;
}

static uint64_t permute_key(const uint64_t key_64_bits)
{
    uint64_t key_56_bits = 0;

    for (size_t i = 0; i < ARRAY_LEN(table_pc_1); ++i)
    {
        uint8_t nth_bit = get_nth_bit_64(key_64_bits, table_pc_1[i] - 1);
        set_nth_bit(&key_56_bits, i, nth_bit);
    }

    return key_56_bits;
}

static uint64_t concatenate_key(const uint32_t left_half, const uint32_t right_half)
{
    uint64_t key_56_bits = 0;
    uint64_t key_48_bits = 0;
    key_56_bits |= (left_half >> 4);
    key_56_bits <<= 28;
    key_56_bits |= (right_half >> 4);
    key_56_bits <<= 8;

    for (size_t i = 0; i < ARRAY_LEN(table_pc_2); ++i)
    {
        uint8_t nth_bit = get_nth_bit_64(key_56_bits, table_pc_2[i] - 1);
        set_nth_bit(&key_48_bits, i, nth_bit);
    }

    return key_48_bits;
}

static uint32_t left_shitf(uint32_t num, const uint8_t n)
{
    return (num << n) | (num >> (28 - n));
}

static uint64_t permute_block(const uint64_t block)
{
    uint64_t permuted_block = 0;

    for (size_t i = 0; i < ARRAY_LEN(table_ip); ++i)
    {
        uint8_t nth_bit = get_nth_bit_64(block, table_ip[i] - 1);
        set_nth_bit(&permuted_block, i, nth_bit);
    }

    return permuted_block;
}

static uint32_t permute_half(const uint32_t half)
{
    uint32_t permuted_half = 0;

    for (size_t i = 0; i < ARRAY_LEN(table_p); ++i)
    {
        uint8_t nth_bit = get_nth_bit_32(half, table_p[i] - 1);
        set_nth_bit_32(&permuted_half, i, nth_bit);
    }

    return permuted_half;
}

static uint64_t expand_half(const uint32_t half)
{
    uint64_t expanded_half = 0;

    for (size_t i = 0; i < ARRAY_LEN(table_e); ++i)
    {
        uint8_t nth_bit = get_nth_bit_32(half, table_e[i] - 1);
        set_nth_bit(&expanded_half, i, nth_bit);
    }

    return expanded_half;
}

static uint32_t func(const uint32_t right_half, const uint64_t key)
{
    uint64_t expanded_half = expand_half(right_half);
    uint64_t xor_result = expanded_half ^ key;
    uint32_t result = 0;

    for (size_t i = 0, j = 0; i < 48; i += 6, ++j)
    {
        uint8_t row_index = 2 * get_nth_bit_64(xor_result, i) + get_nth_bit_64(xor_result, i + 5);
        uint8_t col_index = 8 * get_nth_bit_64(xor_result, i + 1) + 4 * get_nth_bit_64(xor_result, i + 2) + 
                            2 * get_nth_bit_64(xor_result, i + 3) + get_nth_bit_64(xor_result, i + 4);

        result <<= 4;
        result |= tables_s[j][row_index * 16 + col_index];
    }

    result = permute_half(result);

    return result;
}

static uint64_t permute_block_reversed(const uint32_t left_half, const uint32_t right_half)
{
    uint64_t permuted_block = 0;
    uint64_t block = 0;
    block |= right_half;
    block <<= 32;
    block |= left_half;

    for (size_t i = 0; i < ARRAY_LEN(table_ip_reversed); ++i)
    {
        uint8_t nth_bit = get_nth_bit_64(block, table_ip_reversed[i] - 1);
        set_nth_bit(&permuted_block, i, nth_bit);
    }

    return permuted_block;
}

void reverse_key_buffer(uint64_t key_buffer[16])
{
    for (size_t i = 0; i < 8; ++i)
    {
        uint64_t tmp = key_buffer[i];
        key_buffer[i] = key_buffer[16 - i - 1];
        key_buffer[16 - i - 1] = tmp;
    }
}

// used to generate 64-bit DES key and initialization vector
uint64_t generate_key(void)
{
    uint64_t key = 0;

    for (size_t i = 0; i < 8; ++i)
    {
        uint8_t byte = (uint8_t)(rand() % UINT8_MAX);
        key <<= 8;
        key |= byte;
    }

    return key;
}

// saves DES-key and IV in file in bin format 
void fwrite_uint64(FILE *f, const uint64_t num)
{
    if (f)
    {
        fwrite(&num, sizeof(num), 1, f);
    }
}

void generate_subkeys(const uint64_t key_64_bits, uint64_t key_buffer[16])
{   
    // permutation
    uint64_t key_56_bits = permute_key(key_64_bits);
    
    // split into two halves
    uint32_t left_half = key_56_bits >> 32;
    uint32_t right_half = key_56_bits >> 4;

    // left circular shift
    uint32_t generated_keys[16][2]; // 16 keys consist of two halves

    for (size_t i = 0; i < 16; ++i)
    {
        generated_keys[i][0] = left_shitf(left_half, table_shift[i]);
        generated_keys[i][1] = left_shitf(right_half, table_shift[i]);

        left_half = generated_keys[i][0];
        right_half = generated_keys[i][1];
    }

    // concat 16 keys
    for (size_t i = 0; i < 16; ++i)
    {
        key_buffer[i] = concatenate_key(generated_keys[i][0], generated_keys[i][1]);
    }
}

uint64_t encrypt_block(uint64_t key_buffer[16], uint64_t block)
{
    block = permute_block(block);
    
    uint32_t left_half_prev = block >> 32;
    uint32_t right_half_prev = block;
    uint32_t left_half = 0;
    uint32_t right_half = 0;

    for (size_t i = 0; i < 16; ++i)
    {
        left_half = right_half_prev;
        right_half = left_half_prev ^ func(right_half_prev, key_buffer[i]);

        left_half_prev = left_half;
        right_half_prev = right_half;
    }

    return permute_block_reversed(left_half, right_half);
}
