#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iomanip> // TODO(FiTH): remove

#include "transformer/AST/Action.h"
#include "transformer/Config.h"
#include "transformer/File.h"

namespace
{

class SimpleFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
	inja::json& m_tmpl_content;

public:
	SimpleFrontendActionFactory(inja::json& tmpl_content) noexcept
		: m_tmpl_content(tmpl_content)
	{}

	std::unique_ptr<clang::FrontendAction> create() noexcept override
		{ return std::make_unique<transformer::AST::Action>(m_tmpl_content); }
};

}

int main(int argc, const char* argv[])
{
	// TODO(FiTH): https://llvm.org/docs/CommandLine.html

	// setup parser (parse arguments)
	auto opt_parser = clang::tooling::CommonOptionsParser::create(argc, argv,
		transformer::Config::options_category, llvm::cl::NumOccurrencesFlag::ZeroOrMore);

	if (!opt_parser)
		return EXIT_FAILURE;

	clang::tooling::ClangTool transformer_tool(opt_parser->getCompilations(), opt_parser->getSourcePathList());

	inja::json tmpl_content;
	auto action = std::make_unique<SimpleFrontendActionFactory>(tmpl_content);

	// parse cpp code
	int ret = transformer_tool.run(action.get());
	if (ret != EXIT_SUCCESS)
		return ret;

	// generate output
	auto env = inja::Environment {};
	env.set_trim_blocks(true);

	// add custom callbacks
	env.add_callback("valueOr", 2, [](inja::Arguments& args)
	{
		if (args.at(0)->is_null())
			return *args.at(1);

		return *args.at(0);
	});

	auto tmpl = transformer::File::read(transformer::Config::tmpl_file_path_opt.getValue());
	auto compiled_tmpl = env.parse(tmpl);

	auto output = env.render(compiled_tmpl, tmpl_content);
	if (transformer::Config::output_file_path_opt.empty() == false)
		transformer::File::write(transformer::Config::output_file_path_opt.getValue(), output);
	else
		std::cout << output;

	return ret;
}
