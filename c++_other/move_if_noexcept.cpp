#include <iostream>
#include <vector>


template <typename T, T v>
struct integral_constant {
    static const T value = v;
};

struct true_type: integral_constant<bool, true> {};
struct false_type: integral_constant<bool, false> {};

template <typename T>
struct is_nothrow_move {
    template <typename TT>
    static auto f(int) ->
        std::conditional_t<noexcept(TT( std::declval<TT>() )),
					 true_type, false_type>;

    template <typename...>
    static auto f(...) -> false_type;

    static const bool value = decltype(f<T>(0))::value;
};

template <typename T>
static const bool is_nothrow_move_v = is_nothrow_move<T>::value;

template <typename T>
auto my_move_if_noexcept(T& obj) ->
		 std::conditional_t< is_nothrow_move_v<T>, T&&, const T&> {
    return std::move(obj);
}

struct Test {
    Test(Test&&) noexcept {}
    Test() {}
    Test(const Test&) {}
};

template <typename T>
void my_type_is(T&&);

int main() {
    Test obj;

    my_type_is(my_move_if_noexcept(obj) );
    my_type_is( obj );

    return 0;
}
