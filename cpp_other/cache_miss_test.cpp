#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::size_t;

template<typename Iterator, typename T>
void sum_fast(Iterator first, Iterator last, T &init) {
  T variable{};
  for (; first != last; ++first) {
    variable += *first;
  }
  init = variable;
}

template<typename Iterator, typename T>
void sum_slow(Iterator first, Iterator last, T &init) {
  for (; first != last; ++first) {
    init += *first;
  }
}

template<typename T, typename Iterator, typename Func>
T reduce_par(Iterator first, Iterator last, size_t size, Func f) {
  size_t thread_num = std::thread::hardware_concurrency();
  size_t size4thread = size / thread_num;
  std::vector<std::thread> threads(thread_num);
  std::vector<T> res(thread_num, T());

  for (size_t i = 0; i < thread_num; ++i, first += size4thread) {
    threads[i] = std::thread(f, first, first + size4thread, std::ref(res[i]));
  }

  T local_result{};
  f(first, last, local_result);

  for (auto &t: threads) {
    t.join();
  }

  for (T &thread_result: res) {
    local_result += thread_result;
  }
  return local_result;
}

int main(int argc, char **argv) {
  size_t v_size = 2'000'000'000;
  std::vector<int> v(v_size, 1);

  std::cout << "cash-miss version" << std::endl;
  auto first = std::chrono::system_clock::now();
  std::cout << reduce_par<int>(v.begin(), v.end(), v.size(), sum_slow<std::vector<int>::iterator, int>) << std::endl;
  std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - first)).count() << std::endl;

  std::cout << "fast version" << std::endl;
  first = std::chrono::system_clock::now();
  std::cout << reduce_par<int>(v.begin(), v.end(), v.size(), sum_fast<std::vector<int>::iterator, int>) << std::endl;
  std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - first)).count() << std::endl;

  return 0;
}