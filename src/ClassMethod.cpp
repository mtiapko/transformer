#include "transformer/ClassMethod.h"
#include "transformer/Util.h"

namespace transformer
{

ClassMethod::ClassMethod(CXCursor cur) noexcept
	: ClassCallableEntity(cur)
{
	CXTranslationUnit tu       = clang_Cursor_getTranslationUnit(cur);
	CXSourceRange cur_range    = clang_getCursorExtent(cur);
	CXSourceLocation begin_loc = clang_getRangeStart(cur_range);
	this->add_attribute(this->parse_attributes(tu, begin_loc));

	auto method_type = clang_getCursorType(cur);
	auto return_type = clang_getCanonicalType(clang_getResultType(method_type));
	m_return_type = Util::to_string(clang_getTypeSpelling(return_type));

	m_is_const = clang_CXXMethod_isConst(cur);
	m_is_static = clang_CXXMethod_isStatic(cur);
}

}
