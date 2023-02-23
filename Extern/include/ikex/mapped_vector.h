#pragma once
#ifndef __INCLUDE_MAPPED_VECTOR_IKEX_HEADER__
#define __INCLUDE_MAPPED_VECTOR_IKEX_HEADER__

#include "mapped_prototype.h"

namespace ikex {
	template <class key_t, class value_t>
	class mapped_vector : public __prototype_mapped_vector<key_t, value_t> {

	};
}

#endif //__INCLUDE_MAPPED_VECTOR_IKEX_HEADER__