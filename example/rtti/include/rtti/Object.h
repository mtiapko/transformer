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

	const ObjectFieldTypeInfo* object_find_field_info(std::string_view name) const
	{
		const ObjectTypeInfo* info = this->object_type_info();
		const object_type_info_fields_info_map_t::const_iterator field_iter
			= info->fields_info_map.find(name);

		if (field_iter == info->fields_info_map.cend()) {
			// TODO: log
			std::clog << "Object of type '" << info->name << "' does not have subfield '" << name << "'\n";
			return nullptr;
		}

		return field_iter->second;
	}

	ObjectField object_field(std::string_view name) noexcept
	{
		// TODO: add assert for nullptr
		const ObjectFieldTypeInfo* field_info = this->object_find_field_info(name);
		return { this, field_info };
	}

	void object_set_value(const Variant& value) noexcept
	{
		// TODO: add assert for nullptr
		m_info->setter(this, value);
	}
};

}

#endif // !__RTTI_OBJECT_H__
