#include "rtti/DB.h"

#ifndef RTTI_DISABLE_DEFAULT_VARIANT
#	include <nlohmann/json.hpp>
#endif

namespace rtti
{

/* Implementation of 'DB::object_set_value_default' */

#ifndef RTTI_DISABLE_OBJECT_SET_AND_GET_VALUE_DEFAULT

/* static */ void DB::object_set_value_default(void* obj, const Variant& object, const ObjectTypeInfo* info) noexcept
{
	RTTI_ASSERT(object.type() == Variant::value_t::object, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
		variant_type_name(object), info->name, variant_dump_as_string(object)));

	const Variant::object_t* map = object.get_ptr<const Variant::object_t*>();
	for (const auto& [key, value]: *map) {
		auto field_iter = info->fields_info_map.find(key);
		RTTI_ASSERT(field_iter != info->fields_info_map.cend(),
			"Type '", info->name, "' does not have any field named '", key, '\'');

		field_iter->second->setter(obj, value);
	}
}

#endif // !RTTI_DISABLE_OBJECT_SET_AND_GET_VALUE_DEFAULT

/* Setters/getters for builtin types */

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

#define RTTI_BUILTIN_TYPE_VALUE_SET(builtin_type, variant_type_1, variant_type_2, variant_type_3) \
	template<> \
	/* static */ void DB::object_set_value<builtin_type>(builtin_type* obj, const Variant& value) noexcept \
	{ \
		if (value.type() == Variant::value_t::number_##variant_type_1) { \
			auto value_data = value.get_ptr<const Variant::number_##variant_type_1##_t*>(); \
			*obj = *value_data; \
		} else if (value.type() == Variant::value_t::number_##variant_type_2) { \
			auto value_data = value.get_ptr<const Variant::number_##variant_type_2##_t*>(); \
			*obj = *value_data; \
		} else if (value.type() == Variant::value_t::number_##variant_type_3) { \
			auto value_data = value.get_ptr<const Variant::number_##variant_type_3##_t*>(); \
			*obj = *value_data; \
		} else { \
			RTTI_ASSERT(false, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT( \
				variant_type_name(value), #builtin_type, variant_dump_as_string(value))); \
		} \
	}

/* Char (not signed and not unsigned, like 0) */
RTTI_BUILTIN_TYPE_VALUE_SET(char, integer, unsigned, float)

/* Signed integers */
RTTI_BUILTIN_TYPE_VALUE_SET(signed char,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_VALUE_SET(signed short,       integer, unsigned, float)
RTTI_BUILTIN_TYPE_VALUE_SET(signed int,         integer, unsigned, float)
RTTI_BUILTIN_TYPE_VALUE_SET(signed long,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_VALUE_SET(signed long long,   integer, unsigned, float)

/* Unsigned integers */
RTTI_BUILTIN_TYPE_VALUE_SET(unsigned char,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_VALUE_SET(unsigned short,     unsigned, integer, float)
RTTI_BUILTIN_TYPE_VALUE_SET(unsigned int,       unsigned, integer, float)
RTTI_BUILTIN_TYPE_VALUE_SET(unsigned long,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_VALUE_SET(unsigned long long, unsigned, integer, float)

/* Floats */
RTTI_BUILTIN_TYPE_VALUE_SET(float,  float, integer, unsigned)
RTTI_BUILTIN_TYPE_VALUE_SET(double, float, integer, unsigned)

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

/* Setters/getters for common STL types */

#ifndef RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

template<> /* static */ void DB::object_set_value<std::string>(std::string* obj, const Variant& value) noexcept
{
	auto value_data = value.get_ptr<const Variant::string_t*>();
	RTTI_ASSERT(value_data != nullptr, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
		variant_type_name(value), "std::string", value));

	*obj = *value_data;
}

#endif // !RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

}
