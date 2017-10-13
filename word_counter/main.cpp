#include <bits/stdc++.h>
#include "world_count.h"

using namespace std;

template <typename F>
double check_time(F f, string s) {
    auto start = std::chrono::steady_clock::now();
    f(s.c_str(), s.length());
    auto finish = std::chrono::steady_clock::now();
    auto t = finish - start;
    return std::chrono::duration <double, std::milli> (t).count();
}

const std::array<string, 15> strs = {{
                                             "hello, hello, hellooooooooooo",
                                             "abc",
                                             " aaaaaaaaaaaaaaa aaaaaaaaaaaaaaa",
                                             " aaaaaaaaaaaaaaa aaaaaaaaaaaaaaa a",
                                             " aaaaaaaaaaaaaaa aaaaaaaaaaaaaa  a",
                                             "a               a               ",
                                             "a b c d e f g h a b c d e f g h a b c d e f g h a b c d e f g h ",
                                             "                 omg so hard test               ",
                                             "gg gg ggg ggg ggggggggggggg ggggggggggggggggggggg wp",
                                             "",
                                             "gav",
                                             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                                             "xxx xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx",
                                             "                                           xxx xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx                                         ",
                                             "xxx                                                                                                                                                                 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx",

                                     }};

void f(std::string &s) {
    size_t x = word_count_slow(s.c_str(), s.length());
    size_t y = word_count_fast(s.c_str(), s.length());
    cout << x << " " << y << " ";
    assert(x == y);
    double t1 = check_time(word_count_slow, s);
    double t2 = check_time(word_count_fast, s);
    std::cout << "speed up: " << std::fixed << t1/t2 << std::endl;
}

bool test1() {
    cout << "TEST 1\n";
    for (auto s: strs) {
        f(s);
    }
    cout << "TEST 1 OK\n";
    return true;
}

bool test2() {
    string s;
    ifstream fin("input.txt");

    cout << "TEST 2\n";
    while (getline(fin, s)) {
        f(s);
    }

    cout << "TEST 2 OK\n";
    fin.close();
    return true;
}

int main() {
    cout.precision(6);
   test1();
   test2();
    return 0;
}