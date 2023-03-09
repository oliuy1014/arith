/* TODO: remove bitpack.h */
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include <assert.h>
#include <stdio.h>


#define MAX_BITS 64

void print_stuff(int64_t n, unsigned width);
Except_T Bitpack_Overflow = { "Overflow packing bits" };

uint64_t lshift_u(uint64_t n, unsigned shift_sz)
{
        if (shift_sz >= MAX_BITS) {
                return 0;
        } else {
                return n << shift_sz;
        }
}

uint64_t rshift_u(uint64_t n, unsigned shift_sz)
{
        if (shift_sz >= MAX_BITS) {
                return 0;
        } else {
                return n >> shift_sz;
        }
}

int64_t lshift_s(int64_t n, unsigned shift_sz)
{
        if (shift_sz >= MAX_BITS) {
                return 0;
        } else {
                return n << shift_sz;
        }
}

int64_t rshift_s(int64_t n, unsigned shift_sz)
{
        if (shift_sz >= MAX_BITS) {
                return ~0;
        } else {
                return n >> shift_sz;
        }
}

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        if (width <= 0) {
                return false;
        }
        uint64_t max_val = rshift_u(~0 , (MAX_BITS - width));
        return n <= max_val;
}

void print_stuff(int64_t n, unsigned width)
{
        int64_t max_val = rshift_u(~0 , (MAX_BITS - width + 1));
        printf("input = %ld, width = %d, max_val = %ld\n", n, width, max_val);
}

bool Bitpack_fitss(int64_t n, unsigned width)
{       
        if (width <= 0) {
                return false;
        }
        int64_t min_val = lshift_s(~0, width - 1);
        int64_t max_val = rshift_u(~0 , (MAX_BITS - width + 1));
        // print_stuff(n, width);
        return (n >= min_val && n <= max_val);
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (width == 0) {
                return 0;
        }

        uint64_t mask = rshift_u(~0, MAX_BITS - width);
        mask = lshift_u(mask, lsb);
        uint64_t result = word & mask;
        result = rshift_u(result, lsb);
        return result;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (width == 0) {
                return 0;
        }

        // printf("word :%ld\n", word);
        int64_t result = Bitpack_getu(word, width, lsb);
        // printf("getu word :%ld\n", result);
        result = lshift_u(result, (MAX_BITS - width));
        // printf("lshword :%ld\n", result);
        result = rshift_s(result, (MAX_BITS - width));
        // printf("rshword :%ld\n", width));, result);
        return result;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
        // assert(value >= 0);
        // assert(width >= 0);
        //printf("val %ld\n", value);
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        if (!(Bitpack_fitsu(value, width))) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask = rshift_u(~0, MAX_BITS - width);
        mask = ~(lshift_u(mask, lsb));
        word = word & mask;
        value = lshift_u(value, lsb);
        word = word | value;
        return word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        if (!(Bitpack_fitss(value, width))) {
                RAISE(Bitpack_Overflow);
        }

        value = Bitpack_getu(value, width, 0);
        word = Bitpack_newu(word, width, lsb, value);
        return word;
}


