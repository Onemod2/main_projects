#include <cstdlib>

// Main class of conteiner that contein any type of data
//

template <typename T, typename Alloc = std::allocator<T>>
class Conteiner {
private:
	T* data; 
};
