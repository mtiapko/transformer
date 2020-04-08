#ifndef __RTTI_DB_ARRAY_TYPES_DEFAULT_SETTER_GETTER_H__
#define __RTTI_DB_ARRAY_TYPES_DEFAULT_SETTER_GETTER_H__

#include "rtti/DB/Log.h"
#include "rtti/Variant.h"
#include "rtti/DB.h"

namespace rtti
{

// TODO(FiTH): add multidimensional array support
// TODO(FiTH): add char[] support?

template<typename T, size_t N>
/* static */ void DB::variable_set_value(T (&arr)[N], const Variant& array_value) noexcept
{
	RTTI_ASSERT(array_value.is_array(), RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(
		RTTI_PRINT_TUPLE("<builtin array [", N, "]>"), array_value));

	auto array_value_ptr = array_value.get_ptr<const Variant::array_t*>();

	RTTI_WARN_ASSERT(array_value_ptr->size() == N, "Array variable [", N, "] and array value [",
		array_value_ptr->size(), "] have different sizes: ", array_value);

	size_t min_size = std::min(array_value_ptr->size(), N);

	for (size_t i = 0; i < min_size; ++i)
		variable_set_value<T>(arr + i, array_value[i]);
}

template<typename T, size_t N>
/* static */ void DB::variable_get_value(const T (&arr)[N], Variant& array_value) noexcept
{
	if (!array_value.is_array())
		array_value = Variant::array_t {};

	auto array_value_ptr = array_value.get_ptr<Variant::array_t*>();

	array_value_ptr->clear();
	array_value_ptr->reserve(N);

	for (const T& value: arr)
		array_value_ptr->emplace_back(value);
}

}

#endif // !__RTTI_DB_ARRAY_TYPES_DEFAULT_SETTER_GETTER_H__
