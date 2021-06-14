#ifndef __TRANSFORMER_RTTI_ENUM_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_ENUM_TYPE_INFO_H__

#include "transformer-rtti/stdlib/Span.h"
#include "transformer-rtti/stdlib/Map.h"
#include "transformer-rtti/NamedTypeInfo.h"

namespace transformer::rtti
{

struct EnumConstantInfo
{
	StringView name;

	union
	{
		int64_t  signed_value;
		uint64_t unsigned_value;
	};
};

struct EnumTypeInfo final : public NamedTypeInfo
{
	const TypeID integer_type;

	const Span<const EnumConstantInfo> enumerators;
	const Map<StringView, const EnumConstantInfo*> enumerators_map;
	const Map<uint64_t, Span<const EnumConstantInfo* const>> enumerators_value_map;
};

}

#endif // !__TRANSFORMER_RTTI_ENUM_TYPE_INFO_H__
