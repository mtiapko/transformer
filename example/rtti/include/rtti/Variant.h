#ifndef __RTTI_VARIANT_H__
#define __RTTI_VARIANT_H__

#include "nlohmann/json_fwd.hpp"
#include "rtti/Config.h"

namespace rtti
{

using Variant = nlohmann::basic_json<
	variant_object_t,
	variant_array_t,
	variant_string_t,
	variant_bool_t,
	variant_integer_t,
	variant_unsigned_t,
	variant_float_t
>;

}

#endif // !__RTTI_VARIANT_H__
