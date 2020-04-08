#ifndef __RTTI_DB_STL_TYPES_DEFAULT_SETTER_GETTER_H__
#define __RTTI_DB_STL_TYPES_DEFAULT_SETTER_GETTER_H__

#include "rtti/DB/Log.h"
#include "rtti/Variant.h"
#include "rtti/DB.h"

namespace rtti
{

/* * * * * * * * * * * * *
 *                       *
 *      std::vector      *
 *                       *
 * * * * * * * * * * * * */

template<typename T>
/* static */ void DB::variable_set_value(std::vector<T>* vec, const Variant& array_value) noexcept
{
	auto array_value_ptr = array_value.get_ptr<const Variant::array_t*>();
	RTTI_ASSERT(array_value_ptr != nullptr, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT("std::vector", array_value));

	vec->clear();
	vec->reserve(array_value_ptr->size());

	for (const Variant& value: *array_value_ptr)
		variable_set_value<T>(&vec->emplace_back(), value);
}

template<typename T>
/* static */ void DB::variable_get_value(const std::vector<T>* vec, Variant& array_value) noexcept
{
	if (!array_value.is_array())
		array_value = Variant::array_t {};

	auto array_value_ptr = array_value.get_ptr<Variant::array_t*>();

	array_value_ptr->clear();
	array_value_ptr->reserve(vec->size());

	for (const T& value: *vec)
		array_value_ptr->emplace_back(value);
}

/* * * * * * * * * * * * *
 *                       *
 *      std::string      *
 *                       *
 * * * * * * * * * * * * */

/* static */ void DB::variable_set_value(std::string* str, const Variant& string_value) noexcept
{
	auto string_value_ptr = string_value.get_ptr<const Variant::string_t*>();
	RTTI_ASSERT(string_value_ptr != nullptr, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT("std::string", string_value));

	*str = *string_value_ptr;
}

/* static */ void DB::variable_get_value(const std::string* str, Variant& string_value) noexcept
{
	string_value = *str;
}

}

#endif // !__RTTI_DB_STL_TYPES_DEFAULT_SETTER_GETTER_H__
