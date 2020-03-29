#include <assert.h>
#include "transformer/Enum.h"
#include "transformer/Util.h"

namespace transformer
{

/* static */ CXChildVisitResult Enum::visitor(CXCursor cur, CXCursor /* parent */,
	CXClientData closure) noexcept
{
	auto e = static_cast<Enum*>(closure);
	switch (clang_getCursorKind(cur)) {
		case CXCursor_EnumConstantDecl:
			if (e->is_unsigned_integer_type())
				e->add_const(cur, (uint64_t)clang_getEnumConstantDeclUnsignedValue(cur));
			else
				e->add_const(cur, (int64_t)clang_getEnumConstantDeclValue(cur));

			break;
		default: break;
	}

	return CXChildVisit_Continue;
}

/* static */ bool Enum::is_signed_type(CXType type) noexcept
{
	// TODO(FiTH): is there a better way to check signed/unsigned int?
	return (CXType_Char_S <= type.kind && type.kind <= CXType_Int128);
}

/* static */ bool Enum::is_unsigned_type(CXType type) noexcept
{
	return (CXType_Bool <= type.kind && type.kind <= CXType_UInt128);
}

Enum::Enum(CXCursor cur, std::string full_name) noexcept
	: Entity(cur, std::move(full_name))
	, m_is_unsigned_integer_type
		{ Enum::is_unsigned_type(clang_getCanonicalType(clang_getEnumDeclIntegerType(cur))) }
	, m_integer_type { Util::to_string(clang_getTypeSpelling(clang_getEnumDeclIntegerType(cur))) }
{
	assert(Enum::is_signed_type(clang_getCanonicalType(clang_getEnumDeclIntegerType(cur))) ||
		Enum::is_unsigned_type(clang_getCanonicalType(clang_getEnumDeclIntegerType(cur))));
	clang_visitChildren(cur, Enum::visitor, this);
}

}
