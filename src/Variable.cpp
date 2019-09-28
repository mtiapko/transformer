#include "transformer/Variable.h"
#include "transformer/Util.h"

namespace trans
{

Variable::Variable(CXCursor cur) noexcept
	: Entity(cur)
	, m_type { Util::to_string(clang_getTypeSpelling(clang_getCursorType(cur))) }
{}

}
