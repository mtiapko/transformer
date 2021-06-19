#ifndef __TRANSFORMER_RTTI_RECORD_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_RECORD_TYPE_INFO_H__

#include "transformer-rtti/stdlib/Span.h"
#include "transformer-rtti/stdlib/Map.h"
#include "transformer-rtti/QualifiedType.h"
#include "transformer-rtti/NamedTypeInfo.h"

namespace transformer::rtti
{

// TODO(FiTH): add TypeID of owner of this base?
// TODO(FiTH): what if there are 2 bases with the same TypeID?
struct RecordBaseInfo
{
	const TypeID    id;
	const ptrdiff_t offset;
};

struct RecordFieldInfo
{
	const StringView    name;
	const ptrdiff_t     offset;    // TODO(FiTH): can be a bit field!
	const QualifiedType qual_type;
};

using RecordMethodPtrType = void (*)(const void* self, void* ret, const void* args[]);

struct RecordMethodParamInfo
{
	const StringView    name;
	const QualifiedType type;
};

struct RecordMethodInfo
{
	const StringView          name;
	const RecordMethodPtrType ptr;

	const uint8_t is_const: 1;
	// TODO(FiTH): const uint8_t is_noexcept: 1;

	const QualifiedType ret_type;
	const Span<const RecordMethodParamInfo> params;
};

struct RecordTypeInfo : public NamedTypeInfo
{
	const Span<const RecordBaseInfo>         bases;
	const Map<TypeID, const RecordBaseInfo*> bases_map;

	const Span<const RecordFieldInfo>             fields;
	const Map<StringView, const RecordFieldInfo*> fields_map;

	const Span<const RecordMethodInfo>             methods;
	const Map<StringView, const RecordMethodInfo*> methods_map; // TODO(FiTH): overloaded methods?
};

}

#endif // !__TRANSFORMER_RTTI_RECORD_TYPE_INFO_H__
