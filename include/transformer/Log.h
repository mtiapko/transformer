#ifndef __TRANSFORMER_LOG_H__
#define __TRANSFORMER_LOG_H__

#include <iostream>
#include <clang-c/CXString.h>

#define TRANS_PRINT_IMPL(out, ...)  trans::Log::print(std::c##out, ##__VA_ARGS__, '\n');
#define TRANS_PRINT(...)            TRANS_PRINT_IMPL(log, ##__VA_ARGS__);
#define TRANS_PRINT_ERR(...)        TRANS_PRINT_IMPL(err, ##__VA_ARGS__);

#define TRANS_LOG_IMPL(out, title, ...) TRANS_PRINT_IMPL(out, title, " " __FILE__ "::", __func__, \
	" (", __LINE__, ") - ", ##__VA_ARGS__)

#define TRANS_LOG(...)       TRANS_LOG_IMPL(log, "[  MSG  ]", ##__VA_ARGS__)
#define TRANS_LOG_ERROR(...) TRANS_LOG_IMPL(err, "[ ERROR ]", ##__VA_ARGS__)

namespace trans
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
