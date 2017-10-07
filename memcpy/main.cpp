#include <iostream>
#include <chrono>
#include <xmmintrin.h>
#include <random>

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
        asm volatile (
        "movdqa (%1), %0\n\t"
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

bool test() {
    const int amount = 60;
    const size_t MAXN = (size_t) 1e6;
    auto * from = new char[MAXN];
    auto * to = new char[MAXN];

    std::cout.precision(6);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 25);
    std::uniform_real_distribution<> num_dis(0, MAXN - 1);
    auto gl = [&gen, &dis]() -> char {
        return static_cast<char>('a' + (int) dis(gen));
    };

    for (int j = 0; j < amount; j++) {

        auto n = (size_t) num_dis(gen);
        srand(time(0));
        auto fill_arrays = [from, to, n, gl]() {
            for (size_t i = 0; i < n; i++) {
                from[i] = gl();
                to[i] = '0';
            }
        };
        auto show = [n](char *a) {
            for (size_t i = 0; i < n; i++) std::cout << a[i];
            std::cout << std::endl;
        };
        auto check = [to, from, n]() -> bool {
            for (size_t i = 0; i < n; i++) if (to[i] != from[i]) return false;
            return true;
        };

        fill_arrays();
        double t1 = check_time(easy_memcpy, to, from, n);
        fill_arrays();
//        show(from);
        double t2 = check_time(memcpy_16, to, from, n);
//        show(to);
        if (!check()) return false;

        std::cout << "speed up: " << std::fixed << t1/t2 << std::endl;
    }

    delete(from);
    delete(to);
    return true;
}

int main() {
    bool ok = test();
    if (!ok) {
        std::cout << "FAILED!" << std::endl;
    } else {
        std::cout << "OK" << std::endl;
    }
    return 0;
}