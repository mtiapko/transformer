#ifndef __TRANSFORMER_RTTI_CONCEPTS_UNQUALIFIED_TYPE_IS_SAME_AS_H__
#define __TRANSFORMER_RTTI_CONCEPTS_UNQUALIFIED_TYPE_IS_SAME_AS_H__

#include <type_traits>

namespace transformer::rtti
{

template<typename T, typename CompareWithT>
concept UnqualifiedTypeIsSameAs = std::is_same_v<
	std::remove_const_t<T>,
	std::remove_const_t<CompareWithT>
>;

}

#endif // !__TRANSFORMER_RTTI_CONCEPTS_UNQUALIFIED_TYPE_IS_SAME_AS_H__
