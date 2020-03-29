#ifndef __TRANSFORMER_ENUM_CONST_H__
#define __TRANSFORMER_ENUM_CONST_H__

#include "transformer/Entity.h"

namespace transformer
{

class EnumConst : public Entity
{
private:
	union
	{
		int64_t  m_signed_value;
		uint64_t m_unsigned_value;
	};

public:
	EnumConst(CXCursor cur, int64_t value) noexcept : Entity(cur), m_signed_value(value) {}
	EnumConst(CXCursor cur, uint64_t value) noexcept : Entity(cur), m_unsigned_value(value) {}

	int64_t signed_value() const noexcept { return m_signed_value; }
	uint64_t unsigned_value() const noexcept { return m_unsigned_value; }
};

}

#endif // !__TRANSFORMER_ENUM_CONST_H__
