#ifndef __RTTI_VARIANT_H__
#define __RTTI_VARIANT_H__

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

/* Variant helpers to hide massive code in .cpp */

template<template<typename...> typename T>
bool variant_holds_alternative(const Variant& value) noexcept;

size_t         variant_array_size(const Variant& array) noexcept;
const Variant* variant_array_element(const Variant& array, size_t i) noexcept;

variant_type_name_t      variant_type_name(const Variant& value) noexcept;
variant_dump_as_string_t variant_dump_as_string(const Variant& value) noexcept;

}

#endif // !RTTI_DISABLE_DEFAULT_VARIANT

#endif // !__RTTI_VARIANT_H__
