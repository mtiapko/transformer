#ifndef __RTTI_OBJECT_TYPE_H__
#define __RTTI_OBJECT_TYPE_H__

#include "rtti/Object.h"

#define RTTI_OBJECT_TYPE_BODY \
	template<typename T> friend rtti::ObjectTypeInfo rtti::DB::make_object_type_info() noexcept

namespace rtti
{

template<typename T>
class ObjectType : public Object
{
public:
	ObjectType() noexcept
		: Object(DB::get_object_type_info<T>())
	{}
};

}

#endif // !__RTTI_OBJECT_TYPE_H__
