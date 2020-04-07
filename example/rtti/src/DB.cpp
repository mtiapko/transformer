#include "rtti/Variant.h"
#include "rtti/DB.h"

namespace rtti
{

/* Implementation of 'DB::object_set_value_default' and 'DB::object_get_value_default' */

#ifndef RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

/* static */ void DB::object_set_value_default(void* obj, const Variant& object, const ObjectTypeInfo* info) noexcept
{
	RTTI_ASSERT(object.is_object(), RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(variant_type_name(object),
		info->name, variant_dump_as_string(object)));

	const Variant::object_t* map = object.get_ptr<const Variant::object_t*>();
	for (const auto& [key, value]: *map) {
		auto field_iter = info->fields_info_map.find(key);
		RTTI_ASSERT_CONTINUE(field_iter != info->fields_info_map.cend(), "Type '", info->name,
			"' does not have any field named '", key, '\'');

		field_iter->second->type_info.setter(obj, value);
	}
}

/* static */ void DB::object_get_value_default(const void* obj, Variant& object, const ObjectTypeInfo* info) noexcept
{
	for (const ObjectFieldInfo& field_info: info->fields_info) {
		Variant field_value;
		field_info.type_info.getter(obj, field_value);

		if (!field_value.is_null())
			object.emplace(field_info.name, std::move(field_value));
	}
}

#endif // !RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

}
