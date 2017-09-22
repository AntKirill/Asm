#include <iostream>
#include <chrono>
#include <xmmintrin.h>

void* easy_memcpy( void* dest, const void* src, std::size_t count ) {
    auto * d = reinterpret_cast<char*>(dest);
    auto * s = reinterpret_cast<const char*>(src);
    for (size_t i = 0; i < count; i++) d[i] = s[i];
    return static_cast<void *>(d);
}

size_t align(char *d, const char *s, size_t count) {
    size_t p = 0;
    for (; ((reinterpret_cast<size_t>(d) + p) % 16 != 0) && (p < count); p++)
        d[p] = s[p];
    return p;
}

void* memcpy_8(void* dest, const void* src, size_t count) {
    auto * d = reinterpret_cast<char*>(dest);
    auto * s = reinterpret_cast<const char*>(src);

    size_t p = align(d, s, count);
    size_t tail = (count - p) % 8;

    for (; p < count - tail; p += 8) {
        size_t tmp = 0;
        __asm__ volatile(
        "mov %1, %0\n\t"
        "mov %0, %2"
        :
        : "r"(tmp), "r"((const char*)src + p), "r" ((char*)dest + p));
    }
    for (; p < count; p++) d[p] = s[p];
    return dest;
}

void* memcpy_16(void* dest, void const* src, size_t count) {
    auto * d = reinterpret_cast<char*>(dest);
    auto * s = reinterpret_cast<const char*>(src);
    auto save_ptr = d;

    size_t p = align(d, s, count);
    size_t tail = (count - p) % 16;
    s += p;
    d += p;
    for (size_t i = p; i < count - tail; i += 16) {
        __m128i reg;
        asm (
        "movdqu (%1), %0\n\t"
        "movntdq %0, (%2)\n\t"
        "add $16, %1\n\t"
        "add $16, %2\n\t"
        : "=x" (reg), "=r" (s), "=r" (d)
        : "1" (s), "2" (d)
        : "memory");
    }
    _mm_sfence();

    for (size_t i = 0; i < tail; i++) d[i] = s[i];

    return save_ptr;
}

template <typename F>
double check_time(F f, char *to, char *from, size_t count) {
    auto start = std::chrono::steady_clock::now();
    f(to, from, count);
    auto finish = std::chrono::steady_clock::now();
    auto t = finish - start;
    return std::chrono::duration <double, std::milli> (t).count();
}

void show(char *a, size_t n) {
    for (size_t i = 0; i < n; i++) std::cout << a[i];
    std::cout << std::endl;
}

void test() {
    const int amount = 100;
    const size_t MAXN = (size_t) 1e6;
    auto * from = new char[MAXN];
    auto * to = new char[MAXN];

    std::cout.precision(6);
    for (int j = 0; j < amount; j++) {

        size_t n = 1 + ((size_t) random()) % MAXN;
        srand(time(0));
        auto gen_letter = []() -> char {return (char) ('a' + (rand() % 26));};
        auto fill_arrays = [from, to, n, gen_letter]() {
            for (size_t i = 0; i < n; i++) {
                from[i] = gen_letter();
                to[i] = '0';
            }
        };
        auto show = [n](char *a) {
            for (size_t i = 0; i < n; i++) std::cout << a[i];
            std::cout << std::endl;
        };

        fill_arrays();
        double t1 = check_time(easy_memcpy, to, from, n);
        fill_arrays();
        double t2 = check_time(memcpy_16, to, from, n);

        std::cout << "speed up: " << std::fixed << t1/t2 << std::endl;
    }

    delete(from);
    delete(to);
}

int main() {
    test();
    return 0;
}