#include "bitpack.h"
#include "assert.h"
#include <stdio.h>
#include <stdbool.h>

void check_fits();
void check_getu();
void check_gets();
void check_g_nu(uint64_t word, unsigned w, unsigned lsb, uint64_t val);
void check_g_ns(uint64_t word, unsigned w, unsigned lsb, int64_t val);
int main()
{
        check_fits();
        check_getu();
        check_gets();
        check_g_nu(2345, 5, 3, 21);
        check_g_ns(2345, 5, 3, -16);
        check_g_ns(2345, 5, 3, 15);
        check_g_nu(6969420, 8, 4, -127);
        check_g_ns(6969420, 8, 4, -128);
}

void check_fits()
{
        bool fitsu = Bitpack_fitsu(9, 3);
        assert(!fitsu);
        fitsu = Bitpack_fitsu(7, 3);
        assert(fitsu);
        fitsu = Bitpack_fitsu(127, 7);
        assert(fitsu);
        fitsu = Bitpack_fitsu(128, 7);
        assert(!fitsu);
        bool fitss = Bitpack_fitss(4, 3);
        assert(!fitss);
        fitss = Bitpack_fitss(3, 3);
        assert(fitss);
        fitss = Bitpack_fitss(-4, 3);
        assert(fitss);
        fitss = Bitpack_fitss(-6, 3);
        assert(!fitss);
        fitss = Bitpack_fitss(-5, 3);
        assert(!fitss);
        fitss = Bitpack_fitss(31, 6);
        assert(fitss);
        fitss = Bitpack_fitss(32, 6);
        assert(!fitss);
        fitss = Bitpack_fitss(-32, 6);
        assert(fitss);
        fitss = Bitpack_fitss(-33, 6);
        assert(!fitss);
        fitss = Bitpack_fitss(-55, 6);
        assert(!fitss);
}

void check_getu()
{
        assert(Bitpack_getu(1985229328, 4, 0) == 0);
        assert(Bitpack_getu(1985229328, 4, 4) == 1);
        assert(Bitpack_getu(1985229328, 4, 8) == 2);
        assert(Bitpack_getu(1985229328, 4, 12) == 3);
        assert(Bitpack_getu(243, 5, 3) == 30);
        assert(Bitpack_getu(30, 1, 0) == 0);
}
void check_gets()
{
        assert(Bitpack_gets(1985229328, 4, 0) == 0);
        assert(Bitpack_gets(1985229328, 4, 4) == 1);
        assert(Bitpack_gets(1985229328, 4, 8) == 2);
        assert(Bitpack_gets(1985229328, 4, 12) == 3);
        assert(Bitpack_gets(1985229328, 4, 16) == 4);
        assert(Bitpack_gets(1985229328, 4, 20) == 5);
        assert(Bitpack_gets(1985229328, 4, 24) == 6);
        assert(Bitpack_gets(1985229328, 4, 28) == 7);
        assert(Bitpack_gets(1985229328, 4, 32) == 0);
        assert(Bitpack_gets(1985229328, 4, 36) == 0);

        assert(Bitpack_gets(15, 0, 0) == 0);
        assert(Bitpack_gets(15, 3, 0) == -1);
        assert(Bitpack_gets(15, 1, 1) == -1);
        assert(Bitpack_gets(15, 1, 2) == -1);
}
// void check_newu()
// {
        
// }

void check_g_nu(uint64_t word, unsigned w, unsigned lsb, uint64_t val)
{
        assert((Bitpack_getu(Bitpack_newu(word, w, lsb, val), w, lsb)) == val);
}

void check_g_ns(uint64_t word, unsigned w, unsigned lsb, int64_t val)
{
        assert(Bitpack_gets(Bitpack_news(word, w, lsb, val), w, lsb) == val);
}
