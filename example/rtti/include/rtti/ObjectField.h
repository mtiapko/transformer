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
	const ObjectFieldInfo* m_field_info = nullptr;

public:
	ObjectField(std::nullptr_t) noexcept {}
	ObjectField(void* parent_ptr, const ObjectFieldInfo* field_info) noexcept
		: m_parent_ptr(parent_ptr)
		, m_field_info(field_info)
	{}

	// TODO(FiTH): add array API

	ObjectField subfield(type_info_name_view_t name) noexcept
	{
		RTTI_ASSERT_RET(m_field_info != nullptr, nullptr,
			"Object field not exists. Failed to get subfield '", name, '\'');

		const ObjectTypeInfo* object_type_info = m_field_info->get_object_type_info();
		RTTI_ASSERT_RET(object_type_info != nullptr, nullptr, "No RTTI for object field '",
			m_field_info->name, "' of type '", m_field_info->type_info.name,
			"'. Failed to get subfield: ", name);

		const object_type_info_fields_info_map_t::const_iterator subfield_iter
			= object_type_info->fields_info_map.find(name);

		if (subfield_iter == object_type_info->fields_info_map.cend())
			return nullptr;

		void* field_ptr = m_field_info->get_addr(m_parent_ptr);
		return { field_ptr, subfield_iter->second };
	}

	void set_value(const Variant& value) noexcept
	{
		RTTI_ASSERT(m_field_info != nullptr, "Object field not exists. "
			"Failed to set value: ", value);

		m_field_info->type_info.setter(m_parent_ptr, value);
	}

	ObjectField& operator=(const Variant& value) noexcept
	{
		this->set_value(value);
		return *this;
	}


	void get_value(Variant& value) const noexcept
	{
		RTTI_ASSERT(m_field_info != nullptr, "Object field not exists. Failed to get its value");

		m_field_info->type_info.getter(m_parent_ptr, value);
	}

	Variant get_value() const noexcept;
	operator Variant() const noexcept;
};

}

#endif // !__RTTI_OBJECT_FIELD_H__
