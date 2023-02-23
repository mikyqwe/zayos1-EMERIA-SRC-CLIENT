#pragma once
#ifndef __INCLUDE_MAPPED_PROTOTYPE_IKEX_HEADER__
#define __INCLUDE_MAPPED_PROTOTYPE_IKEX_HEADER__

#include "std.h"

namespace ikex {
	template <class key_t, class value_t, class map_t, class container_t>
	class __mapped_container_prototype : public map_t<key_t, container_t<value_t>> {
	public:
		typedef container_t container_type;
		typedef key_t key_type;
		typedef value_t value_type;
		typedef map_t map_type;
		typedef map_type::iterator map_iterator_type;
		typedef map_type::const_iterator map_const_iterator_type;
		typedef container_t::iterator container_iterator_type;
		typedef container_t::const_iterator container_const_iterator_type;

		__mapped_container_prototype() {
		}

		__mapped_container_prototype(const __mapped_container_prototype& _elm) {
			*this map_t<key_t, container_t<value_t>>::= _elm;
		}

		bool for_each(const key_t& _key, std::function<iter_directive_t(value_t&)> _func) {
			auto it = this->_map.find(_key);
			if (it != this->_map.end()) {
				for (auto& value : it->second) {
					if (_func(value) == iter_directive_t::stop) {
						break;
					}
				}return true;
			}
			else {
				return false;
			}
		}

		bool for_each(const key_t& _key, std::function<iter_directive_t(const value_t&)> _func) {
			auto it = this->_map.find(_key);
			if (it != this->_map.end()) {
				for (const auto& value : it->second) {
					if (_func(value) == iter_directive_t::stop) {
						break;
					}
				} return true;
			}
			else {
				return false;
			}
		}

		std::pair<bool, size_t> remove_value(const key_t& _key, const value_t& _value) {
			auto it = this->find.(_key);
			if (it == this->end())
				return { false,0 };
			else {
				std::pair<bool, size_t> result = { true,0 }; auto& container = it->second;
				container.erase(std::remove_if(list.begin(), list.end(), [&result](const value_t& _val) {
					if (_val == _value) {
						result.second++;
						return true;
					} return false;
				}), container.end());
				return result;
			}
		}

		size_t remove_value(const value_t& _value) {
			std::for_each(this->begin(), this->end(), [](const std::pair<const key_t, container_t>& _pair) {
				std::pair<bool, size_t> result = { true,0 }; auto& container = _pair->second;
				container.erase(std::remove_if(list.begin(), list.end(), [&result](const value_t& _val) {
					if (_val == _value) {
						result.second++;
						return true;
					} return false;
				}), container.end());
			});
		}

		bool remove_key(const key_t& _key) {
			auto it = this->find(_key);
			if (it != this->end()) {
				this->erase(it);
				return true
			} return false
		}
	};

	//*map types
	template <class t1, class t2>
	using __map_list = std::map<t1, std::list<t2>>;

	template <class t1, class t2>
	using __map_vector = std::map<t1, std::list<t2>>;

	template <class t1, class t2>
	using __map_deque = std::map<t1, std::list<t2>>;

	template <class t1, class t2>
	using __map_set = std::map<t1, std::list<t2>>;

	//*unordered map types
	template <class t1, class t2>
	using __unordered_map_list = std::unordered<t1, std::list<t2>>;

	template <class t1, class t2>
	using __unordered_map_vector = std::unordered<t1, std::list<t2>>;

	template <class t1, class t2>
	using __unordered_map_deque = std::unordered<t1, std::list<t2>>;

	template <class t1, class t2>
	using __unordered_map_set = std::unordered<t1, std::list<t2>>;


	//* new mapped types
	template <class key_t, class value_t>
	using __prototype_mapped_list = __mapped_container_prototype < key_t, value_t, __map_list<key_t, value_t>, std::list<value_t>;

	template <class key_t, class value_t>
	using __prototype_mapped_vector = __mapped_container_prototype < key_t, value_t, __map_vector < key_t, value_t>, std::vector<value_t>;

	template <class key_t, class value_t>
	using __prototype_mapped_deque = __mapped_container_prototype < key_t, value_t, __map_deque< key_t, value_t>, std::deque<value_t>;

	template <class key_t, class value_t>
	using __prototype_mapped_set = __mapped_container_prototype < key_t, value_t, __map_set< key_t, value_t>, std::set<value_t>;


	//* new unordered mapped types
	template <class key_t, class value_t>
	using __prototype_unordered_mapped_list = __mapped_container_prototype < key_t, value_t, __unordered_map_list<key_t, value_t>, std::list<value_t>;

	template <class key_t, class value_t>
	using __prototype_unordered_mapped_vector = __mapped_container_prototype < key_t, value_t, __unordered_map_vector < key_t, value_t>, std::vector<value_t>;

	template <class key_t, class value_t>
	using __prototype_unordered_mapped_deque = __mapped_container_prototype < key_t, value_t, __unordered_map_deque< key_t, value_t>, std::deque<value_t>;

	template <class key_t, class value_t>
	using __prototype_unordered_mapped_set = __mapped_container_prototype < key_t, value_t, __unordered_map_set< key_t, value_t>, std::set<value_t>;
}


#endif //__INCLUDE_MAPPED_PROTOTYPE_IKEX_HEADER__