#ifndef __TRANSFORMER_RTTI_POINTER_TYPE_INFO_H__
#define __TRANSFORMER_RTTI_POINTER_TYPE_INFO_H__

#include "transformer-rtti/QualifiedType.h"

namespace transformer::rtti
{

struct PointerTypeInfo : public TypeInfo
{
	const QualifiedType pointee_type;
};

}

#endif // !__TRANSFORMER_RTTI_POINTER_TYPE_INFO_H__
