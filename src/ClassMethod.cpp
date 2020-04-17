#include "transformer/ClassMethod.h"
#include "transformer/Util.h"
#include "transformer/Log.h"

namespace transformer
{

ClassMethod::ClassMethod(CXCursor cur) noexcept
	: Entity(cur)
{
	CXTranslationUnit tu       = clang_Cursor_getTranslationUnit(cur);
	CXSourceRange cur_range    = clang_getCursorExtent(cur);
	CXSourceLocation begin_loc = clang_getRangeStart(cur_range);
	this->add_attribute(this->parse_attributes(tu, begin_loc, -1));

	auto method_type = clang_getCursorType(cur);
	auto return_type = clang_getCanonicalType(clang_getResultType(method_type));
	m_return_type = Util::to_string(clang_getTypeSpelling(return_type));

	int num_args = clang_Cursor_getNumArguments(cur);
	// TODO(FiTH): add assert

	if (num_args == -1) {
		TF_LOG_ERROR("Failed to get number of method arguments!");
		return;
	}

	m_args.reserve(num_args);
	for (int i = 0; i < num_args; ++i)
		m_args.emplace_back(clang_Cursor_getArgument(cur, i));

	m_is_const = clang_CXXMethod_isConst(cur);
	m_is_static = clang_CXXMethod_isStatic(cur);
}

}
