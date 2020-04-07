#ifndef __RTTI_ASSERT_H__
#define __RTTI_ASSERT_H__

#include "rtti/Log.h"

/*
	TODO(FiTH): this is bad idea to leave assert and log defines empty in release build

	------------ EXAMPLE.cpp --------------
	if (...)
		RTTI_ASSERT(...) or RTTI_PRINT(...)

	do_something_important();
	---------------------------------------

	If RTTI_ASSERT empty - something important will never happen
*/

/* RTTI assert */

#ifndef RTTI_DISABLE_ASSERT

#include <stdlib.h>

#ifndef RTTI_DISABLE_ASSERT_EXIT_ON_FAIL
#	define RTTI_ASSERT_FAIL_ACTION std::exit(EXIT_FAILURE)
#else
#	define RTTI_ASSERT_FAIL_ACTION /* empty */
#endif // !RTTI_DISABLE_ASSERT_EXIT_ON_FAIL

#ifndef RTTI_DISABLE_ASSERT_PRINT
#	define RTTI_ASSERT_PRINT(x, ...) RTTI_PRINT_ERR("[ASSERT!] ", "Assert '" x "' failed. ", ##__VA_ARGS__)
#else
#	define RTTI_ASSERT_PRINT(...) /* empty */
#endif // !RTTI_DISABLE_ASSERT_PRINT

#define RTTI_ASSERT_IMPL(x, action, ...) \
	if (!(x)) { \
		RTTI_ASSERT_PRINT(#x, __VA_ARGS__); \
		RTTI_ASSERT_FAIL_ACTION; \
		\
		action; \
	}

#define RTTI_ASSERT_CONTINUE(x, ...) RTTI_ASSERT_IMPL(x, continue, __VA_ARGS__)
#define RTTI_ASSERT_RET(x, ret, ...) RTTI_ASSERT_IMPL(x, return ret, __VA_ARGS__)
#define RTTI_ASSERT(x, ...)          RTTI_ASSERT_RET(x, /* no ret */, __VA_ARGS__)

#else
#	define RTTI_ASSERT_RET(...) /* empty */
#	define RTTI_ASSERT(...)     /* empty */
#endif // !RTTI_DISABLE_ASSERT

/* RTTI warning assert */

#ifndef RTTI_DISABLE_WARN_ASSERT

#define RTTI_WARN_ASSERT(x, ...) \
	if (!(x)) { \
		RTTI_LOG_WARN_IMPL("Warning assert '" #x "' failed. ", __VA_ARGS__); \
	}

#else
#	define RTTI_WARN_ASSERT(...) /* empty */
#endif // !RTTI_DISABLE_WARN_ASSERT

#endif // !__RTTI_ASSERT_H__
