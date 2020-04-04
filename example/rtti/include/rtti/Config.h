#ifndef __RTTI_CONFIG_H__
#define __RTTI_CONFIG_H__

#include <vector>
#include <string_view>
#include <unordered_map>
#include <map>

#include "rtti/Log.h"

#ifdef RTTI_EXTERNAL_CONFIG_PATH
#	include RTTI_EXTERNAL_CONFIG_PATH
#endif

#ifndef RTTI_NOT_USE_DEFAULT_CONFIG

namespace rtti
{

/* VariantConfig.h */
template<typename U, typename V, typename... Args>
using variant_object_t   = std::map<U, V, Args...>;

template<typename U, typename... Args>
using variant_array_t    = std::vector<U, Args...>;

using variant_string_t   = std::string;

using variant_bool_t     = bool;
using variant_integer_t  = int64_t;
using variant_unsigned_t = uint64_t;
using variant_float_t    = double;

/* TypeInfo config */
using type_info_name_t = std::string_view;

/* ObjectFieldTypeInfo config */
using object_field_type_info_get_addr_t             = void* (*)(void*) noexcept;
using object_field_type_info_get_object_type_info_t = const struct ObjectTypeInfo* (*)() noexcept;

/* ObjectTypeInfo config */
using object_type_info_fields_info_t     = std::vector<struct ObjectFieldTypeInfo>;
using object_type_info_fields_info_map_t = std::unordered_map<
	type_info_name_t,
	const struct ObjectFieldTypeInfo*
>;

}

#endif // !RTTI_NOT_USE_DEFAULT_CONFIG

#endif // !__RTTI_CONFIG_H__
