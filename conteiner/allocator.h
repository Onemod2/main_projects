// Header and source of allocator

// If data of allocator was ended, allocator throw an exception;
// Allocator don't check if user give not our address;

#include <stdexcept>

template <typename T>
class Allocator {
	private:
		T* data;
		std::size_t current = 0;
		const std::size_t max_size;
	
	public:
		Allocator(const std::size_t);
		Allocator();
		~Allocator();
	
		T* allocate(const std::size_t);
		void deallocate(void*, const std::size_t);
		
		template <typename... Args>
		void construct(void *, Args&&...);
		void destroy(T*);
		
		using value_type = T;
};	

template <typename T>
Allocator<T>::Allocator(const std::size_t max_size) : max_size(max_size) {
	data = reinterpret_cast<T*>(new char[max_size * sizeof(T)]);
}

template <typename T>
Allocator<T>::~Allocator() {
	if (data != nullptr) {
		delete[] reinterpret_cast<char*>(data);
	}
}

template <typename T>
Allocator<T>::Allocator() : data(nullptr), max_size(0u) {}	

template <typename T>
T* Allocator<T>::allocate(const std::size_t n) {
	if (current + n > max_size) {
		throw std::overflow_error("Exceed a max size of allocator");	
	}

	std::size_t tmp = current;
	current += n;

	return data + tmp;	
}

template <typename T>
void Allocator<T>::deallocate(void *ptr, std::size_t n) {
	return;
}

template <typename T>
template <typename... Args>
void Allocator<T>::construct(void *ptr, Args&&... args) {
	new (ptr) T(std::forward<Args&&>(args)...);
}

template <typename T>
void Allocator<T>::destroy(T* obj) {
	obj->~T();
}


