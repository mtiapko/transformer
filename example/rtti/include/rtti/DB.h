#ifndef __RTTI_DB_H__
#define __RTTI_DB_H__

#include "rtti/ObjectTypeInfo.h"

namespace rtti
{

class DB
{
private:
	static void object_set_value_default(void* obj, const Variant& value, const ObjectTypeInfo* info) noexcept;

public:
	template<typename T>
	static ObjectTypeInfo make_object_type_info() noexcept;

	template<typename T>
	static const ObjectTypeInfo* get_object_type_info() noexcept
	{
		return nullptr;
	}

	template<typename T>
	static void object_set_value(void* obj, const Variant& value) noexcept
	{
		const ObjectTypeInfo* info = DB::get_object_type_info<T>();
		if (info == nullptr) {
			// TODO: log
			return;
		}

		DB::object_set_value_default(obj, value, info);
	}
};

}

#endif // !__RTTI_DB_H__
