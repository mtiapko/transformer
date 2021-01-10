#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iomanip> // TODO(FiTH): remove

#include "transformer/AST/Action.h"
#include "transformer/Config.h"

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
	auto opt_parser = clang::tooling::CommonOptionsParser::create(argc, argv,
		transformer::Config::options_category, llvm::cl::NumOccurrencesFlag::ZeroOrMore);

	if (!opt_parser)
		return EXIT_FAILURE;

	clang::tooling::ClangTool transformer_tool(opt_parser->getCompilations(), opt_parser->getSourcePathList());

	inja::json tmpl_content;
	auto action = std::make_unique<SimpleFrontendActionFactory>(tmpl_content);

	int ret = transformer_tool.run(action.get());
	std::cout << std::setw(4) << tmpl_content << '\n';

	return ret;
}
