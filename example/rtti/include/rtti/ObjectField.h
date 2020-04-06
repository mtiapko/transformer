#ifndef __RTTI_OBJECT_FIELD_H__
#define __RTTI_OBJECT_FIELD_H__

#include "rtti/ObjectTypeInfo.h"
#include "rtti/Assert.h"

namespace rtti
{

class ObjectField
{
private:
	void* m_parent_ptr = nullptr;
	const ObjectFieldTypeInfo* m_field_info = nullptr;

public:
	ObjectField(std::nullptr_t) noexcept {}
	ObjectField(void* parent_ptr, const ObjectFieldTypeInfo* field_info) noexcept
		: m_parent_ptr(parent_ptr)
		, m_field_info(field_info)
	{}

	const ObjectFieldTypeInfo* find_subfield_info(type_info_name_view_t name) const noexcept
	{
		RTTI_ASSERT_RET(m_field_info != nullptr, nullptr,
			"Object field not exists. Failed to find subfield '", name, "' info");

		const ObjectTypeInfo* object_type_info = m_field_info->get_object_type_info();
		RTTI_ASSERT_RET(object_type_info != nullptr, nullptr, "No RTTI for object field '",
			m_field_info->field_name, "' of type '", m_field_info->name, '\'');

		const object_type_info_fields_info_map_t::const_iterator field_iter
			= object_type_info->fields_info_map.find(name);

		if (field_iter == object_type_info->fields_info_map.cend())
			return nullptr;

		return field_iter->second;
	}

	ObjectField subfield(type_info_name_view_t name) noexcept
	{
		RTTI_ASSERT_RET(m_field_info != nullptr, nullptr,
			"Object field not exists. Failed to get subfield '", name, '\'');

		const ObjectFieldTypeInfo* subfield_info = this->find_subfield_info(name);

		RTTI_WARN_ASSERT(subfield_info != nullptr, "Object field '", (m_field_info != nullptr
			? m_field_info->field_name : "<no RTTI>"), "' of type '", m_field_info->name,
			"' does not have subfield '", name, '\'');

		void* this_ptr = m_field_info->get_addr(m_parent_ptr);
		return { this_ptr, subfield_info };
	}

	void set_value(const Variant& value) noexcept
	{
		RTTI_ASSERT(m_field_info != nullptr, "Object field not exists. "
			"Failed to set value '", variant_dump_as_string(value), '\'');

		m_field_info->setter(m_parent_ptr, value);
	}

	ObjectField& operator=(const Variant& value) noexcept
	{
		this->set_value(value);
		return *this;
	}


	void get_value(Variant& value) const noexcept
	{
		RTTI_ASSERT(m_field_info != nullptr, "Object field not exists. Failed to get its value");

		m_field_info->getter(m_parent_ptr, value);
	}

	Variant get_value() const noexcept;
	operator Variant() const noexcept;
};

}

#endif // !__RTTI_OBJECT_FIELD_H__
