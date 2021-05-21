#ifndef __TRANSFORMER_RTTI_STDLIB_ARRAY_H__
#define __TRANSFORMER_RTTI_STDLIB_ARRAY_H__

#include <array>

namespace transformer::rtti
{

template<typename T, std::size_t N>
using Array = std::array<T, N>;

}

#endif // !__TRANSFORMER_RTTI_STDLIB_ARRAY_H__
