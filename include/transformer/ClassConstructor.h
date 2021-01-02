#ifndef __TRANSFORMER_CLASS_CONSTRUCTOR_H__
#define __TRANSFORMER_CLASS_CONSTRUCTOR_H__

#include "transformer/ClassCallableEntity.h"

namespace transformer
{

class ClassConstructor : public ClassCallableEntity
{
public:
	ClassConstructor(CXCursor cur) noexcept;
};

}

#endif // !__TRANSFORMER_CLASS_CONSTRUCTOR_H__
