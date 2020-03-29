#include "transformer/Entity.h"
#include "transformer/Util.h"

namespace transformer
{

Entity::Entity(CXCursor cur) noexcept
	: Entity(cur, Util::get_full_name(cur))
{}

Entity::Entity(CXCursor cur, std::string full_name) noexcept
	: m_is_from_main_file { (clang_Location_isFromMainFile(clang_getCursorLocation(cur)) != 0) }
	, m_file_path         { Util::get_file_path(cur) }
	, m_full_name         { std::move(full_name) }
	, m_name              { Util::to_string(clang_getCursorSpelling(cur)) }
{}

}
