#ifndef __RTTI_TYPE_INFO_H__
#define __RTTI_TYPE_INFO_H__

#include "rtti/Variant.h"

namespace rtti
{

#ifndef RTTI_DISABLE_DEFAULT_CONFIG

using type_info_setter_t = void (*)(void*, const Variant&) noexcept;
using type_info_getter_t = Variant (*)(const void*) noexcept;

#endif // !RTTI_DISABLE_DEFAULT_CONFIG

struct TypeInfo
{
public:
	type_info_name_t name;

	type_info_setter_t setter;
	type_info_getter_t getter;
};

}

#endif // !__RTTI_TYPE_INFO_H__
