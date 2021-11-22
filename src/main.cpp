#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <iomanip> // TODO(FiTH): remove

#include "transformer/AST/Action.h"
#include "transformer/Config.h"
#include "transformer/File.h"
#include "transformer/Log.h"

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
	// TODO(FiTH): accumulate many sources to avoid type info duplications of std type

	// setup parser (parse arguments)
	auto opt_parser = clang::tooling::CommonOptionsParser::create(argc, argv,
		transformer::Config::options_category, llvm::cl::NumOccurrencesFlag::ZeroOrMore);

	if (!opt_parser)
		return EXIT_FAILURE;

	clang::tooling::ClangTool transformer_tool(opt_parser->getCompilations(), opt_parser->getSourcePathList());

	// create template content
	inja::json tmpl_content;
	for (const auto& flag: transformer::Config::content_flag_opt)
		tmpl_content[flag] = true;

	for (const auto& json_str: transformer::Config::content_json_opt) {
		auto json = inja::json::parse(json_str);
		if (json.is_object() == false) {
			TF_LOG_ERROR("Argument '", transformer::Config::content_json_opt.ArgStr, "' must be an object: ", json_str);
			return EXIT_FAILURE;
		}

		for (auto& [key, val]: json.items())
			tmpl_content[key] = std::move(val);
	}

	// parse cpp code
	auto action = std::make_unique<SimpleFrontendActionFactory>(tmpl_content);
	int ret = transformer_tool.run(action.get());
	if (ret != EXIT_SUCCESS)
		return ret;

	// dump generated content
	if (transformer::Config::dump_generated_content_opt) {
		std::cout << std::setw(2) << tmpl_content << '\n';
		return EXIT_SUCCESS;
	}

	// generate output
	auto env = inja::Environment {};
	env.set_trim_blocks(true);
	// env.set_lstrip_blocks(...);

	// add custom callbacks
	env.add_callback("valueOr", 2, [](inja::Arguments& args)
	{
		// TODO(FiTH): WARNING! This is copy of array???
		if (args.at(0)->is_null())
			return *args.at(1);

		return *args.at(0);
	});

	// TODO(FiTH): add "file:line" format?
	env.add_callback("log", 1, [](inja::Arguments& args)
	{
		std::clog << "INJA LOG> " << *args[0] << '\n';
		return nullptr;
	});

	// add named sub templates
	for (const auto& named_sub_tmpl: transformer::Config::named_sub_template_list) {
		const auto delim_pos = named_sub_tmpl.find(':');
		assert(delim_pos != std::string::npos); // TODO(FiTH)

		const std::string_view sub_tmpl_name = { named_sub_tmpl.begin(), named_sub_tmpl.begin() + delim_pos };
		const std::string_view sub_tmpl_path = { named_sub_tmpl.begin() + delim_pos + 1, named_sub_tmpl.end() };

		auto sub_tmpl = transformer::File::read(sub_tmpl_path);
		auto compiled_sub_tmpl = env.parse(sub_tmpl);

		// TODO(FiTH): this method has a bad API...
		env.include_template(std::string(sub_tmpl_name), compiled_sub_tmpl);
	}

	auto tmpl = transformer::File::read(transformer::Config::tmpl_file_path_opt.getValue());
	auto compiled_tmpl = env.parse(tmpl);

	auto output = env.render(compiled_tmpl, tmpl_content);
	if (transformer::Config::output_file_path_opt.empty() == false)
		transformer::File::write(transformer::Config::output_file_path_opt.getValue(), output);
	else
		std::cout << output;

	return ret;
}
