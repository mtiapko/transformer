#ifndef __TRANSFORMER_CONFIG_H__
#define __TRANSFORMER_CONFIG_H__

#include <llvm/Support/CommandLine.h>

namespace transformer
{

template<size_t N>
struct FormatedArgString
{
	char data[N] {};

	constexpr FormatedArgString(const char (&str)[N])
	{
		for (size_t i = 0; i < N - 1; ++i)
			data[i] = (str[i] != '_' ? str[i] : '-');
	};
};

template<FormatedArgString Str>
constexpr auto& operator"" _fmt_arg()
{
	return Str;
}

#define CMD_LINE_ARG(arg, arg_desc) \
	static inline llvm::cl::opt<std::string> arg##_opt { \
		#arg##_fmt_arg.data, \
		llvm::cl::desc(arg_desc), \
		llvm::cl::cat(options_category) \
	}

#define CMD_LINE_ARG_ALIAS(arg, arg_alias) \
	static inline llvm::cl::alias arg##_short_opt { \
		arg_alias, \
		llvm:: cl::desc("Alias for --" #arg##_fmt_arg.data), \
		llvm::cl::aliasopt(arg##_opt) \
	}

#define CMD_LINE_ARG_WITH_ALIAS(arg, arg_alias, arg_desc) \
	CMD_LINE_ARG(arg, arg_desc); \
	CMD_LINE_ARG_ALIAS(arg, arg_alias)

class Config
{
public:
	static inline llvm::cl::OptionCategory options_category { "Transformer options" };

public:
	CMD_LINE_ARG_WITH_ALIAS(tmpl_file_path, "t", "Template file path");
};

}

#undef CMD_LINE_ARG
#undef CMD_LINE_ARG_ALIAS
#undef CMD_LINE_ARG_WITH_ALIAS

#endif // !__TRANSFORMER_CONFIG_H__
