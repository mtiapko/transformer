#include <iostream>
#include "transformer/Generator.h"
#include "transformer/File.h"

namespace trans
{

/* static */ void Generator::tmpl_content_visitor(mstch::node& obj) noexcept
{
	if (auto arr = boost::get<mstch::array>(&obj); arr != nullptr)
		return Generator::tmpl_content_visitor(arr);

	if (auto map = boost::get<mstch::map>(&obj); map != nullptr)
		return Generator::tmpl_content_visitor(map);
}

/* static */ void Generator::tmpl_content_visitor(mstch::array* arr) noexcept
{
	for (auto& obj: *arr)
		Generator::tmpl_content_visitor(obj);
}

/* static */ void Generator::tmpl_content_visitor(mstch::map* map) noexcept
{
	for (auto& [name, node]: *map) {
		if (auto arr = boost::get<mstch::array>(&node); arr != nullptr) {
			map->emplace(name + std::string("_count"), std::to_string(arr->size()));
			if (!arr->empty()) {
				if (auto first = boost::get<mstch::map>(&arr->front()); first != nullptr)
					first->emplace(name + std::string("_is_first_elem"), true);

				if (auto last = boost::get<mstch::map>(&arr->back()); last != nullptr)
					last->emplace(name + std::string("_is_last_elem"), true);
			} else {
				map->emplace(name + std::string("_is_empty"), true);
			}
		}

		Generator::tmpl_content_visitor(node);
	}
}

/* static */ mstch::map Generator::create_class_tmpl_content(const Parser& p, const Class& c) noexcept
{
	mstch::array base_classes_list;
	base_classes_list.reserve(c.base_classes().size());
	for (auto base_id: c.base_classes()) {
		const auto& b = p.classes().entities()[base_id];
		base_classes_list.emplace_back(Generator::create_base_class_tmpl_content(b));
	}

	mstch::array fields_list;
	fields_list.reserve(c.fields().size());
	Generator::create_recursive_class_fields_tmpl_content(p, c, fields_list);

	return {
		{ "class_file_path",    c.file_path()                },
		{ "class_full_name",    c.full_name()                },
		{ "class_name",         c.name()                     },
		{ "class_base_classes", std::move(base_classes_list) },
		{ "class_fields",       std::move(fields_list)       }
	};
}

/* static */ mstch::map Generator::create_base_class_tmpl_content(const Class& b) noexcept
{
	return {
		{ "base_class_file_path", b.file_path() },
		{ "base_class_full_name", b.full_name() },
		{ "base_class_name",      b.name()      }
	};
}

/* static */ void Generator::create_recursive_class_fields_tmpl_content(const Parser& p, const Class& c,
	mstch::array& fields_list) noexcept
{
	for (auto base_id: c.base_classes()) {
		const auto& b = p.classes().entities()[base_id];
		Generator::create_recursive_class_fields_tmpl_content(p, b, fields_list);
	}

	for (const auto& f: c.fields())
		fields_list.emplace_back(Generator::create_class_field_tmpl_content(f));
}

/* static */ mstch::map Generator::create_class_field_tmpl_content(const ClassField& f) noexcept
{
	return {
		{ "field_file_path", f.file_path() },
		{ "field_full_name", f.full_name() },
		{ "field_name",      f.name()      },
		{ "field_type",      f.type()      }
	};
}

/* static */ mstch::map Generator::create_enum_tmpl_content(const Enum& e) noexcept
{
	mstch::array consts_list;
	consts_list.reserve(e.consts().size());
	for (const auto& c: e.consts())
		consts_list.emplace_back(Generator::create_enum_const_tmpl_content(e, c));

	return {
		{ "enum_file_path",    e.file_path()          },
		{ "enum_full_name",    e.full_name()          },
		{ "enum_name",         e.name()               },
		{ "enum_integer_type", e.integer_type()       },
		{ "enum_consts",       std::move(consts_list) }
	};
}

/* static */ mstch::map Generator::create_enum_const_tmpl_content(const Enum& e, const EnumConst& c) noexcept
{
	auto value = (e.is_unsigned_integer_type()
		? std::to_string(c.unsigned_value())
		: std::to_string(c.signed_value()));

	return {
		{ "const_file_path", c.file_path()    },
		{ "const_full_name", c.full_name()    },
		{ "const_name",      c.name()         },
		{ "const_value",     std::move(value) }
	};
}

/* static */ mstch::map Generator::create_tmpl_content(const Config& cfg, const Parser& parser) noexcept
{
	mstch::array classes_list;
	classes_list.reserve(parser.classes().entities().size());
	for (const auto& c: parser.classes().entities()) {
		if (!c.is_from_main_file())
			continue;

		classes_list.emplace_back(Generator::create_class_tmpl_content(parser, c));
	}

	mstch::array enums_list;
	enums_list.reserve(parser.enums().entities().size());
	for (const auto& e: parser.enums().entities()) {
		if (!e.is_from_main_file())
			continue;

		enums_list.emplace_back(Generator::create_enum_tmpl_content(e));
	}

	mstch::map tmpl_content {
		{ "file_path", cfg.src_file_path().string() },
		{ "classes",   std::move(classes_list)      },
		{ "enums",     std::move(enums_list)        }
	};

	for (const auto& [name, value]: cfg.tmpl_args())
		tmpl_content.emplace(name, std::string { value });

	return tmpl_content;
}

Generator::Generator(const Config& cfg, const Parser& parser)
{
	auto tmpl = File::read(cfg.tmpl_file_path());
	auto tmpl_content = Generator::create_tmpl_content(cfg, parser);
	Generator::tmpl_content_visitor(&tmpl_content);

	mstch::config::escape = [](const std::string& str) { return str; };
	auto res = mstch::render(tmpl, tmpl_content);

	if (!cfg.out_file_path().empty()) File::write(cfg.out_file_path(), res);
	else std::cout << res;
}

}
