#include <cassert>
#include <x86intrin.h>
#include "world_count.h"

size_t word_count_slow(const char *str, size_t size) {
    bool prv_space = true;
    size_t cnt = 0;
    for (size_t i = 0; i < size; i++) {
        if (prv_space && str[i] != ' ') cnt++;
        prv_space = (str[i] == ' ');
    }
    return cnt;
}

static inline int popcnt128(const __m128i &n) {
    const __m128i n_hi = _mm_unpackhi_epi64(n, n);
    return static_cast<int>(__popcntq(static_cast<uint64_t>(_mm_cvtsi128_si64(n)))
                            + __popcntq(static_cast<uint64_t>(_mm_cvtsi128_si64(n_hi))));
}

size_t word_count_fast(const char *str, size_t size) {
    static const __m128i all_spaces = _mm_set1_epi8(' ');
    static const __m128i all_zeros = _mm_set1_epi8(0);
    static const __m128i all_ff = _mm_set1_epi8(static_cast<char>(0xff));
    static const __m128i highest_ff = _mm_set_epi8(static_cast<char>(0xff), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (size < 32) return word_count_slow(str, size);

    const char *p = str;
    bool prv_space = true;
    size_t cnt = 0;
    //first easy count of not aligned
    for (; reinterpret_cast<size_t>(p) % 16 != 0; p++) {
        if (prv_space && *p != ' ') cnt++;
        prv_space = (*p == ' ');
    }

    size_t rest = (static_cast<size_t>(str + size - p)) % 16;

    __m128i prv_store = prv_space ? all_ff : all_zeros;
    for (; p != str + size - rest; p += 16) {
        __m128i store = _mm_cmpeq_epi8(_mm_load_si128(reinterpret_cast<const __m128i *>(p)), all_spaces);
        __m128i word_mask = _mm_and_si128(_mm_xor_si128(all_ff, store),
                                          _mm_alignr_epi8(store, prv_store, 15));

        cnt += popcnt128(word_mask) / 8;
        prv_store = store;
    }

    __m128i a = highest_ff;
    __m128i b = _mm_and_si128(prv_store, a);
    int k = _mm_movemask_epi8(b);
    prv_space = (k != 0);

    //second easy count of not aligned
    for (; p != str + size; p++) {
        if (prv_space && *p != ' ') cnt++;
        prv_space = (*p == ' ');
    }

    return cnt;
}


