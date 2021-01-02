#include "transformer/ClassConstructor.h"

namespace transformer
{

ClassConstructor::ClassConstructor(CXCursor cur) noexcept
	: ClassCallableEntity(cur)
{
	CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cur);
	CXSourceLocation cur_loc = clang_getCursorLocation(cur);
	this->add_attribute(this->parse_attributes(tu, cur_loc));
}

}
