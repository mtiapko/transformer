#include "transformer/Variable.h"
#include "transformer/Util.h"

namespace transformer
{

Variable::Variable(CXCursor cur) noexcept
	: Entity(cur)
{
	auto cxtype = clang_getCanonicalType(clang_getCursorType(cur));
	auto kind = cxtype.kind;

	m_type = Util::to_string(clang_getTypeSpelling(cxtype));

	m_is_builtin_type = (CXType_FirstBuiltin <= kind && kind <= CXType_LastBuiltin);
	m_is_pointer_type = (kind == CXType_Pointer);
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

		m_elements_counts = (kind == CXType_ConstantArray
			? clang_getArraySize(cxtype)
			: std::numeric_limits<size_t>::max());
	}
}

}
