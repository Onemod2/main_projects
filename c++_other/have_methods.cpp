#include <vector>

template <typename T>
struct hasSizeMethod {
    template <typename TT>
    static auto help_f(TT) -> 
			decltype( std::declval<TT>().size(), char() );

    template <typename...>
    static int help_f(...);

    static const bool value = 
			std::is_same_v<decltype(help_f<T>( std::declval<T>() )), char>;
};

template <typename T>
bool has_size_method_v = hasSizeMethod<T>::value;

template <typename T, typename... Args>
auto hasFooMethod(T) -> 
	decltype( std::declval<T>().foo( std::declval<Args>()... ), char() );

template <typename...>
int hasFooMethod(...);

template <typename T, typename... Args>
bool has_foo_method_v = 
	std::is_same_v<
	decltype(hasFooMethod<T, Args...>( std::declval<T>() )),
  char>;

struct Test {
    void foo() {}
    void foo(int, int) {}
    void foo(char, char) {}
};

int main() {
    std::cout << has_foo_method_v<Test, int, int> << std::endl;
    std::cout << has_foo_method_v<Test, char, char> << std::endl;
    std::cout << has_foo_method_v<Test, int> << std::endl;
    std::cout << has_foo_method_v<Test> << std::endl;
    std::cout << has_foo_method_v<int> << std::endl;

    return 0;
}
