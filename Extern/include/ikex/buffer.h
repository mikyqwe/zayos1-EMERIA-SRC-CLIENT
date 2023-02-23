#pragma once
#ifndef __INCLUDE_BUFFER_IKEX_HEADER__
#define __INCLUDE_BUFFER_IKEX_HEADER__

#include "std.h"
#include "vector.h"

namespace ikex {

	//* optimized to be used as container of bytes (unsigned char) or any other small data type (short, int, long)
	//* not really efficient to be used with big data structure
	template <class value_t>
	class __buffer_prototype : public ikex::vector< value_t >{
	public:
		using buffer_size_t = size_t;
		static constexpr buffer_size_t buffer_cell_size = 64;

		//* initializer ctor
		__buffer_prototype() {
			this->reserve(buffer_cell_size);
		}

		//* copy ctor
		__buffer_prototype(const __buffer_prototype<value_t>& copy) {
			this->reserve(copy.capacity());
			this->insert(copy);
		}

		__buffer_prototype(const ikex::vector< value_t >& copy) : ikex::vector< value_t >(copy){
		}

		__buffer_prototype(const buffer_size_t n, const value_t elm) :
			ikex::vector< value_t >(n,elm)
		{
		}

		template<class ...T>
		__buffer_prototype(const T... args):
			ikex::vector< value_t >(args...)
		{
		}

		template <class ...T>
		void operator = (const T... args) {
			this->ikex::vector < value_t >::operator = (args...);
		}

		//* other ctors
		__buffer_prototype(const buffer_size_t initial_size) {
			this->__reserve(initial_size);
		}

		template <const buffer_size_t S>
		__buffer_prototype( value_t arr[S]) {
			this->insert(arr);
		}

		template <const buffer_size_t S>
		void insert_back(const value_t arr[S]) {
			if (this->__free() < S)
				this->__reserve(this->size()+S);

			const auto old_size = this->size();
			this->resize( old_size + S);
			std::memcpy(this->data() + sizeof(value_t) * old_size, arr, sizeof(value_t) * S);
		}

		virtual void insert_back(const __buffer_prototype<value_t>& another) {
			this->__reserve(this->size() + another.size());
			const auto old_size = this->size();
			this->resize(old_size + another.size());
			std::memcpy(this->data() + sizeof(value_t) * old_size, another.data(), sizeof(value_t) * another.size());
		}

	protected:
		virtual void __reserve(const buffer_size_t amount) {
			const auto cells_count = (amount / buffer_cell_size) + 1;
			const auto ncapacity   = cells_count * buffer_cell_size;
			if (this->capacity() > ncapacity)
				return;
			this->reserve(ncapacity);
		}

		virtual buffer_size_t __free() {
			return this->capacity() - this->size();
		}


	};



}

#endif //__INCLUDE_BUFFER_IKEX_HEADER__