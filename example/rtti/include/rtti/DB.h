#ifndef __RTTI_DB_H__
#define __RTTI_DB_H__

#include "rtti/ObjectTypeInfo.h"
#include "rtti/Assert.h"

#define RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(value_type, variable_type, value) \
	"Failed to set value of type '", value_type, "' for variable of type '", variable_type, "': ", value

namespace rtti
{

class DB
{
private:
#ifndef RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT
	static void object_set_value_default(void* obj, const Variant& value, const ObjectTypeInfo* info) noexcept;
	static void object_get_value_default(const void* obj, Variant& value, const ObjectTypeInfo* info) noexcept;
#endif

public:
	template<typename T>
	static ObjectTypeInfo make_object_type_info() noexcept;

	template<typename T>
	static const ObjectTypeInfo* get_object_type_info() noexcept
	{
		return nullptr;
	}

	// TODO(FiTH): add comments for blocks

	template<typename T>
	static void object_set_value(T* obj, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

		const ObjectTypeInfo* info = DB::get_object_type_info<T>();

		RTTI_ASSERT(info != nullptr, "Failed to set value of type '", variant_type_name(value),
			"' for object with no RTTI: ", variant_dump_as_string(value));

		DB::object_set_value_default(obj, value, info);

#else
		RTTI_UNUSED(obj);
		RTTI_UNUSED(value);
#endif // !RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT
	}

	template<typename T>
	static void object_get_value(const T* obj, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

		const ObjectTypeInfo* info = DB::get_object_type_info<T>();

		RTTI_ASSERT(info != nullptr, "Failed to get value from object with no RTTI");

		DB::object_get_value_default(obj, value, info);

#else
		RTTI_UNUSED(obj);
		RTTI_UNUSED(value);
#endif // !RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT
	}

	// TODO(FiTH): add multidimensional array support
	// TODO(FiTH): add char[] support?
	template<typename T, size_t N>
	static void object_set_value(T (&arr)[N], const Variant& array) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_ARRAYS_SET_AND_GET_VALUE

		RTTI_ASSERT(variant_holds_alternative<variant_array_t>(array), RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
			variant_type_name(array), "<builtin array>", variant_dump_as_string(array)));

		size_t array_size = variant_array_size(array);

		RTTI_WARN_ASSERT(array_size == N, "Array variable [", N, "] and array value [",
			array_size, "] have different sizes: ", array);

		size_t min_size = std::min(array_size, N);

		for (size_t i = 0; i < min_size; ++i) {
			const Variant& value = variant_array_element(array, i);
			object_set_value<T>(arr + i, value);
		}

#else
		RTTI_UNUSED(arr);
		RTTI_UNUSED(array);
#endif // !RTTI_DISABLE_BUILTIN_ARRAYS_SET_AND_GET_VALUE
	}

	template<typename T, size_t N>
	static void object_get_value(const T (&arr)[N], Variant& array) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_ARRAYS_SET_AND_GET_VALUE

		variant_make_array(array);
		variant_array_reserve(array, N);

		for (const T& value: arr)
			variant_array_emplace_back(array) = value;

#else
		RTTI_UNUSED(arr);
		RTTI_UNUSED(array);
#endif // !RTTI_DISABLE_BUILTIN_ARRAYS_SET_AND_GET_VALUE
	}

#ifndef RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

	template<typename T>
	static void object_set_value(std::vector<T>* vec, const Variant& array) noexcept
	{
		RTTI_ASSERT(variant_holds_alternative<variant_array_t>(array), RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
			variant_type_name(array), "std::vector", variant_dump_as_string(array)));

		size_t array_size = variant_array_size(array);

		vec->clear();
		vec->reserve(array_size);

		for (size_t i = 0; i < array_size; ++i) {
			const Variant& value = variant_array_element(array, i);
			object_set_value<T>(&vec->emplace_back(), value);
		}
	}

	template<typename T>
	static void object_get_value(const std::vector<T>* vec, Variant& array) noexcept
	{
		variant_make_array(array);
		variant_array_reserve(array, vec->size());

		for (const T& value: *vec)
			variant_array_emplace_back(array) = value;
	}

#endif // !RTTI_DISABLE_STL_TYPES_SET_AND_GET_VALUE

};

}

#endif // !__RTTI_DB_H__
