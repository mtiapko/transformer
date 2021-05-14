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

#define CMD_LINE_ARG(type, arg, arg_desc, ...) \
	static inline llvm::cl::opt<type> arg##_opt { \
		#arg##_fmt_arg.data, \
		llvm::cl::desc(arg_desc), \
		llvm::cl::cat(options_category) \
		__VA_OPT__(,) __VA_ARGS__\
	}

#define CMD_LINE_STR_ARG(arg, arg_desc, ...) CMD_LINE_ARG(std::string, arg, arg_desc __VA_OPT__(,) __VA_ARGS__)

#define CMD_LINE_ARG_ALIAS(arg, arg_alias) \
	static inline llvm::cl::alias arg##_short_opt { \
		arg_alias, \
		llvm::cl::desc("Alias for --" #arg##_fmt_arg.data), \
		llvm::cl::aliasopt(arg##_opt) \
	}

#define CMD_LINE_STR_ARG_WITH_ALIAS(arg, arg_alias, arg_desc, ...) \
	CMD_LINE_STR_ARG(arg, arg_desc __VA_OPT__(,) __VA_ARGS__); \
	CMD_LINE_ARG_ALIAS(arg, arg_alias)

class Config
{
public:
	static inline llvm::cl::OptionCategory options_category { "Transformer options" };

// NOTE(FiTH): https://llvm.org/docs/CommandLine.html
public:
	CMD_LINE_STR_ARG_WITH_ALIAS(tmpl_file_path,           "t", "Template file path", llvm::cl::value_desc("file path"), llvm::cl::Required);
	CMD_LINE_STR_ARG_WITH_ALIAS(src_file_path,            "s", "Source file path",   llvm::cl::value_desc("file path")); // TODO(FiTH): opt_parser->getCompilations()
	CMD_LINE_STR_ARG_WITH_ALIAS(output_file_path,         "o", "Output file path",   llvm::cl::value_desc("file path"));
	CMD_LINE_ARG(bool,          gen_content_for_includes,      "Generate content for included files? (disabled by default)");
	CMD_LINE_ARG(bool,          dump_generated_content,        "Dump generated content for template");

	// TODO(FiTH): rename?
	static inline llvm::cl::list<std::string> content_flag_opt { "add-content-flag" };
	static inline llvm::cl::list<std::string> content_json_opt { "add-content-json" };
};

}

#undef CMD_LINE_ARG
#undef CMD_LINE_ARG_ALIAS
#undef CMD_LINE_ARG_WITH_ALIAS

#endif // !__TRANSFORMER_CONFIG_H__
