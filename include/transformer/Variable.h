#ifndef __TRANSFORMER_VARIABLE_H__
#define __TRANSFORMER_VARIABLE_H__

#include "transformer/Entity.h"

namespace trans
{

class Variable : public Entity
{
private:
	std::string m_type;

public:
	Variable(CXCursor cur) noexcept;

	const auto& type() const noexcept { return m_type; }
};

}

#endif // !__TRANSFORMER_VARIABLE_H__
