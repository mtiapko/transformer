#include "rtti/Variant.h"

#if !defined(RTTI_DISABLE_DEFAULT_VARIANT) \
	&& !defined(RTTI_DISABLE_DEFAULT_VARIANT_HELPERS)

#include <nlohmann/json.hpp>

namespace rtti
{

template<>
bool variant_holds_alternative<variant_array_t>(const Variant& value) noexcept
{
	return (value.type() == Variant::value_t::array);
}

variant_type_name_t variant_type_name(const Variant& value) noexcept
{
	return value.type_name();
}

variant_dump_as_string_t variant_dump_as_string(const Variant& value) noexcept
{
	return value.dump();
}

size_t variant_array_size(const Variant& array) noexcept
{
	return array.size();
}

const Variant* variant_array_element(const Variant& array, size_t i) noexcept
{
	if (array.size() <= i)
		return nullptr;

	return &array[i];
}

}

#endif // !RTTI_DISABLE_DEFAULT_VARIANT && !RTTI_DISABLE_DEFAULT_VARIANT_HELPERS
