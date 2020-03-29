#ifndef __TRANSFORMER_PARSER_H__
#define __TRANSFORMER_PARSER_H__

#include "transformer/EntityScope.h"
#include "transformer/Config.h"
#include "transformer/Class.h"
#include "transformer/Enum.h"

namespace transformer
{

class Parser
{
private:
	EntityScope<Class> m_classes;
	EntityScope<Enum>  m_enums;

	static CXChildVisitResult visitor(CXCursor cur, CXCursor parent,
		CXClientData closure) noexcept;

public:
	Parser(const Config& cfg);

	auto& enums() noexcept { return m_enums; }
	const auto& enums() const noexcept { return m_enums; }

	auto& classes() noexcept { return m_classes; }
	const auto& classes() const noexcept { return m_classes; }
};

}

#endif // !__TRANSFORMER_PARSER_H__
