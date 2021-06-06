#ifndef __TRANSFORMER_RTTI_NAMED_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_NAMED_TYPE_INFO_H__

#include "transformer-rtti/stdlib/StringView.h"
#include "transformer-rtti/TypeInfo.h"

namespace transformer::rtti
{

struct NamedTypeInfo : public TypeInfo
{
	const StringView name;
};

}

#endif // !__TRANSFORMER_RTTI_NAMED_TYPE_INFO_H__
