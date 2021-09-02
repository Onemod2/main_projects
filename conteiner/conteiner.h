#include <cstdlib>
#include <algorithm>
#include <memory>
#include <exception>

// Main class of conteiner that contein any type of data
//

template <typename T, typename Alloc = std::allocator<T>>
class Conteiner {
	private:
		template <typename ValueType>
		class Iterator;

	public:
		using iterator = Iterator<T>;
		using const_iterator = Iterator<const T>;

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

		iterator begin() {
			return iterator(data);
		}
		
		iterator end() {
			return iterator(data + sz);
		}

		const_iterator cbegin() const {
			return const_iterator(data);
		}
		
		const_iterator cend() const {
			return const_iterator(data + sz);
		}
	
	private:
		template <typename ValueType>
		class Iterator 
		:	public std::iterator<std::random_access_iterator_tag, ValueType> {
		
			public:
				Iterator() {};	
				Iterator(ValueType* ptr) : ptr(ptr) {}
				Iterator(const Iterator& it) : ptr(it.ptr) {}

				ValueType& operator*() {
					return *ptr;	
				}
				
				Iterator operator++(int) {
					Iterator tmp = *this;
					ptr++;
					return tmp;	
				}

				Iterator& operator++() {
					ptr++;
					return *this;	
				}

				Iterator operator--(int) {
					Iterator tmp = *this;
					ptr--;
					return tmp;	
				}

				Iterator& operator--() {
					ptr--;
					return *this;	
				}

				T& operator->() {
					return *ptr;
				}

				Iterator operator+(int i) const {
					return Iterator(ptr + i);
				}

				Iterator operator-(int i) const {
					return Iterator(ptr - i);
				}

				bool operator==(const Iterator& it) const {
					return this->ptr == it.ptr;	
				}

				bool operator!=(const Iterator& it) const {
					return !(*this == it);	
				}	

			private:
				ValueType* ptr = nullptr;
		};


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
			AllocTraits::construct(
				alloc, new_arr + i, std::move_if_noexcept(data[i]));	
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
	AllocTraits::construct(
			alloc, data + sz, T(std::forward<Args>(args)...));
	
	sz++;	
}

