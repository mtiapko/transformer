#include <iostream>
#include "transformer/Generator.h"
#include "transformer/File.h"
#include "transformer/Log.h"

namespace transformer
{

/* static */ inja::json Generator::create_entity_tmpl_content(const Entity& e) noexcept
{
	return {
		{ /* entity_ */ "file_path",  e.file_path()  },
		{ /* entity_ */ "full_name",  e.full_name()  },
		{ /* entity_ */ "name",       e.name()       },
		{ /* entity_ */ "attributes", e.attributes() }
	};
}

/* static */ inja::json Generator::create_entity_tmpl_content(const Entity& e, inja::json content) noexcept
{
	inja::json j = Generator::create_entity_tmpl_content(e);

	content.insert(j.begin(), j.end());
	return content;
}

/* static */ inja::json Generator::create_variable_tmpl_content(const Variable& v) noexcept
{
	inja::json j = Generator::create_entity_tmpl_content(v,
	{
		{ /* variable_ */ "type",            v.type()                              },
		/* ---------------------------------------------------------------------- */
		{ /* variable_ */ "is_builtin_type",          v.is_builtin_type()          },
		{ /* variable_ */ "is_pointer_type",          v.is_pointer_type()          },
		{ /* variable_ */ "is_lvalue_reference_type", v.is_lvalue_reference_type() },
		{ /* variable_ */ "is_rvalue_reference_type", v.is_rvalue_reference_type() },
		{ /* variable_ */ "is_array_type",            v.is_array_type()            },
		{ /* variable_ */ "is_enum_type",             v.is_enum_type()             }
	});

	if (v.is_array_type()) {
		j.emplace(/* variable_ */ "array_element_type", v.element_type());
		j.emplace(/* variable_ */ "is_incomplete_array", v.is_incomplete_array());

		if (!v.is_incomplete_array())
			j.emplace(/* variable_ */ "array_elements_count", v.elements_count());
	}

	return j;
}

/* static */ inja::json Generator::create_class_tmpl_content(const Parser& p, const Class& c) noexcept
{
	std::vector<inja::json> base_classes_list;
	base_classes_list.reserve(c.base_classes().size());
	for (auto base_id: c.base_classes()) {
		const auto& b = p.classes().entities()[base_id];
		base_classes_list.emplace_back(Generator::create_entity_tmpl_content(b));
	}

	std::vector<inja::json> fields_list;
	fields_list.reserve(c.fields().size()); /* NOTE(FiTH): reserve only for class field, w/o fields from base classes */
	Generator::create_recursive_class_fields_tmpl_content(p, c, fields_list);

	std::vector<inja::json> methods_list;
	for (const auto& m: c.methods())
		methods_list.emplace_back(Generator::create_class_method_tmpl_content(m));

	return Generator::create_entity_tmpl_content(c,
	{
		{ /* class_ */ "base_classes", std::move(base_classes_list) },
		{ /* class_ */ "fields",       std::move(fields_list)       },
		{ /* class_ */ "methods",      std::move(methods_list)      }
	});
}

/* static */ void Generator::create_recursive_class_fields_tmpl_content(const Parser& p, const Class& c,
	std::vector<inja::json>& fields_list) noexcept
{
	for (auto base_id: c.base_classes()) {
		const auto& b = p.classes().entities()[base_id];
		Generator::create_recursive_class_fields_tmpl_content(p, b, fields_list);
	}

	for (const auto& f: c.fields())
		fields_list.emplace_back(Generator::create_class_field_tmpl_content(f));
}

/* static */ inja::json Generator::create_class_field_tmpl_content(const ClassField& f) noexcept
{
	return Generator::create_variable_tmpl_content(f);
}

/* static */ inja::json Generator::create_class_method_tmpl_content(const ClassMethod& m) noexcept
{
	std::vector<inja::json> args_list;
	for (const auto& arg: m.args())
		args_list.emplace_back(Generator::create_variable_tmpl_content(arg));

	return Generator::create_entity_tmpl_content(m,
	{
		{ /* method_ */ "return_type", m.return_type()      },
		{ /* method_ */ "args",        std::move(args_list) },
		{ /* method_ */ "is_const",    m.is_const()         },
		{ /* method_ */ "is_static",   m.is_static()        }
	});
}

/* static */ inja::json Generator::create_enum_tmpl_content(const Enum& e) noexcept
{
	std::vector<inja::json> consts_list;
	consts_list.reserve(e.consts().size());
	for (const auto& c: e.consts())
		consts_list.emplace_back(Generator::create_enum_const_tmpl_content(e, c));

	return Generator::create_entity_tmpl_content(e,
	{
		{ /* enum_ */ "integer_type", e.integer_type()       },
		{ /* enum_ */ "consts",       std::move(consts_list) }
	});
}

/* static */ inja::json Generator::create_enum_const_tmpl_content(const Enum& e, const EnumConst& c) noexcept
{
	auto value = (e.is_unsigned_integer_type()
		? std::to_string(c.unsigned_value())
		: std::to_string(c.signed_value()));

	return Generator::create_entity_tmpl_content(e,
	{
		{ /* const_ */ "value", std::move(value) }
	});
}

/* static */ inja::json Generator::create_tmpl_content(const Config& cfg, const Parser& parser) noexcept
{
	std::vector<inja::json> classes_list;
	classes_list.reserve(parser.classes().entities().size());
	for (const auto& c: parser.classes().entities()) {
		if (!c.is_from_main_file())
			continue;

		classes_list.emplace_back(Generator::create_class_tmpl_content(parser, c));
	}

	std::vector<inja::json> enums_list;
	enums_list.reserve(parser.enums().entities().size());
	for (const auto& e: parser.enums().entities()) {
		if (!e.is_from_main_file())
			continue;

		enums_list.emplace_back(Generator::create_enum_tmpl_content(e));
	}

	// TODO: set to 'env' variable (env.classes, env.file_path...)?
	inja::json tmpl_content {
		{ "file_path", cfg.src_file_path().native() },
		{ "classes",   std::move(classes_list)      },
		{ "enums",     std::move(enums_list)        }
	};

	for (const auto& [name, value]: cfg.tmpl_args())
		tmpl_content.emplace(name, std::string { value });

	return tmpl_content;
}

Generator::Generator(const Config& cfg, const Parser& parser)
{
	auto tmpl_content = Generator::create_tmpl_content(cfg, parser);

	if (cfg.dump_tmpl_content()) {
		if (!cfg.out_file_path().empty()) {
			File::write(cfg.out_file_path(), tmpl_content.dump(4),
				/* extra new line */ true, cfg.append_output());
		} else {
			std::cout << std::setw(4) << tmpl_content << '\n';
		}

		return;
	}

	if (cfg.tmpl_file_path().empty())
		return;

	auto env = inja::Environment {};
	env.set_trim_blocks(!cfg.no_strip_first_newline());
	env.set_lstrip_blocks(cfg.strip_beg_whitespaces());

	auto tmpl = File::read(cfg.tmpl_file_path());
	try {
		auto compiled_tmpl = env.parse(tmpl);
		auto res = env.render(compiled_tmpl, tmpl_content);

		if (!cfg.out_file_path().empty()) {
			File::write(cfg.out_file_path(), res, /* extra new line */ false, cfg.append_output());
		} else {
			std::cout << res;
		}
	} catch (const inja::InjaError& e) {
		TF_PRINT_ERR("[ inja  ] ", cfg.tmpl_file_path().native(), ' ', e.what());

		std::exit(EXIT_FAILURE);
	}
}

}
