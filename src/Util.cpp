#include "transformer/Util.h"

namespace trans
{

/* static */ std::string Util::to_string(CXString s) noexcept
{
	const auto c_str = clang_getCString(s);
	auto std_s = std::string(c_str != nullptr ? c_str : "");
	clang_disposeString(s);
	return std_s;
}

/* static */ std::string Util::get_file_path(CXCursor cur) noexcept
{
	CXFile file;
	const auto loc = clang_getCursorLocation(cur);
	clang_getSpellingLocation(loc, &file, nullptr, nullptr, nullptr);
	return Util::to_string(clang_getFileName(file));
}

/* static */ std::string Util::get_full_name(CXCursor cur) noexcept
{
	std::string full_name;
	while (clang_isDeclaration(clang_getCursorKind(cur)) != 0) {
		const auto name_part = Util::to_string(clang_getCursorSpelling(cur));
		if (!full_name.empty()) {
			full_name.insert(0, "::", 2);
			full_name.insert(0, name_part);
		} else {
			full_name = name_part;
		}

		cur = clang_getCursorSemanticParent(cur);
	}

	return full_name;
}

}
