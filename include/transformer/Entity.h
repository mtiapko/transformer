#ifndef __TRANSFORMER_ENTITY_H__
#define __TRANSFORMER_ENTITY_H__

#include <string>
#include <clang-c/Index.h>

namespace trans
{

class Entity
{
private:
	bool        m_is_from_main_file;
	std::string m_file_path;
	std::string m_full_name;
	std::string m_name;

public:
	Entity(CXCursor cur) noexcept;
	Entity(CXCursor cur, std::string full_name) noexcept;

	bool is_from_main_file() const noexcept { return m_is_from_main_file; }
	const auto& file_path() const noexcept { return m_file_path; }
	const auto& full_name() const noexcept { return m_full_name; }
	const auto& name() const noexcept { return m_name; }
};

}

#endif // !__TRANSFORMER_ENTITY_H__
