#ifndef __TRANSFORMER_TOKEN_PTR_H__
#define __TRANSFORMER_TOKEN_PTR_H__

#include <clang-c/Index.h>
#include <memory>

namespace transformer
{

class TokenPtr
{
private:
	CXTranslationUnit m_tu;
	CXToken* m_token;
	size_t m_count;

public:
	TokenPtr(const CXTranslationUnit& tu, CXToken* token, size_t count) noexcept
		: m_tu(tu)
		, m_token(token)
		, m_count(count)
	{}

	~TokenPtr() noexcept
	{
		if (m_token != nullptr) {
			clang_disposeTokens(m_tu, m_token, m_count);
			m_token = nullptr;
		}
	}

	operator CXToken*() const noexcept { return m_token; }

	size_t count() const noexcept { return m_count; }
};

}

#endif // !__TRANSFORMER_TOKEN_PTR_H__
