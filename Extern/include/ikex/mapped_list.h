#pragma once
#ifndef __INCLUDE_MAPPED_LIST_IKEX_HEADER__
#define __INCLUDE_MAPPED_LIST_IKEX_HEADER__
#include "mapped_prototype.h"

namespace ikex {
	template <class key_t , class value_t>
	using mapped_list = __prototype_mapped_list<key_t, value_t>;

}

#endif //__INCLUDE_MAPPED_LIST_IKEX_HEADER__