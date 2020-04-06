#ifndef __RTTI_CONFIG_H__
#define __RTTI_CONFIG_H__

/*
	TODO(FiTH): allow disable only part of default config?
		ex.: RTTI_DISABLE_DEFAULT_VARIANT_CONFIG or RTTI_DISABLE_DEFAULT_TYPE_INFO_CONFIG?

	TODO(FiTH): enable/disable RTTI_LOG/RTTI_ASSERT printing file/func/line info

	Used defines:
		* RTTI_DISABLE_DEFAULT_CONFIG
			Disable default config (default types).
		* RTTI_DISABLE_DEFAULT_VARIANT
			Disable default Variant that using <nlohmann/json.hpp>. User must provide another Variant.
		* RTTI_DISABLE_DEFAULT_VARIANT_HELPERS
			Disable default Variant helpers.
		* RTTI_DISABLE_LOG
			Disable any logging.
		* RTTI_DISABLE_ASSERT
			Disable asserts.
		* RTTI_DISABLE_ASSERT_EXIT_ON_FAIL
			Disable 'std::exit' call when assert fails, just use 'return'.
		* RTTI_DISABLE_ASSERT_PRINT
			Disable printing 'assert' information.
		* RTTI_DISABLE_WARN_ASSERT
			Disable warning asserts.
		* RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT
			Disable 'DB::object_set_value_default' method.
		* RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
			Disable default setters/getters for builtin types (int, float, ...)
		* RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE
			Disable default setters/getters for common STL types (string, vector, ...).

*/

// TODO(FiTH): remove after debug

// #define RTTI_DISABLE_DEFAULT_CONFIG
// #define RTTI_DISABLE_DEFAULT_VARIANT
// #define RTTI_DISABLE_DEFAULT_VARIANT_HELPERS

// #define RTTI_DISABLE_LOG
// #define RTTI_DISABLE_ASSERT
#define RTTI_DISABLE_ASSERT_EXIT_ON_FAIL
// #define RTTI_DISABLE_ASSERT_PRINT
// #define RTTI_DISABLE_WARN_ASSERT
// #define RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT
// #define RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
// #define RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

/* External config that can override or extend default config */
#ifdef RTTI_EXTERNAL_CONFIG_PATH
#	include RTTI_EXTERNAL_CONFIG_PATH
#endif

/* Some helper defines */
#define RTTI_UNUSED(x) (void)x

/* Default config */
#ifndef RTTI_DISABLE_DEFAULT_CONFIG

#include <vector>
#include <string_view>
#include <unordered_map>
#include <map>

namespace rtti
{

/* Variant config */
template<typename U, typename V, typename... Args>
using variant_object_t   = std::map<U, V, Args...>;

template<typename U, typename... Args>
using variant_array_t    = std::vector<U, Args...>;

using variant_string_t   = std::string;

using variant_bool_t     = bool;
using variant_integer_t  = int64_t;
using variant_unsigned_t = uint64_t;
using variant_float_t    = double;

/* Variant helpers config */
using variant_type_name_t      = std::string_view;
using variant_dump_as_string_t = std::string;

/* TypeInfo config */
using type_info_name_t      = std::string;
using type_info_name_view_t = std::string;
// TODO(FiTH): make it std::string_view when std::unordered_map will support '.find' by string_view

/* ObjectFieldTypeInfo config */
using object_field_type_info_field_name_t           = std::string;
using object_field_type_info_get_addr_t             = void* (*)(void*) noexcept;
using object_field_type_info_get_object_type_info_t = const struct ObjectTypeInfo* (*)() noexcept;

/* ObjectTypeInfo config */
using object_type_info_fields_info_t     = std::vector<struct ObjectFieldTypeInfo>;
using object_type_info_fields_info_map_t = std::unordered_map<
	type_info_name_t,
	const struct ObjectFieldTypeInfo*
>;

}

#endif // !RTTI_DISABLE_DEFAULT_CONFIG

#endif // !__RTTI_CONFIG_H__
