#include <iostream>
#include <utility>

template < typename T, typename = void >
struct is_my_expr : std::false_type {};

template < typename T>
struct is_my_expr<T, std::void_t<typename T::expr_type> > : std::true_type {};

template <typename T>
struct base_type {
    using expr_type = std::true_type;
    using value_type = T;
    T v;

    base_type() : v{} {}
    base_type(T x) : v{x} {}

    template <typename Res>
    Res eval() const {
        typename Res::value_type x = v;
        return {x};
    }
};

struct int_ : base_type<double> {
    template <typename T>
    int_& operator=(T const& x) {
        v = x.template eval<int_>().v;
        return *this;
    }
};

struct float_ : base_type<float> {
    template <typename T>
    float_& operator=(T const& x) {
        v = x.template eval<float_>().v;
        return *this;
    }
};

struct double_ : base_type<double> {
    using base = base_type<double>;

    using base::base;

    template <typename T>
    double_(T const& x, std::enable_if_t<is_my_expr<T>::value, int> = 0)
    : base{x.template eval<double_>().v}
    {}

    template <typename T>
    double_& operator=(T const& x) {
        base::v = x.template eval<double_>().v;
        return *this;
    }
};

template <typename O1, typename O2>
struct divv {
        using expr_type = std::true_type;

    O1 o1;
    O2 o2;

    template <typename Res>
    Res eval() const {
        return Res{o1.template eval<Res>().v / o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct mul {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    Res eval() const {
        return Res{o1.template eval<Res>().v * o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct plus {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    Res eval() const {
        return Res{o1.template eval<Res>().v + o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct minus {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    Res eval() const {
        return Res{o1.template eval<Res>().v - o2.template eval<Res>().v };
    }
};



template <typename O1, typename O2>
divv<O1, O2> operator/(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
mul<O1, O2> operator*(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
plus<O1, O2> operator+(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
minus<O1, O2> operator-(O1 o1, O2 o2) { return {o1, o2}; }

int main() {
    {
        int_ x{10};
        int_ y{3};

        double_ z  = x/y; // This does not work without constructor !!!

        std::cout << z.v << "\n";
    }
    {
        int_ x{10};
        int_ y{3};

        double_ z;
        std::cout << z.v << "\n";
        z = x/y;

        std::cout << z.v << "\n";
    }

}