#ifndef __TRANSFORMER_GENERATOR_H__
#define __TRANSFORMER_GENERATOR_H__

#include "transformer/Parser.h"
#include "inja/inja.hpp"

namespace transformer
{

class Generator
{
private:
	static inja::json create_entity_tmpl_content(const Entity& e) noexcept;
	static inja::json create_entity_tmpl_content(const Entity& e, inja::json content) noexcept;
	static inja::json create_class_tmpl_content(const Parser& p, const Class& c) noexcept;
	static void create_recursive_class_fields_tmpl_content(const Parser& p, const Class& c,
		std::vector<inja::json>& fields_list) noexcept;
	static inja::json create_class_field_tmpl_content(const ClassField& f) noexcept;
	static inja::json create_enum_tmpl_content(const Enum& e) noexcept;
	static inja::json create_enum_const_tmpl_content(const Enum& e, const EnumConst& c) noexcept;
	static inja::json create_tmpl_content(const Config& cfg, const Parser& parser) noexcept;

public:
	Generator(const Config& cfg, const Parser& parser);
};

}

#endif // !__TRANSFORMER_GENERATOR_H__
