#ifndef __TRANSFORMER_ENUM_H__
#define __TRANSFORMER_ENUM_H__

#include <vector>
#include "transformer/EnumConst.h"

namespace transformer
{

class Enum : public Entity
{
private:
	bool                   m_is_unsigned_integer_type;
	std::string            m_integer_type;
	std::vector<EnumConst> m_consts;

	static CXChildVisitResult visitor(CXCursor cur, CXCursor /* parent */,
		CXClientData closure) noexcept;

	static bool is_signed_type(CXType type) noexcept;
	static bool is_unsigned_type(CXType type) noexcept;

public:
	Enum(CXCursor cur, std::string full_name) noexcept;

	bool is_unsigned_integer_type() const noexcept { return m_is_unsigned_integer_type; }
	const auto& integer_type() const noexcept { return m_integer_type; }

	void add_const(CXCursor cur, int64_t value) noexcept { m_consts.emplace_back(cur, value); }
	void add_const(CXCursor cur, uint64_t value) noexcept { m_consts.emplace_back(cur, value); }
	const auto& consts() const noexcept { return m_consts; }
};

}

#endif // !__TRANSFORMER_ENUM_H__
