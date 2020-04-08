#ifndef __RTTI_OBJECT_H__
#define __RTTI_OBJECT_H__

#include "rtti/ObjectField.h"
#include "rtti/DB.h"

namespace rtti
{

class Object
{
private:
	template<typename T> friend class ObjectType;

	const ObjectTypeInfo* m_info;

	explicit Object(const ObjectTypeInfo* info) noexcept
		: m_info(info)
	{}

public:
	const ObjectTypeInfo* object_type_info() const noexcept
	{
		return m_info;
	}

	const ObjectFieldInfo* object_find_field_info(type_info_name_view_t name) const noexcept
	{
		RTTI_ASSERT_RET(m_info != nullptr, nullptr,
			"No RTTI for object. Failed to find field '", name, "' info");

		const object_type_info_fields_info_map_t::const_iterator field_iter
			= m_info->fields_info_map.find(name);

		if (field_iter == m_info->fields_info_map.cend())
			return nullptr;

		return field_iter->second;
	}

	ObjectField object_field(type_info_name_view_t name) noexcept
	{
		const ObjectFieldInfo* field_info = this->object_find_field_info(name);

		RTTI_WARN_ASSERT(field_info != nullptr, "Object of type '", (m_info != nullptr
			? m_info->name : "<no RTTI>"), "' does not have field '", name, '\'');

		return { this, field_info };
	}

	void object_set_value(const Variant& value) noexcept
	{
		RTTI_ASSERT(m_info != nullptr, "No RTTI for object. "
			"Failed to set value '", value, '\'');

		m_info->setter(this, value);
	}

	void object_get_value(Variant& value) const noexcept
	{
		RTTI_ASSERT(m_info != nullptr, "No RTTI for object. Failed to get its value");

		return m_info->getter(this, value);
	}

	Variant object_get_value() const noexcept;
};

}

#endif // !__RTTI_OBJECT_H__
