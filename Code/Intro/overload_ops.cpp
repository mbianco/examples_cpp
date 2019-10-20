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

    constexpr base_type() : v{} {}
    constexpr base_type(T x) : v{x} {}

    template <typename Res>
    constexpr Res eval() const {
        typename Res::value_type x = v;
        return {x};
    }
};

struct int_ : base_type<int> {
    using base = base_type<int>;

    using base::base;

    template <typename T>
    constexpr int_(T const& x, std::enable_if_t<is_my_expr<T>::value, int> = 0)
    : base{x.template eval<int_>().v}
    {}

    template <typename T>
    constexpr int_& operator=(T const& x) {
        v = x.template eval<int_>().v;
        return *this;
    }
};

struct float_ : base_type<float> {
    using base = base_type<float>;

    using base::base;

    template <typename T>
    constexpr float_(T const& x, std::enable_if_t<is_my_expr<T>::value, int> = 0)
    : base{x.template eval<float_>().v}
    {}

    template <typename T>
    constexpr float_& operator=(T const& x) {
        v = x.template eval<float_>().v;
        return *this;
    }
};

struct double_ : base_type<double> {
    using base = base_type<double>;

    using base::base;

    template <typename T>
    constexpr double_(T const& x, std::enable_if_t<is_my_expr<T>::value, int> = 0)
    : base{x.template eval<double_>().v}
    {}

    template <typename T>
    constexpr double_& operator=(T const& x) {
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
    constexpr Res eval() const {
        return Res{o1.template eval<Res>().v / o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct mul {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    constexpr Res eval() const {
        return Res{o1.template eval<Res>().v * o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct plus {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    constexpr Res eval() const {
        return Res{o1.template eval<Res>().v + o2.template eval<Res>().v };
    }
};

template <typename O1, typename O2>
struct minus {
    using expr_type = std::true_type;
    O1 o1;
    O2 o2;

    template <typename Res>
    constexpr Res eval() const {
        return Res{o1.template eval<Res>().v - o2.template eval<Res>().v };
    }
};



template <typename O1, typename O2>
constexpr divv<O1, O2> operator/(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
constexpr mul<O1, O2> operator*(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
constexpr plus<O1, O2> operator+(O1 o1, O2 o2) { return {o1, o2}; }
template <typename O1, typename O2>
constexpr minus<O1, O2> operator-(O1 o1, O2 o2) { return {o1, o2}; }

int main() {

    {
        int x{14};
        int y{3};

        double zi = x/y+y/x;
        std::cout << zi << "\n";
    }

    {
        double x{14};
        double y{3};

        double zi = x/y+y/x;
        std::cout << zi << "\n";
    }

    {
        int_ x{14};
        int_ y{3};

        int_ zi = x/y+y/x;

        std::cout << zi.v << "\n";
        double_ z  = x/y+y/x; // This does not work without constructor !!!

        std::cout << z.v << "\n";
    }
    {
        int_ x{14};
        int_ y{3};

        double_ z;
        std::cout << z.v << "\n";
        z = x/y+y/x;

        std::cout << z.v << "\n";
    }

    {
        constexpr int_ x{14};
        constexpr int_ y{3};

        constexpr double_ z  = x/y+y/x; // This does not work without constructor !!!

        static_assert(z.v == 14./3.+3./14., "");

        std::cout << z.v << "\n";
    }
}