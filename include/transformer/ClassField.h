#ifndef __TRANSFORMER_CLASS_FIELD_H__
#define __TRANSFORMER_CLASS_FIELD_H__

#include "transformer/Variable.h"

namespace trans
{

class ClassField : public Variable
{
public:
	ClassField(CXCursor cur) noexcept : Variable(cur) {}
};

}

#endif // !__TRANSFORMER_CLASS_FIELD_H__
