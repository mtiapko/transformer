#ifndef __TRANSFORMER_CLASS_CALLABLE_ENTITY_H__
#define __TRANSFORMER_CLASS_CALLABLE_ENTITY_H__

#include <vector>
#include "transformer/Variable.h"

namespace transformer
{

// TODO(FiTH): rename to just CallableEntity? create Function class derived from CallableEntity?
class ClassCallableEntity : public Entity
{
protected:
	std::vector<Variable> m_args;

	// TODO(FiTH): bool m_is_deleted = false;

protected:
	ClassCallableEntity(CXCursor cur) noexcept;

public:
	const auto& args() const noexcept { return m_args; }
};

}

#endif // !__TRANSFORMER_CLASS_CALLABLE_ENTITY_H__
