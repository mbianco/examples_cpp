#include <iostream>
#include <tuple>
#include <functional>
#include <utility>

namespace deduce_args {
    template <typename R, typename F, typename ...Args>
    using fpconstguy = R(F::*)(Args...) const;

    template <typename R, typename F, typename ...Args>
    using fpguy = R(F::*)(Args...);

    template <typename R, typename ...Args>
    std::tuple<Args...> deduce_arg_types(R(*)(Args...));

    template <typename R, typename F, typename ...Args>
    std::tuple<Args...> deduce_arg_types(R(F::*)(Args...) const);


    template <typename R, typename F, typename ...Args>
    std::tuple<Args...> deduce_arg_types(R(F::*)(Args...) );

    template <typename R, typename F, typename ...Args>
    R(F::* const_fp(R(F::*)(Args...) const) )(Args...) const ;

    template <typename R, typename F, typename ...Args>
    R(F::* non_const_fp(R(F::*)(Args...)) )(Args...) ;
} // namespace deduce_args

int foo(int, int) { return 0; }

struct A {
    int operator()(float, int) const {return 0;}
};

struct B {
    int operator()(int, int) {return 0;}
};


struct C {
    int operator()(int, int) const {return 1;}
    int operator()(int, float) {return 2;}
};

int main() {
    using namespace deduce_args;

    static_assert(std::is_same_v<decltype(deduce_arg_types(foo)), std::tuple<int, int> >, "" );

    static_assert(std::is_same_v<decltype(deduce_arg_types(&A::operator())), std::tuple<float, int> >, "" );

    static_assert(std::is_same_v<decltype(deduce_arg_types(&B::operator())), std::tuple<int, int> >, "" );

    decltype(const_fp(&C::operator())) ff = &C::operator();
    static_assert(std::is_same_v<decltype(deduce_arg_types(ff)), std::tuple<int, int> >, "" );

    decltype(non_const_fp(&C::operator())) ffnc = &C::operator();
    static_assert(std::is_same_v<decltype(deduce_arg_types(ffnc)), std::tuple<int, float> >, "" );

    auto lam = [](int, int) { return 0; };

    using LAM = decltype(lam);
    static_assert(std::is_same_v<decltype(deduce_arg_types(&LAM::operator())), std::tuple<int, int> >, "" );
}

