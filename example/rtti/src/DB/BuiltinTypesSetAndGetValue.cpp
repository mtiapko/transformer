#include "rtti/Variant.h"
#include "rtti/DB.h"

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

#define RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(builtin_type, variant_type_1, variant_type_2, variant_type_3) \
	template<> \
	/* static */ void DB::object_set_value<builtin_type>(builtin_type* obj, const Variant& value) noexcept \
	{ \
		if (value.is_number_##variant_type_1()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_1##_t*>(); \
			*obj = *value_data; \
		} else if (value.is_number_##variant_type_2()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_2##_t*>(); \
			*obj = *value_data; \
		} else if (value.is_number_##variant_type_3()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_3##_t*>(); \
			*obj = *value_data; \
		} else if (value.is_null()) { \
			*obj = {}; \
		} else { \
			RTTI_ASSERT(false, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT( \
				variant_type_name(value), #builtin_type, variant_dump_as_string(value))); \
		} \
	} \
	\
	template<> \
	/* static */ void DB::object_get_value<builtin_type>(const builtin_type* obj, Variant& value) noexcept \
	{ \
		value = *obj; \
	}

namespace rtti
{

/* Char (not signed and not unsigned, like 0) */
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(char, integer, unsigned, float)

/* Bool */
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(bool, integer, unsigned, float)

/* Signed integers */
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(signed char,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(signed short,       integer, unsigned, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(signed int,         integer, unsigned, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(signed long,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(signed long long,   integer, unsigned, float)

/* Unsigned integers */
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(unsigned char,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(unsigned short,     unsigned, integer, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(unsigned int,       unsigned, integer, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(unsigned long,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(unsigned long long, unsigned, integer, float)

/* Floats */
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(float,  float, integer, unsigned)
RTTI_BUILTIN_TYPE_SET_AND_GET_VALUE(double, float, integer, unsigned)

}

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
