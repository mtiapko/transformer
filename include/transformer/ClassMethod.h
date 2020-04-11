#include <vector>
#include "transformer/Variable.h"

// clang_CXXMethod_isStatic

namespace transformer
{

class ClassMethod : public Entity
{
private:
	std::string m_return_type;
	std::vector<Variable> m_args;

	bool m_is_const = false;
	bool m_is_static = false;

public:
	ClassMethod(CXCursor cur) noexcept;

	const auto& return_type() const noexcept { return m_return_type; }
	const auto& args() const noexcept { return m_args; }

	bool is_const() const noexcept { return m_is_const; }
	bool is_static() const noexcept { return m_is_static; }
};

}
