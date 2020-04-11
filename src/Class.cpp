#include "transformer/Parser.h"
#include "transformer/Util.h"

namespace transformer
{

/* static */ CXChildVisitResult Class::visitor(CXCursor cur, CXCursor /* parent */,
	CXClientData closure) noexcept
{
	auto [parser, c] = *static_cast<std::pair<Parser*, Class*>*>(closure);
	switch (clang_getCursorKind(cur)) {
		case CXCursor_CXXBaseSpecifier: {
			cur = clang_getCursorDefinition(cur);
			if (clang_Cursor_isAnonymous(cur))
				break;

			auto full_name = Util::get_full_name(cur);
			if (full_name.empty())
				break;

			auto& classes = parser->classes();
			const auto& base = classes.add(Util::get_full_name(cur), parser, cur);
			const auto base_id = std::distance<const Class*>(classes.entities().data(), &base);
			c->add_base_class(base_id);
			break;
		}

		case CXCursor_FieldDecl: c->add_field(cur); break;
		case CXCursor_CXXMethod: c->add_method(cur); break;
		default: break;
	}

	return CXChildVisit_Continue;
}

Class::Class(Parser* parser, CXCursor cur, std::string full_name) noexcept
	: Entity(cur, std::move(full_name))
{
	auto closure = std::make_pair(parser, this);
	clang_visitChildren(cur, Class::visitor, &closure);
}

}
