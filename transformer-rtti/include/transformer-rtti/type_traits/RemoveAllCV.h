#ifndef __TRANSFORMER_RTTI_TYPE_TRAITS_REMOVE_ALL_CV_H__
#define __TRANSFORMER_RTTI_TYPE_TRAITS_REMOVE_ALL_CV_H__

#include <type_traits> // TODO(FiTH): replace this?

namespace transformer::rtti::type_traits
{

template<typename T>
consteval auto remove_all_cv() noexcept
{
	static_assert(std::is_reference_v<T> == false);

	if constexpr (std::is_pointer_v<T>) {
		using next_type = decltype(remove_all_cv<std::remove_pointer_t<T>>());
		return std::type_identity<typename next_type::type*> {};
	} else if constexpr (std::is_array_v<T>) {
		using next_type = decltype(remove_all_cv<std::remove_extent_t<T>>());
		return std::type_identity<typename next_type::type[std::extent_v<T>]> {};
	} else {
		return std::type_identity<std::remove_cv_t<T>> {};
	}
}

template<typename T>
using remove_all_cv_t = typename decltype(remove_all_cv<T>())::type;

}

#endif // !__TRANSFORMER_RTTI_TYPE_TRAITS_REMOVE_ALL_CV_H__
