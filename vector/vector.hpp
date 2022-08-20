#pragma once
# include <memory>
# include <algorithm>
# include <cstddef>
# include <tgmath.h>
#include <iostream>
#include "../utils/iterator_traits.hpp"
#include "../utils/is_integral.hpp"
#include "../utils/enable_if.hpp"
#include "../utils/utils.hpp"
namespace ft{
    template <typename T, typename Alloc = std::allocator<T> >
    class vector{
        public:
            typedef T																value_type;
            typedef Alloc															allocator_type;
            typedef typename allocator_type::reference 								reference;
            typedef typename allocator_type::const_reference 						const_reference;
            typedef typename allocator_type::const_pointer 							const_pointer;
            typedef typename allocator_type::pointer 								pointer;
            typedef ft::random_access_iterator<value_type> 							iterator;
            typedef ft::random_access_iterator<const value_type> 					const_iterator;
            typedef ft::reverse_iterator<iterator> 									reverse_iterator;
            typedef ft::reverse_iterator<const_iterator> 							const_reverse_iterator;
            typedef typename ft::iterator_traits<iterator>::difference_type 		difference_type;
            typedef typename allocator_type::size_type 								size_type;

			explicit vector(const allocator_type &allocator = allocator_type()){
				_allocator = allocator;
				_data = nullptr;
				_capacity = 0;
				_size = 0;
			}//inition

			explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &allocator = allocator_type()){
				if (n < 0)
					throw std::bad_alloc();
				else	
					{
						_allocator = allocator;
						_capacity = n;
						_size = n;

						if (!n)
							_data = nullptr;
						else{
							_data = _allocator.allocate(n);
							for (size_type i = 0; i < n; i++)
							_allocator.construct(_data + i, val);
						}
					}
			}

