#ifndef __TRANSFORMER_CLASS_METHOD_H__
#define __TRANSFORMER_CLASS_METHOD_H__

#include "transformer/ClassCallableEntity.h"

// clang_CXXMethod_isStatic

namespace transformer
{

class ClassMethod : public ClassCallableEntity
{
private:
	std::string m_return_type;

	bool m_is_const = false;
	bool m_is_static = false;

public:
	ClassMethod(CXCursor cur) noexcept;

	const auto& return_type() const noexcept { return m_return_type; }

	bool is_const() const noexcept { return m_is_const; }
	bool is_static() const noexcept { return m_is_static; }
};

}

#endif // !__TRANSFORMER_CLASS_METHOD_H__
