#include <getopt.h>
#include <iostream>
#include "transformer/Exception.h"
#include "transformer/Config.h"

namespace transformer
{

/* static */ void Config::print_help()
{
	std::cerr << R"(
Usage:
    transformerformer <tmpl-file-path> <src-file-path> [options] [compiler args]...

Options:
    -t --tmpl-file-path          template file path
    -s --src-file-path           source file path
    -o --out-file-path           output file path. default: stdout
       --append-output           append output if writing to file
    -a --tmpl-arg                template argument to use when rendering
                                 can be used several times
       --no-strip-first-newline  not strip first newline after statement
       --strip-beg-whitespaces   strip tab/spaces from beginning of a line
       --dump-tmpl-content       dump template content as JSON (dry run)
    -c --compiler-args           all subsequent arguments will be sent
                                 to the compiler
    -h --help                    show help (this) message

Bug report: https://github.com/mtiapko/transformer/issues
)";
}

Config::Config(int argc, char* const argv[])
{
	// TODO(FiTH): multi-template? or multi-source? or both (but how)?
	const char* const short_opts = "t:s:o:a:c::h";
	int long_opts_flag = 0;
	const option long_opts[] = {
		{ "tmpl-file-path",         required_argument, nullptr, 't' },
		{ "src-file-path",          required_argument, nullptr, 's' },
		{ "out-file-path",          required_argument, nullptr, 'o' },
		{ "append-output",          no_argument,       &long_opts_flag, 'a' },
		{ "tmpl-arg",               required_argument, nullptr, 'a' },
		{ "no-strip-first-newline", no_argument,       &long_opts_flag, 'n' },
		{ "strip-beg-whitespaces",  no_argument,       &long_opts_flag, 'w' },
		{ "compiler-args",          optional_argument, nullptr, 'c' },
		{ "dump-tmpl-content",      no_argument,       &long_opts_flag, 'd' },
		{ "help",                   no_argument,       nullptr, 'h' },
		{}
	};

	int compiler_args_pos = -1;
	while (true) {
		const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
		if (opt == -1)
			break;

		switch (opt) {
			case 't': m_tmpl_file_path = optarg; break;
			case 's': m_src_file_path = optarg; break;
			case 'o': m_out_file_path = optarg; break;
			case 'a': {
				const auto arg = std::string_view { optarg };
				if (const auto delim_pos = arg.find(':'); delim_pos != std::string::npos) {
					const size_t first_pos = 0;
					const size_t first_count = delim_pos;

					const size_t second_pos = delim_pos + 1;
					const size_t second_count = arg.size() - second_pos;

					m_tmpl_args.emplace_back(
						std::string_view { arg.data() + first_pos, first_count },
						std::string_view { arg.data() + second_pos, second_count }
					);
				} else {
					m_tmpl_args.emplace_back(arg, std::string_view { "true" });
				}

				break;
			}
			case 'c':
				compiler_args_pos = optind;
				optind = argc;
				break;
			case 0:
				switch (long_opts_flag) {
					case 'a': m_append_output = true; break;
					case 'n': m_no_strip_first_newline = true; break;
					case 'w': m_strip_beg_whitespaces = true; break;
					case 'd': m_dump_tmpl_content = true; break;
					default:
						throw TF_EXCEPTION("Unexpected long option. Internal command line arguments parser error");
				}

				break;
			default:
				Config::print_help();
				if (opt != 'h')
					throw TF_EXCEPTION("Failed to parse command line arguments");

				std::exit(EXIT_SUCCESS);
		}
	}

	const auto final_optind = optind;
	if (m_tmpl_file_path.empty() && optind < argc)
		m_tmpl_file_path = argv[optind++];


	if (m_src_file_path.empty() && optind < argc)
		m_src_file_path = argv[optind++];

	if (compiler_args_pos != -1) {
		m_compiler_argc = argc - compiler_args_pos;
		m_compiler_argv = argv + compiler_args_pos - (optind - final_optind);
	} else {
		m_compiler_argc = argc - optind;
		m_compiler_argv = argv + optind;
	}
}

}
