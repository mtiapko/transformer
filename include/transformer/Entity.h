#ifndef __TRANSFORMER_ENTITY_H__
#define __TRANSFORMER_ENTITY_H__

#include <string>
#include <clang-c/Index.h>

namespace transformer
{

class Entity
{
private:
	bool        m_is_from_main_file;
	std::string m_file_path;
	std::string m_full_name;
	std::string m_name;
	std::string m_attributes;

	static CXSourceLocation skip_current_token(const CXTranslationUnit tu,
		const CXSourceLocation& current_loc, int32_t step) noexcept;

	static CXSourceLocation find_double_char_location(const CXTranslationUnit& tu,
		CXSourceLocation current_loc, std::string_view char_str, int32_t offset, bool force) noexcept;

public:
	Entity(CXCursor cur) noexcept;
	Entity(CXCursor cur, std::string full_name) noexcept;

	bool is_from_main_file() const noexcept { return m_is_from_main_file; }
	const auto& file_path() const noexcept { return m_file_path; }
	const auto& full_name() const noexcept { return m_full_name; }
	const auto& name() const noexcept { return m_name; }
	const auto& attributes() const noexcept { return m_attributes; }

	void parse_attributes(const CXTranslationUnit& tu,
		const CXSourceLocation& loc, int32_t offset) noexcept;
};

}

#endif // !__TRANSFORMER_ENTITY_H__
