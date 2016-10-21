#ifndef stack_cpp
#define stack_cpp
#pragma once
#include <iostream>
//using namespace std;



template <typename T1, typename T2>
void construct(T1 * ptr, T2 const & value) {
	new(ptr)T1(value);
}

template <typename T>
void destroy(T * ptr) noexcept
{
	ptr->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept
{
	for (; first != last; ++first) {
		destroy(&*first);
	}
}



template <typename T>
class allocator{
protected:
	allocator(size_t size = 0);
	~allocator();
	auto swap(allocator & other) -> void;
	allocator(allocator const &) = delete;
	auto operator =(allocator const &)->allocator & = delete;
	T * ptr_;
	size_t size_;
	size_t count_;
};

template <typename T>
allocator<T>::allocator(size_t size) : ptr_((T*)(operator new(size*sizeof(T)))), size_(size), count_(0){};

template<typename T>
allocator<T>::~allocator(){ operator delete(ptr_); }

template<typename T>
auto allocator<T>::swap(allocator & other)->void{
	std::swap(ptr_, other.ptr_);
	std::swap(size_, other.size_);
	std::swap(count_, other.count_);
}



template <typename T>
class stack : private allocator<T>
{
public:
	stack(size_t size=0); /* noexcept */
	stack(const stack&); /* strong */
	~stack(); /* noexcept */
	size_t count() const noexcept; /* noexcept */
	void push(T const &); /* strong */
	const T& top(); /* strong */
	void pop(); /* strong */
	stack& operator=(const stack&); /* strong */
	bool empty() noexcept; /* noexcept */
};

/*template<typename T>
T* copy_mas(const T *p1, size_t c, size_t s){
T *p2 = (T*)(operator new(s*sizeof(T)));
try{ std::copy(p1, p1 + c, p2); }
catch (...){ destroy(p2, p2 + s); operator delete(p2); throw; }
return p2;
}*/

template <typename T>
stack<T>::stack(size_t size) : allocator<T>(size){};

template <typename T>// + 
stack<T>::stack(const stack& x){
	allocator<T>::count_ = tmp.count_;
	allocator<T>::ptr_ = mem_copy(tmp.count_, tmp.size_, tmp.ptr_);
	allocator<T>::size_ = tmp.size_;
};

template <typename T>//+
stack<T>::~stack(){ destroy(allocator<T>::ptr_, allocator<T>::ptr_ + allocator<T>::count_); }

template <typename T>//+
size_t stack<T>::count()const noexcept { return allocator<T>::count_; }

template <typename T>//добавилась ф-я мем_копи для копирования вместо цикла //+
auto mem_copy(size_t count_m, size_t array_size_m, const T * tmp)->T* {
	T *mass = new T[array_size_m];
	try {
	std::copy(tmp, tmp + count_m, mass);}
	catch (...){
	delete[] mass;
		throw;
	}
	return mass;
}
template <typename T>//+
void stack<T>::push(T const &a){
	if (allocator<T>::count_ == allocator<T>::size_) {
		size_t array_size = allocator<T>::size_ * 2 + (allocator<T>::size_ == 0);
		T *tmp = mem_copy(allocator<T>::count_, array_size, allocator<T>::ptr_);
		delete[] allocator<T>::ptr_;
		allocator<T>::ptr_ = tmp;
		allocator<T>::size_ = array_size;
		
	}
	construct(allocator<T>::ptr_ + allocator<T>::count_, a);
	++allocator<T>::count_;
}

template <typename T>//+
const T& stack<T>::top(){
	if (allocator<T>::count_ > 0) return allocator<T>::ptr_[allocator<T>::count_ - 1];
	else throw("stack pust");
}

template <typename T>//+
void stack<T>::pop(){//сначала очистка памяти затем уменьшение count_
	if (allocator<T>::count_ > 0) { destroy(&(allocator<T>::ptr_[allocator<T>::count_]));  --allocator<T>::count_; }
	else throw("stack pust");
}

template<typename T>//+
stack<T>& stack<T>::operator=(const stack& b){
	if (this != &b){
		T* cp = mem_copy(tmp.count_, tmp.size_, tmp.ptr_);
		delete[] allocator<T>::ptr_; 
		allocator<T>::ptr_ = cp;
		allocator<T>::size_ =tmp.size_;
		allocator<T>::count_ = tmp.count_;
	}
	return *this;
}

template<typename T>//+
bool stack<T>::empty() noexcept { return(allocator<T>::count_ == 0); }

#endif
