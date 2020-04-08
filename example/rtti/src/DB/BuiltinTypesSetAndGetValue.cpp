#include "rtti/Config.h"

#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER

#include "rtti/DB/Log.h"
#include "rtti/Variant.h"
#include "rtti/DB.h"

#define RTTI_BUILTIN_TYPE_SETTER_GETTER(builtin_type, variant_type_1, variant_type_2, variant_type_3) \
	template<> \
	/* static */ void DB::variable_set_value<builtin_type>(builtin_type* obj, const Variant& value) noexcept \
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
			RTTI_ASSERT(false, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(#builtin_type, value)); \
		} \
	} \
	\
	template<> \
	/* static */ void DB::variable_get_value<builtin_type>(const builtin_type* obj, Variant& value) noexcept \
	{ \
		value = *obj; \
	}

namespace rtti
{

/* Char (not signed and not unsigned, like 0) */
RTTI_BUILTIN_TYPE_SETTER_GETTER(char, integer, unsigned, float)

/* Bool */
RTTI_BUILTIN_TYPE_SETTER_GETTER(bool, integer, unsigned, float)

/* Signed integers */
RTTI_BUILTIN_TYPE_SETTER_GETTER(signed char,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(signed short,       integer, unsigned, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(signed int,         integer, unsigned, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(signed long,        integer, unsigned, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(signed long long,   integer, unsigned, float)

/* Unsigned integers */
RTTI_BUILTIN_TYPE_SETTER_GETTER(unsigned char,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(unsigned short,     unsigned, integer, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(unsigned int,       unsigned, integer, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(unsigned long,      unsigned, integer, float)
RTTI_BUILTIN_TYPE_SETTER_GETTER(unsigned long long, unsigned, integer, float)

/* Floats */
RTTI_BUILTIN_TYPE_SETTER_GETTER(float,  float, integer, unsigned)
RTTI_BUILTIN_TYPE_SETTER_GETTER(double, float, integer, unsigned)

}

#endif // !RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
