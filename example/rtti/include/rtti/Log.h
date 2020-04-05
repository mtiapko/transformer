#ifndef __RTTI_LOG_H__
#define __RTTI_LOG_H__

#include "rtti/Config.h"

#ifndef RTTI_DISABLE_LOG

#include <iostream>

#define RTTI_PRINT_IMPL(out, ...)  rtti::Log::print(std::c##out, ##__VA_ARGS__, '\n');

#define RTTI_PRINT(...)     RTTI_PRINT_IMPL(log, ##__VA_ARGS__);
#define RTTI_PRINT_ERR(...) RTTI_PRINT_IMPL(err, ##__VA_ARGS__);

#define RTTI_LOG_FMT __FILE__ "::", __func__, " (", __LINE__, ") - "

#define RTTI_LOG_IMPL(...)       RTTI_PRINT("[  MSG  ] ", ##__VA_ARGS__)
#define RTTI_LOG(...)            RTTI_LOG_IMPL(RTTI_LOG_FMT, ##__VA_ARGS__)

#define RTTI_LOG_WARN_IMPL(...)  RTTI_PRINT_ERR("[WARNING] ", ##__VA_ARGS__)
#define RTTI_LOG_WARN(...)       RTTI_LOG_WARN_IMPL(RTTI_LOG_FMT, ##__VA_ARGS__)

#define RTTI_LOG_ERR_IMPL(...) RTTI_PRINT_ERR("[ ERROR ] ", ##__VA_ARGS__)
#define RTTI_LOG_ERR(...)      RTTI_LOG_ERR_IMPL(RTTI_LOG_FMT, ##__VA_ARGS__)

namespace rtti
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

#else
#	define RTTI_PRINT(...)     /* empty */
#	define RTTI_PRINT_ERR(...) /* empty */

#	define RTTI_LOG(...)       /* empty */
#	define RTTI_LOG_WARN(...)  /* empty */
#	define RTTI_LOG_ERR(...)   /* empty */
#endif // !RTTI_DISABLE_LOG

#endif // !__RTTI_LOG_H__
