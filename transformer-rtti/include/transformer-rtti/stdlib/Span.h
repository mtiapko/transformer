#ifndef __TRANSFORMER_RTTI_STDLIB_SPAN_H__
#define __TRANSFORMER_RTTI_STDLIB_SPAN_H__

#include <span>

namespace transformer::rtti
{

template<typename T, size_t Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

}

#endif // !__TRANSFORMER_RTTI_STDLIB_SPAN_H__
