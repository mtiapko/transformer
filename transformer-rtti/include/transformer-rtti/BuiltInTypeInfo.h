#ifndef __TRANSFORMER_RTTI_BUILT_IN_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_BUILT_IN_TYPE_INFO_H__

#include "transformer-rtti/NamedTypeInfo.h"

namespace transformer::rtti
{

struct BuiltInTypeInfo final : public NamedTypeInfo
{
	const bool is_signed;
};

}

#endif // !__TRANSFORMER_RTTI_BUILT_IN_TYPE_INFO_H__
