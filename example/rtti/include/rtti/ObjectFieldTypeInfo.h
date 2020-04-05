#ifndef __RTTI_OBJECT_FIELD_TYPE_INFO_H__
#define __RTTI_OBJECT_FIELD_TYPE_INFO_H__

#include "rtti/TypeInfo.h"

namespace rtti
{

struct ObjectFieldTypeInfo : public TypeInfo
{
	object_field_type_info_name_t field_name;

	object_field_type_info_get_addr_t             get_addr;
	object_field_type_info_get_object_type_info_t get_object_type_info;
};

}

#endif // !__RTTI_OBJECT_FIELD_TYPE_INFO_H__
