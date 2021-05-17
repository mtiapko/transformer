#ifndef __TRANSFORMER_RTTI_DBG_LOG_H__
#define __TRANSFORMER_RTTI_DBG_LOG_H__

#include <iostream>

#include "transformer-rtti/Config.h"

#define RTTI_LOG_PRINT(...) dbg::Log::print(__VA_ARGS__ __VA_OPT__(,) '\n')

#define RTTI_LOG(...)      RTTI_LOG_PRINT("[  MSG  ] " __VA_OPT__(,) __VA_ARGS__)
#define RTTI_LOG_ERR(...)  RTTI_LOG_PRINT("[ ERROR ] " __VA_OPT__(,) __VA_ARGS__)
#define RTTI_LOG_WARN(...) RTTI_LOG_PRINT("[WARNING] " __VA_OPT__(,) __VA_ARGS__)

namespace transformer::rtti::dbg
{

class Log
{
public:
	template<typename... Args>
	static void print(const Args&... args) noexcept
	{
		(std::cout << ... << args);
	}
};

}

#endif // !__TRANSFORMER_RTTI_DBG_LOG_H__
