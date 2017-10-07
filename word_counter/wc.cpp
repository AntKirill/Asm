#include <cstddef>
#include <string>
#include <xmmintrin.h>
#include <x86intrin.h>
#include <iostream>


static size_t upd(size_t &wc, char cur, bool &space) {
    if (space && cur != ' ') {
        wc++;
    }
    space = (cur == ' ');
    return wc;
}

size_t word_count_slow(const char* str, const size_t size) {
    size_t cnt = 0;
    bool space = true;
    for (size_t i = 0; i < size; i++) {
        upd(cnt, str[i], space);
    }
    return cnt;
}

static const __m128i all_spaces = _mm_set1_epi8(' ');
static const __m128i all_zeros = _mm_set1_epi8(0);

static size_t simd_step(__m128i &cur, __m128i &prev, __m128i &x, bool last) {
    size_t d = 0;

    x = _mm_adds_epu8(
            _mm_and_si128(
                _mm_set1_epi8(1), 
                _mm_andnot_si128(
                    _mm_alignr_epi8(cur, prev, 1), 
                    prev)
                ),
            x
        );

    if (last || _mm_movemask_epi8(x) != 0) {
        x = _mm_sad_epu8(all_zeros, x);
        d += _mm_cvtsi128_si32(x);
        x = _mm_srli_si128(x, 8);
        d += _mm_cvtsi128_si32(x);
        x = all_zeros;
    }

    prev = cur;
    return d;
}

size_t word_count_fast(const char* const new_str, const size_t size) {
    const char* str = new_str;
    bool space = true;
    if (size < 128) {
        return word_count_slow(str, size);
    }
    size_t p = 0;
    size_t word_counter = (str[0] == ' ') ? 0 : 1;

    //fisrt copy of not alligned
    space = false;
    for (;(size_t)str % 16 != 0; p++, str++)
        upd(word_counter, *str, space);


    if (space && *str != ' ' && p != 0) word_counter++;
    size_t tail = size - (size - p) % 16 - 16;

    __m128i cur = _mm_cmpeq_epi8(_mm_load_si128((__m128i*) str), all_spaces);
    __m128i prev = cur;
    __m128i x = all_zeros;

    for (size_t i = p; i < tail; i += 16, str += 16) {
        cur = _mm_cmpeq_epi8(_mm_load_si128((__m128i*) (str + 16)), all_spaces);
        word_counter += simd_step(cur, prev, x, i + 16 >= tail);
    }

    p = tail;
    if (*(str - 1) == ' ' && *(str) != ' ') {
        word_counter--;
    }

    //second copy of not alligned
    space = (*(str - 1) == ' ');
    for (size_t i = p; i < size; i++, str++) 
        upd(word_counter, *str, space);

    return word_counter;
}