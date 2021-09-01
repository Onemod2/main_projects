#include <cstdlib>
#include <algorithm>
#include <memory>
#include <exception>

// Main class of conteiner that contein any type of data
//

template <typename T, typename Alloc = std::allocator<T>>
class Conteiner {
public:
	Conteiner();
	Conteiner(std::size_t size, T obj = T());

	Conteiner(Conteiner&& cnt);
	Conteiner(const Conteiner& cnt);
		
	template <typename U>
	void push_back(U&& obj);

	template <typename... Args>
	void emplace_back(Args&&... args);

	void reserve(std::size_t size);

	size_t size() const noexcept {
		return sz;
	}

	T& operator[](size_t i) {
		return data[i];
	}

	T& at(size_t i) {
		if (i >= sz) {
			throw std::out_of_range();
		}
		return data[i];
	}

private:
	T* data = nullptr;
	std::size_t sz = 0;
	std::size_t capa = 0;

	Alloc alloc;

	using AllocTraits = std::allocator_traits<Alloc>;
};

template <typename T, typename Alloc>
Conteiner<T, Alloc>::Conteiner() : sz(0u), capa(0u), data(nullptr) {}

template <typename T, typename Alloc>
Conteiner<T, Alloc>::Conteiner(std::size_t size, T obj) {

	reserve(size * 2);

	for (size_t i = 0; i < size; ++i) {
		try {	
			AllocTraits::construct(alloc, data + i, obj);
		} 
		catch(std::exception& e) {
			for (size_t j = 0; j < i; ++j) {
				AllocTraits::destroy(alloc, data + i);	
			}

			AllocTraits::deallocate(alloc, data, size * 2);
			this->sz = 0;
		  this->capa = 0;
		
			throw e;
		}	
	}

	this->sz = size;
}

template <typename T, typename Alloc>
void Conteiner<T, Alloc>::reserve(size_t capa) {
	if (capa <= this->capa) {
		return;
	}

	T* new_arr = AllocTraits::allocate(alloc, capa);

	for (size_t i = 0; i < this->sz; ++i) {
		try {
			AllocTraits::construct(alloc, new_arr + i, std::move_if_noexcept(data[i]));	
		}
		catch (std::exception& e) {
			for (size_t j = 0; j < i; ++j) {
				AllocTraits::destroy(alloc, new_arr + i);	
			}
			AllocTraits::deallocate(alloc, new_arr, capa);

			throw e; 
		}
	}

	T* tmp = data;
	data = new_arr;
	
	AllocTraits::deallocate(alloc, tmp, this->capa);	

	this->capa = capa;	
}

template <typename T, typename Alloc>
template <typename U>
void Conteiner<T, Alloc>::push_back(U&& obj) {
	if (sz == capa) {
		reserve( std::max(std::size_t(1), 2 * sz) );
	}
	
	AllocTraits::construct(alloc, data + sz, std::forward<U>(obj));			
	sz++;
}

template <typename T, typename Alloc>
template <typename... Args>
void Conteiner<T, Alloc>::emplace_back(Args&&... args) {
	if (sz == capa) {
		reserve( std::max(std::size_t(1), 2 * sz) );
	}
	AllocTraits::construct(alloc, data + sz, T(std::forward<Args>(args)...));
	sz++;	
}









 
