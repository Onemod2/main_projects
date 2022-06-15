#include <iostream>

// is_prime_v
namespace {
  template<size_t N, size_t I>
  constexpr bool divide_without_remain =
    N % I != 0 && divide_without_remain<N, I - 1>;

  template<size_t N>
  constexpr bool divide_without_remain<N, 1> = true;

  template<size_t N>
  constexpr bool is_prime_v = divide_without_remain<N, N / 2>;

  template<>
  constexpr bool is_prime_v<1u> = true;
  template<>
  constexpr bool is_prime_v<2u> = true;
  template<>
  constexpr bool is_prime_v<3u> = true;
}// namespace

// fib
namespace {
  template<size_t N>
  constexpr size_t fib = fib<N - 1> + fib<N - 2>;

  template<>
  constexpr size_t fib<1u> = 1;

  template<>
  constexpr size_t fib<0u> = 1;
}// namespace

// upper_sqrt or value
namespace {
  template<size_t N, size_t I>
  constexpr size_t sqrt_help =
    I *I == N ? I : sqrt_help<N, I + 1>;

  template<size_t N>
  constexpr size_t sqrt_help<N, N> = N;

  template<size_t N>
  constexpr size_t ceil_sqrt_integral_v = sqrt_help<N, 1u>;
}// namespace

// fast is_prime
namespace {
  template<size_t N, size_t CeilSqrt, size_t CeilSqrtMinusOne, size_t I>
  constexpr bool fast_prime_help =
    N % I != 0 && fast_prime_help<N, CeilSqrt, CeilSqrtMinusOne, I + 2>;

  template<size_t N, size_t CeilSqrt, size_t CeilSqrtMinusOne>
  constexpr bool fast_prime_help<N, CeilSqrt, CeilSqrtMinusOne, CeilSqrt> = true;

  template<size_t N, size_t CeilSqrt, size_t CeilSqrtMinusOne>
  constexpr bool fast_prime_help<N, CeilSqrt, CeilSqrtMinusOne, CeilSqrtMinusOne> = true;

  template<size_t N>
  constexpr bool fast_is_prime_v =
    N % 2 != 0 && fast_prime_help<N,
                                  ceil_sqrt_integral_v<N>,
                                  ceil_sqrt_integral_v<N> - 1u,
                                  3u>;

  template<>
  constexpr bool fast_is_prime_v<1u> = true;
  template<>
  constexpr bool fast_is_prime_v<2u> = true;
  template<>
  constexpr bool fast_is_prime_v<3u> = true;
}// namespace

int main() {
  std::cout << std::boolalpha;
  {
    std::cout << is_prime_v<10u> << std::endl;     // false
    std::cout << fast_is_prime_v<10u> << std::endl;// false
    std::cout << is_prime_v<7u> << std::endl;      // true
    std::cout << fast_is_prime_v<7u> << std::endl; // true
    std::cout << is_prime_v<19u> << std::endl;     // true
    std::cout << fast_is_prime_v<19u> << std::endl;// true
    std::cout << is_prime_v<51u> << std::endl;     // false
    std::cout << fast_is_prime_v<51u> << std::endl;// false
    std::cout << is_prime_v<1u> << std::endl;      // true
    std::cout << fast_is_prime_v<1u> << std::endl; // true
  }
  {
    std::cout << fib<3u> << std::endl;  // 3
    std::cout << fib<10u> << std::endl; // 89
    std::cout << fib<100u> << std::endl;// 1298777728820984005
  }
  {                                                      // sqrt or value
    std::cout << ceil_sqrt_integral_v<10u> << std::endl; // 10
    std::cout << ceil_sqrt_integral_v<9u> << std::endl;  // 3
    std::cout << ceil_sqrt_integral_v<256u> << std::endl;// 16
  }

  return 0;
}
