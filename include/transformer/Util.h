#ifndef __TRANSFORMER_UTIL_H__
#define __TRANSFORMER_UTIL_H__

#include <clang-c/Index.h>
#include <string>

namespace transformer
{

struct Util
{
	// TODO(FiTH): static char* to_string(CXString s, char* dst, size_t max_size) noexcept;

	static std::string to_string(CXString s) noexcept;
	static std::string get_file_path(CXCursor cur) noexcept;
	static std::string get_full_name(CXCursor cur) noexcept;
};

}

#endif // !__TRANSFORMER_UTIL_H__
