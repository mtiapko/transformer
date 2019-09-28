#include <getopt.h>
#include <iostream>
#include "transformer/Exception.h"
#include "transformer/Config.h"

namespace trans
{

/* static */ void Config::print_help()
{
	std::cerr
		<< "Usage:\n"
		<< "    transformer <tmpl-file-path> <src-file-path> [options] [compiler args]...\n"
		<< '\n'
		<< "Options:\n"
		<< "    -t --tmpl-file-path  template file path\n"
		<< "    -s --src-file-path   source file path\n"
		<< "    -o --out-file-path   output file path. default: stdout\n"
		<< "    -a --tmpl-arg        template argument to use when rendering\n"
		<< "    -c --compiler-args   all subsequent arguments will be sent\n"
		<< "                         to the compiler\n"
		<< "    -h --help            show help (this) message\n"
		<< '\n';
}

Config::Config(int argc, char* const argv[])
{
	const char* const short_opts = "t:s:o:a:c::h";
	const option long_opts[] = {
		{ "tmpl-file-path", required_argument, nullptr, 't' },
		{ "src-file-path",  required_argument, nullptr, 's' },
		{ "out-file-path",  required_argument, nullptr, 'o' },
		{ "tmpl-arg",       required_argument, nullptr, 'a' },
		{ "compiler-args",  optional_argument, nullptr, 'c' },
		{ "help",           no_argument,       nullptr, 'h' },
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
			default:
				Config::print_help();
				if (opt != 'h')
					throw TRANS_EXCEPTION("Failed to parse command line arguments");

				std::exit(EXIT_SUCCESS);
		}
	}

	const auto final_optind = optind;
	if (m_tmpl_file_path.empty()) {
		if (optind >= argc)
			throw TRANS_EXCEPTION("Expected template file path as command line argument");

		m_tmpl_file_path = argv[optind++];
	}

	if (m_src_file_path.empty()) {
		if (optind >= argc)
			throw TRANS_EXCEPTION("Expected source file path as command line argument");

		m_src_file_path = argv[optind++];
	}

	if (compiler_args_pos != -1) {
		m_compiler_argc = argc - compiler_args_pos;
		m_compiler_argv = argv + compiler_args_pos - (optind - final_optind);
	} else {
		m_compiler_argc = argc - optind;
		m_compiler_argv = argv + optind;
	}
}

}
