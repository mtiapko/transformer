#include "transformer/Variable.h"
#include "transformer/Util.h"

namespace transformer
{

Variable::Variable(CXCursor cur) noexcept
	: Entity(cur)
{
	auto cxtype = clang_getCanonicalType(clang_getCursorType(cur));
	CXCursor type_cursor = clang_getTypeDeclaration(cxtype);

	if (clang_Cursor_isAnonymous(type_cursor)) {
		m_type = "decltype(";
		m_type += this->full_name();
		m_type += ')';
	} else {
		m_type = Util::to_string(clang_getTypeSpelling(cxtype));
	}

	// TODO(FiTH): is type is <anonymous> this checks are useless?
	auto kind = cxtype.kind;
	m_is_builtin_type = (CXType_FirstBuiltin <= kind && kind <= CXType_LastBuiltin);
	m_is_pointer_type = (kind == CXType_Pointer);
	m_is_lvalue_reference_type = (kind == CXType_LValueReference);
	m_is_rvalue_reference_type = (kind == CXType_RValueReference);
	m_is_array_type = (kind == CXType_ConstantArray || kind == CXType_IncompleteArray);
	m_is_enum_type = (kind == CXType_Enum);

	if (m_is_array_type) {
		m_element_type = Util::to_string(
			clang_getTypeSpelling(
				clang_getCanonicalType(
					clang_getArrayElementType(cxtype)
				)
			)
		);

		m_elements_count = (kind == CXType_ConstantArray
			? clang_getArraySize(cxtype)
			: std::numeric_limits<size_t>::max());
	}

	CXTranslationUnit tu       = clang_Cursor_getTranslationUnit(cur);
	CXSourceLocation cur_loc   = clang_getCursorLocation(cur);

	CXSourceRange cur_range    = clang_getCursorExtent(cur);
	CXSourceLocation begin_loc = clang_getRangeStart(cur_range);

	this->add_attribute(this->parse_attributes(tu, begin_loc, -1));
	this->add_attribute(this->parse_attributes(tu, cur_loc, -1));
	this->add_attribute(this->parse_attributes(tu, cur_loc, this->name().size()));
}

}
