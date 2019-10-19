#include <iostream>
#include <utility>

struct int_ {
    using value_type = int;
    int v;

    template <typename Res>
    Res eval() const {
        typename Res::value_type x = v;
        return {x};
    }
};

struct double_ {
    using value_type = double;
    double v;

    template <typename Res>
    Res eval() const {
        typename Res::value_type x = v;
        return {x};
    }

    template <typename T>
    double_& operator=(T const& x) {
        v = x.template eval<double_>().v;
        return *this;
    }
};

template <typename O1, typename O2>
struct divv {
    O1 o1;
    O2 o2;

    template <typename Res>
    Res eval() const {
        return Res{o1.template eval<Res>().v / o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
divv<O1, O2> operator/(O1 o1, O2 o2) { return {o1, o2}; }

int main() {
    int_ x{10};
    int_ y{3};

    double_ z;
    z = x/y;

    std::cout << z.v << "\n";
}