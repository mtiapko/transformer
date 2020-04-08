#ifndef __RTTI_DB_H__
#define __RTTI_DB_H__

#include "rtti/ObjectTypeInfo.h"
#include "rtti/Assert.h"

namespace rtti
{

class DB
{
public:
	template<typename T>
	static ObjectTypeInfo make_object_type_info() noexcept;

	template<typename T>
	static const ObjectTypeInfo* get_object_type_info() noexcept
	{
		return nullptr;
	}

	template<typename T>
	static void variable_set_value(T* obj, const Variant& value) noexcept
	{
		auto object_type_info = DB::get_object_type_info<T>();
		RTTI_ASSERT(object_type_info != nullptr, "Variable has no RTTI. Failed to set value: ", value);

		object_type_info->setter(obj, value);
	}

	template<typename T>
	static void variable_get_value(const T* obj, Variant& value) noexcept
	{
		auto object_type_info = DB::get_object_type_info<T>();
		RTTI_ASSERT(object_type_info != nullptr, "Variable has no RTTI. Failed to get value: ", value);

		object_type_info->getter(obj, value);
	}

	/* Builtin array types */

	template<typename T, size_t N>
	static void variable_set_value(T (&arr)[N], const Variant& array_value) noexcept;

	template<typename T, size_t N>
	static void variable_get_value(const T (&arr)[N], Variant& array_value) noexcept;

	/* STL types */

#ifndef RTTI_DISABLE_STL_TYPES_DEFAULT_SETTER_GETTER

	/* std::vector */

	template<typename T>
	static void variable_set_value(std::vector<T>* vec, const Variant& array) noexcept;

	template<typename T>
	static void variable_get_value(const std::vector<T>* vec, Variant& array) noexcept;

	/* std::string */

	static void variable_set_value(std::string* obj, const Variant& value) noexcept;
	static void variable_get_value(const std::string* obj, Variant& value) noexcept;

#endif // !RTTI_DISABLE_STL_TYPES_DEFAULT_SETTER_GETTER

};

}

#endif // !__RTTI_DB_H__
