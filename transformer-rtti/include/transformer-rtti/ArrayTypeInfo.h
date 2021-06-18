#ifndef __TRANSFORMER_RTTI_ARRAY_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_ARRAY_TYPE_INFO_H__

#include "transformer-rtti/PointerTypeInfo.h"

namespace transformer::rtti
{

struct ArrayTypeInfo final : public PointerTypeInfo
{
	// TODO(FiTH): is fixed size array?
	const uint32_t extent; // TODO(FiTH): or size_t?
};

}

#endif // !__TRANSFORMER_RTTI_ARRAY_TYPE_INFO_H__
