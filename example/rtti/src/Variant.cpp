#include "rtti/Variant.h"

#if !defined(RTTI_DISABLE_DEFAULT_VARIANT) \
	&& !defined(RTTI_DISABLE_DEFAULT_VARIANT_HELPERS)

namespace rtti
{

static_assert(std::is_same_v<variant_object_map_t, Variant::object_t>
		&& std::is_same_v<variant_array_vector_t, Variant::array_t>,
	"nlohmann/json implementation changed, 'variant_object_map_t' has wrong type now");

template<>
bool variant_holds_alternative<variant_array_t>(const Variant& value) noexcept
{
	return value.is_array();
}

void variant_make_array(Variant& value) noexcept
{
	if (!value.is_array())
		value = variant_array_vector_t {};
}

size_t variant_array_size(const Variant& array) noexcept
{
	return array.size();
}

void variant_array_reserve(Variant& array, size_t size) noexcept
{
	array.get_ref<variant_array_vector_t&>().reserve(size);
}

const Variant& variant_array_element(const Variant& array, size_t i) noexcept
{
	return array[i];
}

Variant& variant_array_emplace_back(Variant& array) noexcept
{
	return array.emplace_back();
}

variant_type_name_t variant_type_name(const Variant& value) noexcept
{
	return value.type_name();
}

variant_dump_as_string_t variant_dump_as_string(const Variant& value) noexcept
{
	return value.dump();
}

}

#endif // !RTTI_DISABLE_DEFAULT_VARIANT && !RTTI_DISABLE_DEFAULT_VARIANT_HELPERS
