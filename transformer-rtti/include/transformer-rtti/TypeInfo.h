#ifndef __TRANSFORMER_RTTI_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_TYPE_INFO_H__

#include "transformer-rtti/TypeID.h"

namespace transformer::rtti
{

// TODO(FiTH): RECORD_TYPE and RECORD_TEMPLATE_SPECIALIZATION_TYPE must have the same lower bits?
enum class TypeKind : uint32_t // TODO(FiTH): or uint8_t?
{
	UNKNOWN_TYPE = 0,

	/* unnamed types */
	POINTER_TYPE,
	ARRAY_TYPE,

	/* for internal use only! */ BEG_NAMES_TYPES_VALUE,

	/* named types */
	BUILT_IN_TYPE = BEG_NAMES_TYPES_VALUE,
	RECORD_TYPE,
	RECORD_TEMPLATE_TYPE,                // ex.: std::vector,        std::map
	RECORD_TEMPLATE_SPECIALIZATION_TYPE, // ex.: std::vector<float>, std::map<std::string, int>
	ENUM_TYPE,

	/* for internal use only! */ END_NAMED_TYPES_VALUE = ENUM_TYPE
};

// objects with the same type have pointers to the same TypeInfo instance
struct TypeInfo
{
	const TypeID   id;
	const TypeKind kind;

	// TODO(FiTH): size? Or size must be only in CompleteTypeInfo?

	// TODO(FiTH): in DEBUG mode add 'file' and 'line' where 'set_type_info' was called from
};

}

#endif // !__TRANSFORMER_RTTI_TYPE_INFO_H__
