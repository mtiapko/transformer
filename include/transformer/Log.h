#ifndef __TRANSFORMER_LOG_H__
#define __TRANSFORMER_LOG_H__

#include <iostream>
#include <clang-c/CXString.h>

#define TF_PRINT_IMPL(out, ...)  transformer::Log::print(std::c##out, ##__VA_ARGS__, '\n');
#define TF_PRINT(...)            TF_PRINT_IMPL(log, ##__VA_ARGS__);
#define TF_PRINT_ERR(...)        TF_PRINT_IMPL(err, ##__VA_ARGS__);

#define TF_LOG_FMT __FILE__ "::", __func__, " (", __LINE__, ") - "

#define TF_LOG_IMPL(...) TF_PRINT("[  MSG  ] ", ##__VA_ARGS__)
#define TF_LOG(...)      TF_LOG_IMPL(TF_LOG_FMT, ##__VA_ARGS__)

#define TF_LOG_ERROR_IMPL(...) TF_PRINT_ERR("[ ERROR ] ", ##__VA_ARGS__)
#define TF_LOG_ERROR(...)      TF_LOG_ERROR_IMPL(TF_LOG_FMT, ##__VA_ARGS__)

namespace transformer
{

class Log
{
public:
	template<typename... Args>
	static void print(std::ostream& out, const Args&... args) noexcept
	{
		(out << ... << args);
	}
};

}

inline std::ostream& operator<<(std::ostream& out, CXString s)
{
	out << clang_getCString(s);
	clang_disposeString(s);
	return out;
}

#endif // !__TRANSFORMER_LOG_H__
