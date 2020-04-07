#include "rtti/Variant.h"

#include "rect.h"

struct S : public rtti::ObjectType<S>
{
	RTTI_OBJECT_TYPE_BODY;

	int int_field;
	float float_field;
};

void set(rtti::Object* obj) noexcept
{
	auto info = obj->object_type_info();
	for (const auto& f: info->fields_info) {
		obj->object_field(f.name) = { { "x", 7 } };
		obj->object_field(f.name) = "Hello!";
	}
}

int main() noexcept
{
	S s;
	s.object_field("int_field") = 1337;

	const auto info = rtti::DB::get_object_type_info<Rect>();
	if (info == nullptr) {
		std::clog << "type info for Rect is null!\n";
		return EXIT_FAILURE;
	}

	std::clog << "info.name = " << info->name << '\n';
	for (const auto& f: info->fields_info) {
		std::clog << "\tinfo.field.name = " << f.name << '\n';
	}

	Rect r {};

	std::clog << "\t ------ BEG ------\n";

	r.object_field("arr")  = { 1.1f, 2.2f, 3.123f };
	r.object_field("arr2") = { 2.1f, 3.2f, 4.123f, -5, 7u };

	r.object_field("str_arr") = { 'H', 'e', 'l', 'l', 'o', '!', '\0' };

	std::clog << "\t ------ END ------\n";

	r.print();

	std::clog << "\t-----===== Test #0 =====-----\n";
	set(&r);
	r.print();

	std::clog << "\t-----===== Test #1 =====-----\n";
	r.object_field("top_left") = { { "x", 1 }, { "z", -99 } };
	r.object_field("top_left") = 123;

	r.object_field("bot_right") = { { "x", 12345.f } };
	r.object_field("bot_right").subfield("x") = -1.5f;

	r.object_field("name") = "Hello, Rect!";

	r.print();

	std::clog << "\t-----===== Test #2 =====-----\n";
	r.object_set_value(
		{
			{ "bot_right", {
				{ "point", {
					{ "y",   234.345f },
					{ "zzz", -1       }
				} }
			} },
			{ "top_left", {
				{ "x", 123u },
				{ "z", 3.0  },
				{ "y", 5.1f },
			} },
			{ "name", "New name from " __FILE__ },
			{ "some name", 123 - 456 }
		}
	);

	r.print();

	std::clog << r.object_get_value().dump(4, ' ') << '\n';

	return EXIT_SUCCESS;
}
