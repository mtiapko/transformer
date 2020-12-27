#include <getopt.h>
#include <iostream>
#include "transformer/Exception.h"
#include "transformer/Config.h"

namespace transformer
{

/* static */ void Config::print_basic_usage(std::string_view program_path) noexcept
{
	std::cerr
		<< "Usage: " << program_path << " <tmpl-file-path> <src-file-path> [options]...\n"
		<< "Try '" << program_path << " --help' for more information.\n";
}

/* static */ void Config::print_help(std::string_view program_path) noexcept
{
	std::cerr << R"(
Usage:
    )" << program_path << R"( <tmpl-file-path> <src-file-path> [options]... -- [compiler args]...

Options:
    -t --tmpl-file-path          template file path
    -s --src-file-path           source file path
    -o --out-file-path           output file path. default: stdout
       --append-output           append output if writing to file
    -a --tmpl-arg                template argument to use when rendering.
                                 Can be used several times. Ex.: -a key or -a key:val
    -j --tmpl-json-arg           JSON string as argument for template.
                                 Can be used several times
       --no-strip-first-newline  not strip first newline after statement
       --strip-beg-whitespaces   strip tab/spaces from beginning of a line
       --dump-tmpl-content       dump template content as JSON (dry run)
    -c --compiler-args (or --)   all subsequent arguments will be sent
                                 to the compiler
    -h --help                    show help (this) message

Bug report: https://github.com/mtiapko/transformer/issues
)";
}

Config::Config(int argc, char* const argv[])
{
	// TODO(FiTH): multi-template? or multi-source? or both (but how)?
	const char* const short_opts = "-t:s:o:a:j:c::h";
	int long_opts_flag = 0;
	const option long_opts[] = {
		{ "tmpl-file-path",         required_argument, nullptr, 't' },
		{ "src-file-path",          required_argument, nullptr, 's' },
		{ "out-file-path",          required_argument, nullptr, 'o' },
		{ "append-output",          no_argument,       &long_opts_flag, 'a' },
		{ "tmpl-arg",               required_argument, nullptr, 'a' },
		{ "tmpl-json-arg",          required_argument, nullptr, 'j' },
		{ "no-strip-first-newline", no_argument,       &long_opts_flag, 'n' },
		{ "strip-beg-whitespaces",  no_argument,       &long_opts_flag, 'w' },
		{ "compiler-args",          optional_argument, nullptr, 'c' },
		{ "dump-tmpl-content",      no_argument,       &long_opts_flag, 'd' },
		{ "help",                   no_argument,       nullptr, 'h' },
		{}
	};

	if (argc == 1) {
		Config::print_basic_usage(argv[0]);
		std::exit(EXIT_SUCCESS);
	}

	decltype(optind) last_parsed_arg_index = -1;

	bool continue_args_parsing = true;
	while (continue_args_parsing) {
		last_parsed_arg_index = optind;
		const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
		if (opt == -1)
			break;

		switch (opt) {
			case 't': m_tmpl_file_path = optarg; break;
			case 's': m_src_file_path = optarg; break;
			case 'o': m_out_file_path = optarg; break;
			case 'j': [[fallthrough]];
			case 'a': {
				auto& tmpl_args = (opt == 'a'
					? m_tmpl_args
					: m_tmpl_json_args);

				const auto arg = std::string_view { optarg };
				if (const auto delim_pos = arg.find(':'); delim_pos != std::string::npos) {
					const size_t first_pos = 0;
					const size_t first_count = delim_pos;

					const size_t second_pos = delim_pos + 1;
					const size_t second_count = arg.size() - second_pos;

					tmpl_args.emplace_back(
						std::string_view { arg.data() + first_pos, first_count },
						std::string_view { arg.data() + second_pos, second_count }
					);
				} else {
					tmpl_args.emplace_back(arg, std::string_view { "true" });
				}

				break;
			}
			case 'c':
				continue_args_parsing = false;
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
			case 1:
				if      (m_tmpl_file_path.empty()) m_tmpl_file_path = optarg;
				else if (m_src_file_path.empty())  m_src_file_path  = optarg;
				else throw TF_EXCEPTION("Unexpected argument: ", optarg);

				break;
			default:
				Config::print_help(argv[0]);
				if (opt != 'h')
					throw TF_EXCEPTION("Failed to parse command line arguments");

				std::exit(EXIT_SUCCESS);
		}
	}

	// Initial value of last_parsed_arg_index must be -1,
	// so command 'transformer -- -v' will be valid.
	if (++last_parsed_arg_index < argc) {
		m_compiler_argc = argc - last_parsed_arg_index;
		m_compiler_argv = argv + last_parsed_arg_index; // TODO(FiTH): argv[argc] == nullptr?
	}
}

}
