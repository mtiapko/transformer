#include "rtti/Variant.h"
#include "rtti/DB.h"

#ifndef RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

namespace rtti
{

template<> /* static */ void DB::object_set_value<std::string>(std::string* obj, const Variant& value) noexcept
{
	auto value_data = value.get_ptr<const Variant::string_t*>();
	RTTI_ASSERT(value_data != nullptr, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
		variant_type_name(value), "std::string", value));

	*obj = *value_data;
}

template<> /* static */ void DB::object_get_value<std::string>(const std::string* obj, Variant& value) noexcept
{
	value = *obj;
}

}

#endif // !RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE
