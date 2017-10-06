#include <iostream>
#include <vector>
#include <cassert>
#include <functional>

#include "trampoline.h"

using namespace std;

bool t1() {
    trampoline<long long (int, int, int, int, int, int, int, int, int)>
    tr([&](int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9) {
        cout << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " ";
        cout << c7 << " " << c8 << " " << c9 << "|\n";
        return 0;
    }
       );
    trampoline<long long (int, int, int, int, int, int, int, int, int)>
    tr1([&](int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9) {
        cout << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " ";
        cout << c7 << " " << c8 << " " << c9 << "|\n";
        return 0;
    }
        );
    trampoline<long long (int, int, int, int, int, int, int, int, int)>
    tr2([&](int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9) {
        cout << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " ";
        cout << c7 << " " << c8 << " " << c9 << "|\n";
        return 0;
    }
        );
    trampoline<long long (int, int, int, int, int, int, int, int, int)>
    tr3([&](int c1, int c2, int c3, int c4, int c5, int c6, int c7, int c8, int c9) {
        cout << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " ";
        cout << c7 << " " << c8 << " " << c9 << "|\n";
        return 0;
    }
        );
    
    return true;
}

bool t2() {


    {
        trampoline<int (int, int, int, int, int)>
                t([&] (int p0, int p1, int p2, int p3, int p4) {return p0 * p1 * p2 * p3 * p4;});
        auto p = t.get();
        assert (120 == p(1, 2, 3, 4, 5));
    }
    {
        trampoline<double (double, double, double, double, double)>
                t([&] (double p0, double p1, double p2, double p3, double p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        assert (2.8 == p(1.0, -1.2, 1, 1, 1));
    }
    {
        trampoline<float (int, double, double, float, float)>
                t([&] (int p0, double p1, double p2, float p3, float p4) {return (p1 + p2 + p3 + p4 + p0);});
        auto p = t.get();
        assert (5.5 == p(1, 1.2, 1.3, 1, 1));
    }
    {
        trampoline<int (int&)>
                t([&] (int p0) {return p0;});
        auto p = t.get();
        int k = 5;
        assert (5 == p(k));
    }
    return true;
}


struct test_struct_1 {
    int operator()(int c1, int c2, int c3, int c4, int c5) {
        return 123;
    }
};

bool t0() {
    test_struct_1 tmp;
    trampoline<int (int, int, int, int, int)> tr(tmp);
    int (*p)(int, int, int, int, int) = tr.get();
    assert (123 == p(1, 1, 1, 1, 1));
    return true;
}

bool t3() {
    {
        trampoline<int (int, int, int, int, int)>
        t([&] (int p0, int p1, int p2, int p3, int p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        assert (5 == p(1, 1, 1, 1, 1));
    }
    {
        trampoline<double (double, double, double, double, double)>
        t([&] (double p0, double p1, double p2, double p3, double p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        assert (5.2 == p(1.0, 1.2, 1, 1, 1));
    }
    {
        trampoline<float (float, float, float, float, float)>
        t([&] (float p0, float p1, float p2, float p3, float p4) {return p1 + p2 + p3 + p4 + p0;});
        auto p = t.get();
        float res = 5.2;
        assert (res == p(1.0, 1.2, 1, 1, 1));
    }
    {
        trampoline<float (int, double, int, float, float)>
        t([&] (int p0, double p1, int p2, float p3, float p4) {return (p1 + p2 + p3 + p4 + p0);});
        auto p = t.get();
        float res = 5.2;
        assert (res == p(1.0, 1.2, 1, 1, 1));
    }
    {
        trampoline<int (int&)>
        t([&] (int p0) {return p0;});
        auto p = t.get();
        int a = 1;
        assert (1 == p(a));
    }
    return true;
}

bool t4() {
    cout << "Test 2 started" << endl;
    {
        trampoline<long long (int, int, int, int, int, int, int, int)>
        t([&] (int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7)
        {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " int test completed"  << endl;

    }
    {
        trampoline<long long (double, double, double, double, double, double, double, double)>
        t([&] (double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " double test completed"  << endl;
    }
    {
        trampoline<long long (float, float, float, float, float, float, float, float)>
        t([&] (float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " float test completed"  << endl;
    }
    {
        trampoline<long long (double, int, float, int, int, double, double, float)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        assert (8 == p(1, 1, 1, 1, 1, 1, 1, 1));
        cout << " int/double/float test completed"  << endl;
    }
    {
        trampoline<long long (double&, int&, float&, int, int, double, double, float&)>
        t([&] (double p0, int p1, float p2, int p3, int p4, double p5, double p6, float p7)
          {return p1 + p2 + p3 + p4 + p0 + p5 + p6 + p7;});
        auto p = t.get();
        double a = 1;
        int b = 1;
        float c = 1, d = 1;
        assert (8 == p(a, b, c, 1, 1, 1, 1, d));
        cout << " &/int/double/float test completed"  << endl;
    }
    return true;
}

int main() {
    if (t0() && t1() && t2() && t3()) cout << "OK\n";
    return 0;
}
