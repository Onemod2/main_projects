#include "bounded_synch_associative_buffer.h"
#include <iostream>

int main() {
  bounded_synch_buffer<int, int> buffer;
  buffer.add_or_update_item(4, 4);
  std::cout << buffer.value_for(4, 0) << std::endl;

  return 0;
}