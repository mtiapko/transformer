#ifndef __RTTI_OBJECT_TYPE_INFO_H__
#define __RTTI_OBJECT_TYPE_INFO_H__

#include "rtti/ObjectFieldInfo.h"

namespace rtti
{

// TODO(FiTH): add 'parants_info', 'parents_info_set' and 'runtime_cast' (in Object)
struct ObjectTypeInfo : public TypeInfo
{
	object_type_info_fields_info_t     fields_info;
	object_type_info_fields_info_map_t fields_info_map;
};

}

#endif // !__RTTI_OBJECT_TYPE_INFO_H__
