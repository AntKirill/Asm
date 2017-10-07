#include <bits/stdc++.h>
#include "wc.h"

using namespace std;

const std::array<string, 10> strs = { {
	"hello, hello, hellooooooooooo", 
	"abc",
	"                 omg so hard test               ", 
	"gg gg ggg ggg ggggggggggggg ggggggggggggggggggggg wp",
	"",
	"gav", 
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
	"xxx xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx",
	"                                           xxx xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx                                         ",
	"xxx                                                                                                                                                                xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx xxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxx xxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxx xx x x x x x xx x xx x xx x x x x xx x x x x xx x x x x xx x x x xx x x x xx x x x xx x x x x xx x x xxxxxxx x xxxxxxxxxx",

}};

bool test() {
	for (auto s: strs) {
		size_t x = word_count_slow(s.c_str(), s.length());
		size_t y = word_count_fast(s.c_str(), s.length());
		cout << x << " " << y << endl;
		assert(x == y);
	}
	return true;
}

int main() {
	if (test()) cout << "OK\n";

    return 0;
}