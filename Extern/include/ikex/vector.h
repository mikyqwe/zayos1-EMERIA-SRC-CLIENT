#pragma once
#ifndef __INCLUDE_VECTOR_IKEX_HEADER__
#define __INCLUDE_VECTOR_IKEX_HEADER__

#include "std.h"

namespace ikex {

	//* optimized to be used as container of bytes (unsigned char) or any other small data type (short, int, long)
	//* not really efficient to be used with big data structure
	template <class value_t>
	class vector : public std::vector< value_t > {
	public:
		using vector_size_t = size_t;
		static const vector_size_t npos = (vector_size_t)-1;

		//* initializer ctor
		vector() {
		}

		//* copy ctor
		vector(const ikex::vector<value_t>& copy) {
			this->insert_back(copy);
		}

		vector(const std::vector< value_t >& copy) : std::vector< value_t >(copy) {
		}

		vector(const vector_size_t n, const value_t elm) :
			std::vector< value_t >(n, elm)
		{
		}

		template<class ...T>
		vector(const T... args) :
			std::vector< value_t >(args...)
		{
		}

		template <class ...T>
		void operator = (const T... args) {
			this->std::vector < value_t >::operator = (args...);
		}

		//* other ctors
		vector(const vector_size_t initial_size) {
			this->__reserve(initial_size);
		}

		template <const vector_size_t S>
		vector(value_t arr[S]) {
			this->insert_back(arr);
		}

		template <const vector_size_t S>
		void insert_back(const value_t arr[S]) {
			if (this->__free() < S)
				this->__reserve(this->size() + S);

			const auto old_size = this->size();
			this->resize(old_size + S);
			std::memcpy(this->data() + sizeof(value_t) * old_size, arr, sizeof(value_t) * S);
		}

		virtual vector& join(const ikex::vector<value_t>& another) {
			this->__reserve(this->size() + another.size());
			const auto old_size = this->size();
			this->resize(old_size + another.size());
			std::memcpy(this->data() + sizeof(value_t) * old_size, another.data(), sizeof(value_t) * another.size());
			return *this;
		}

		virtual void insert_back(const ikex::vector<value_t>& another) {
			this->__reserve(this->size() + another.size());
			const auto old_size = this->size();
			this->resize(old_size + another.size());
			std::memcpy(this->data() + sizeof(value_t) * old_size, another.data(), sizeof(value_t) * another.size());
		}

		virtual void erase_range(const vector_size_t beg, const vector_size_t end = npos) {
			const auto _max_index = this->size() - 1;
			const auto limit1 = beg > _max_index ? _max_index : beg;
			const auto limit2 = end > _max_index ? _max_index : end;
			const auto _beg   = limit1 > limit2 ? limit2 : limit1;
			const auto _end   = limit1 < limit2 ? limit2 : limit1;

			if (_beg == _end)
				this->erase(this->begin() + _beg);

			else if (_end == _max_index) {
				this->resize(_beg);
			}

			else {
				const auto from = this->data() + ((_end+1) * sizeof(value_t));
				const auto to = this->data() + (_beg * sizeof(value_t));
				const auto _remain_right = (_max_index ) - _end;
				std::memcpy(to, from, _remain_right);
				this->resize(_beg + _remain_right);
			}
		}

		virtual void pop_back(const vector_size_t count) {
			if (count == this->size())
				this->clear();

			else {
				this->erase_range(this->size() - count);
			}
		}

	protected:
		virtual void __reserve(const vector_size_t amount) {
			if (this->capacity() > amount)
				return;
			this->reserve(amount);
		}

		virtual vector_size_t __free() const {
			return this->capacity() - this->size();
		}


	};



}

#endif //__INCLUDE_VECTOR_IKEX_HEADER__