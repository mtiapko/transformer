#ifndef __TRANSFORMER_GENERATOR_H__
#define __TRANSFORMER_GENERATOR_H__

#include "transformer/Parser.h"
#include "mstch/mstch.hpp"

namespace trans
{

class Generator
{
private:
	static void tmpl_content_visitor(mstch::node& obj) noexcept;
	static void tmpl_content_visitor(mstch::array* arr) noexcept;
	static void tmpl_content_visitor(mstch::map* map) noexcept;

	static mstch::map create_class_tmpl_content(const Parser& p, const Class& c) noexcept;
	static mstch::map create_base_class_tmpl_content(const Class& b) noexcept;
	static void create_recursive_class_fields_tmpl_content(const Parser& p, const Class& c,
		mstch::array& fields_list) noexcept;
	static mstch::map create_class_field_tmpl_content(const ClassField& f) noexcept;
	static mstch::map create_enum_tmpl_content(const Enum& e) noexcept;
	static mstch::map create_enum_const_tmpl_content(const Enum& e, const EnumConst& c) noexcept;
	static mstch::map create_tmpl_content(const Config& cfg, const Parser& parser) noexcept;

public:
	Generator(const Config& cfg, const Parser& parser);
};

}

#endif // !__TRANSFORMER_GENERATOR_H__
