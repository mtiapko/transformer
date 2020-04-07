#ifndef __RTTI_OBJECT_FIELD_INFO_H__
#define __RTTI_OBJECT_FIELD_INFO_H__

#include "rtti/TypeInfo.h"

namespace rtti
{

struct ObjectFieldInfo
{
	TypeInfo type_info;

	object_field_info_field_name_t name;

	object_field_info_get_addr_t             get_addr;
	object_field_info_get_object_type_info_t get_object_type_info;
};

}

#endif // !__RTTI_OBJECT_FIELD_INFO_H__
