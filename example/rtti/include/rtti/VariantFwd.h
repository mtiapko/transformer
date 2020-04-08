#ifndef __RTTI_VARIANT_FWD_H__
#define __RTTI_VARIANT_FWD_H__

#include "rtti/Config.h"

#ifndef RTTI_DISABLE_DEFAULT_VARIANT

#include <nlohmann/json_fwd.hpp>

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

#endif // !RTTI_DISABLE_DEFAULT_VARIANT

#endif // !__RTTI_VARIANT_FWD_H__