			template<class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type &allocator = allocator_type(),
					typename std::enable_if <!ft::is_integral<InputIterator>::value>::type* = nullptr){
					_allocator = allocator;
					_size = ft::distance(first, last);
					_capacity = _size;
					_data = _allocator.allocate(_capacity);
					value_type *tempData = _data;
					for (InputIterator it = first; it != last; tempData++, it++)
						_allocator.construct(tempData, *it);
			}
			
			vector(const vector &x){
				_allocator = x._allocator;
				_size = x._size;
				_capacity = x._capacity;
				_data = _allocator.allocate(_capacity);
				for (size_type i = 0; i < _size; i++)
					_allocator.construct(_data + i, x[i]);
			}

			~vector(){
				for (size_type i = 0; i < _size; i++)
					_allocator.destroy(_data + i);
				_allocator.deallocate(_data, _capacity);
			}

			vector &operator=(const vector &x){
				_allocator.deallocate(_data, _capacity);
				_capacity = x._capacity;
				_size = x._size;
				_data = _allocator.allocate(_capacity);
				for (size_type i = 0; i < _size; i++)
					_allocator.construct(_data + i, x[i]);
				return *this;
			}

			iterator begin(){
				return &_data[0];
			}
			
			const_iterator begin() const{
				return &_data[0];
			}

			iterator end(){
				return &_data[_size];
			}

			const_iterator end() const{
				return &_data[_size];
			}

			reverse_iterator rbegin(){
				return reverse_iterator(end());
			}

			const_reverse_iterator rbegin()const{
				return reverse_iterator(end());
			}

			reverse_iterator rend(){
				return reverse_iterator(begin());
			}

			const_reverse_iterator rend()const{
				return reverse_iterator(begin());
			}

			size_type size() const{
				return _size;
			}

			size_type max_size()const{
				return _allocator.max_size();
			}

			void resize(size_type n, value_type val = value_type()){
				if (n < _size)
					for (; _size > n; _size--)
						_allocator.destroy(&_data[_size]);
				else if (n > _size && n < max_size()){
					if (n > _capacity && n <= _capacity*2)
						reserve(_capacity*2);
					else if (n > _capacity *2)
						reserve(n);
					for(; _size < n; _size++)
						_allocator.construct(&_data[_size], val);
				}
			}

			size_type capacity() const{
				return _capacity;
			}

			bool empty() const{
				return _size ? false : true;
			}

			void reserve(size_type n){
				if (n > _capacity && n < max_size()){
					pointer temp = _allocator.allocate(n);
					try
					{
						std::uninitialized_copy(_data, _data + _size, temp);
					}
					catch(...)
					{
						_allocator.deallocate(temp, n);
						throw;
					}
					_allocator.deallocate(_data, _capacity);
					_data = temp;
					_capacity = n;
				}
			}

			reference operator[](size_type n){
				return _data[n];
			}

			const_reference operator[](size_type n) const{
				return _data[n];
			}

			reference at(size_type n){
				if (n < _size)
					return _data[n];
				throw std::out_of_range("ERROR: position out of range");
			}
			const_reference at(size_type n) const{
				if (n < _size)
					return _data[n];
				throw std::out_of_range("ERROR: position out of range");
			}

			reference front(){
				return _data[0];
			}

			const_reference front()const{
				return _data[0];
			}

			reference back(){
				return _data[_size - 1];
			}

			const_reference back()const{
				return _data[_size - 1];
			}

			template<class InputIterator>
			void assign(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = nullptr){
				clear();
				size_type distance = ft::distance(first, last);
				if (_capacity < distance){
					if(_capacity * 2 >= distance)
						reserve(_capacity * 2);
					else
						reserve(distance);
				}
				for(; first < last; _size++, first++)
					_allocator.construct(_data + _size, *first);
			}

			void assign(size_type n, const value_type &val){
				clear();
				if (_capacity < n){
					if (_capacity < n){
						if (_capacity * 2 >= n)
							reserve(_capacity * 2);
						else
							reserve(n);
					}
					for(; _size < n; _size++)
						_allocator.construct(_data + _size, val);
				}
			}

			void pop_back(){
				if (_size){
					_allocator.destroy(&(*(end() - 1)));
					_size--;
				}
			}

			void push_back(const value_type &val){
				if (!_size)
					reserve(1);
				else if(_size == _capacity)
					reserve(_capacity * 2);
				_allocator.construct(&(*end()), val);
				_size++;
			}
			
			iterator insert(iterator position, const value_type &val){
				size_type dist = ft::distance(begin(), position);
				if (_size < _capacity){
					for (iterator it = end(); it != position - 1; it--)
						_allocator.construct(&(*position), val);
					_allocator.construct(&(*position), val);
					_size++;
				}
				else{
					value_type *temp_data = _allocator.allocate(_capacity ? _capacity *=2 : 1);
					size_type i = 0;
					for (iterator it = begin(); it != end(); it++){
						if (it == position){
							_allocator.construct(temp_data + i, val);
							i++;
						}
						_allocator.construct(temp_data + i, *it);
						_allocator.destroy(&(*it));
						i++;
					}
					_allocator.deallocate(_data, _capacity);
					_data = temp_data;
					_size++;
				}
				return (begin() + dist);
			}

			void insert(iterator position, size_type n, const value_type &val){
				if (_size + n > _capacity){
					if (_size + n >= _capacity*2)
						_capacity = _size + n;
					else
						_capacity *= 2;
				}
				value_type *temp_data = _allocator.allocate(_capacity);
				value_type *p = temp_data;
				for (int i = 0; begin() + i < position; i++){
					_allocator.construct(temp_data, *(begin() + i));
					temp_data++;
					_allocator.destroy(&(*(begin() + i)));
				}
				for (size_type i = 0; i < n; i++){
					_allocator.construct(temp_data, val);
					temp_data++;
				}
				for (int i = 0; position + i < end(); i++){
					_allocator.construct(temp_data, *(position + i));
					temp_data++;
					_allocator.destroy(&(*(position + i)));
				}
				_allocator.deallocate(&(*begin()), _size);
				_data = p;
				_size += n;
			}

			template<class InputIterator>
			void insert (iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0){
				size_type n = static_cast<size_type>(ft::distance(first, last));
				size_type distance = static_cast<size_type>(ft::distance(begin(), position));
				if (position > end() || position < begin())
					throw std::range_error("Index Error");
				value_type *temp_data = _allocator.allocate(n);
				try{
					for (size_type i = 0; i < n; i++)
						_allocator.construct(temp_data + i, *first++);
				}
				catch(...){
					for (size_type i = 0; temp_data + i != 0 && i < n; i++)
						_allocator.destroy(temp_data + i);
					_allocator.deallocate(temp_data, n);
					throw std::runtime_error("Error");
				}
				if (_capacity == _size && _capacity)
					reserve(_capacity * 2);
				if (_capacity < _size + n)
					reserve(_size + n);
				for (size_type i = 0; _size - i != distance; ++i){
					_allocator.construct(_data + _size - 1 - i + n,
										_data[_size - i - 1]);
					_allocator.destroy(_data + _size - i - 1);
				}
				for (size_type i = 0; i < n; i++) {
					_allocator.construct(_data + distance + i, temp_data[i]);
					_allocator.destroy(temp_data + i);
					_size++;
				}
				_allocator.deallocate(temp_data, n);
			}

			iterator erase(iterator first, iterator last){
				iterator temp = first;
				size_type distance = ft::distance(first, last);

				for (; first < end() - distance; first++){
					_allocator.destroy(&(*first));
					_allocator.construct(&(*first), *(first + distance));
					_allocator.destroy(&(*(first + distance)));
				}
				_size -= distance;
				return temp;
			}

			iterator erase(iterator position){
				if (position + 1 == end())
					_allocator.destroy(&(*position));
				else{
					for (int i = 0; position + i + 1 < end(); i++) {
						_allocator.construct(&(*(position + i)), *(position + i + 1));
						_allocator.destroy(&(*(position + i + 1)));
					}
					_size -= 1;
					return position;
				}
			}

			void clear(){
				size_type i = 0;
				while(i != _size){
					_allocator.destroy(_data + i);
					++i;
				}
				_size = 0;
			}
				
			allocator_type get_acllocator()const{
				return _allocator;
			}

			void swap(vector &x){
				ft::swap(_data, x._data);
				ft::swap(_capacity, x._capacity);
				ft::swap(_size, x._size);
			}
		private:
			allocator_type _allocator;
			value_type *_data;
			size_type _capacity;
			size_type _size;
	};

	template<class T, class Alloc>
	bool operator==(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc>&rhs){
		if (lhs.size() != rhs.size())
			return false;
		if(ft::equal(lhs.begin(), lhs.end(), rhs.begin()))
			return true;
		return false;}

	template<class T, class Alloc>
	bool operator!=(const ft::vector<T, Alloc>&lhs, const ft::vector<T, Alloc> &rhs){
		return !(lhs == rhs);}

	template <class T, class Alloc>
	bool operator<(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs){
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());}

	template <class T, class Alloc>
	bool operator>(const ft::vector <T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs){
		return (rhs < lhs);
	}

	template <class T, class Alloc>
	bool operator<=(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs){
		return (!(rhs < lhs));
	}

	template<class T, class Alloc>
	bool operator>=(const ft::vector <T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs){
		return (!(lhs < rhs));
	}

	template<class T, class Alloc>
	void swap(ft::vector<T, Alloc>&x, ft::vector<T, Alloc>&y){
		x.swap(y);
	}
}
