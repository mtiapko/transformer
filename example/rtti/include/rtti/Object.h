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

	ObjectField object_field(type_info_name_view_t name) noexcept
	{
		RTTI_ASSERT_RET(m_info != nullptr, nullptr,
			"No RTTI for object. Failed to get field: ", name);

		const object_type_info_fields_info_map_t::const_iterator field_iter
			= m_info->fields_info_map.find(name);

		if (field_iter == m_info->fields_info_map.cend())
			return nullptr;

		return { this, field_iter->second };
	}

	void object_set_value(const Variant& value) noexcept
	{
		RTTI_ASSERT(m_info != nullptr, "No RTTI for object. "
			"Failed to set value: ", value);

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
