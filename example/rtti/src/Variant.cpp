#include "rtti/Variant.h"

#if !defined(RTTI_DISABLE_DEFAULT_VARIANT) \
	&& !defined(RTTI_DISABLE_DEFAULT_VARIANT_HELPERS)

namespace rtti
{

#ifndef RTTI_DISABLE_DEFAULT_CONFIG

static_assert(std::is_same_v<variant_object_map_t, Variant::object_t>
		&& std::is_same_v<variant_array_vector_t, Variant::array_t>,
	"nlohmann/json implementation changed, 'variant_object_map_t' has wrong type now");

#endif // !RTTI_DISABLE_DEFAULT_CONFIG

template<>
bool variant_holds_alternative<variant_array_t>(const Variant& value) noexcept
{
	return (value.type() == Variant::value_t::array);
}

variant_array_vector_t& variant_make_array(Variant& value) noexcept
{
	if (!value.is_array())
		value = variant_array_vector_t {};

	return value.get_ref<variant_array_vector_t&>();
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

Variant* variant_array_element(Variant& array, size_t i) noexcept
{
	return &array[i];
}

const Variant* variant_array_element(const Variant& array, size_t i) noexcept
{
	if (array.size() <= i)
		return nullptr;

	return &array[i];
}

}

#endif // !RTTI_DISABLE_DEFAULT_VARIANT && !RTTI_DISABLE_DEFAULT_VARIANT_HELPERS
