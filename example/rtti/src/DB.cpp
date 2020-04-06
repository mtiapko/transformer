#include "rtti/Variant.h"
#include "rtti/DB.h"

namespace rtti
{

// TODO(FiTH): remove file and do this set/get in .h?

/* Implementation of 'DB::object_set_value_default' and 'DB::object_get_value_default' */

#ifndef RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

/* static */ void DB::object_set_value_default(void* obj, const Variant& object, const ObjectTypeInfo* info) noexcept
{
	RTTI_ASSERT(object.type() == Variant::value_t::object, RTTI_DB_PRINT_FAILED_TO_SET_VALUE_FMT(
		variant_type_name(object), info->name, variant_dump_as_string(object)));

	const Variant::object_t* map = object.get_ptr<const Variant::object_t*>();
	for (const auto& [key, value]: *map) {
		auto field_iter = info->fields_info_map.find(key);
		RTTI_ASSERT(field_iter != info->fields_info_map.cend(),
			"Type '", info->name, "' does not have any field named '", key, '\'');

		field_iter->second->setter(obj, value);
	}
}

/* static */ void DB::object_get_value_default(const void* obj, Variant& object, const ObjectTypeInfo* info) noexcept
{
	for (const ObjectFieldTypeInfo& field_info: info->fields_info) {
		Variant field_value;
		field_info.getter(obj, field_value);

		if (field_value.type() != Variant::value_t::null)
			object.emplace(field_info.field_name, std::move(field_value));
	}
}

#endif // !RTTI_DISABLE_OBJECT_TYPE_SET_AND_GET_VALUE_DEFAULT

}