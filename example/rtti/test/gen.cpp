#include "rtti/Variant.h"
#include "rtti/DB.h"

template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Point2D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Point3D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Rect>() noexcept;

#include "rect.h" // TODO: add file name var

namespace rtti
{

/* * * * * * * * * * * * * * * * * * * * * * *

		Point2D

* * * * * * * * * * * * * * * * * * * * * * * */
template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point2D>() noexcept
{
	/* x */
	auto x_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto x_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto x_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point2D*)parent_ptr)->x;
	};

	/* y */
	auto y_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto y_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto y_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point2D*)parent_ptr)->y;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Point2D",
			/* setter */ (type_info_setter_t)(void (*)(Point2D*, const Variant&) noexcept)
				&DB::object_set_value,
			/* getter */ (type_info_getter_t)(void (*)(const Point2D*, Variant&) noexcept)
				&DB::object_get_value
		},
		/* ObjectTypeInfo */
		/* fields_info */
		{
			{
				/* type_info */
				{
					/* name   */ "float",
					/* setter */ x_setter,
					/* getter */ x_getter
				},
				/* name                 */ "x",
				/* get_addr             */ x_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float>
			},
			{
				/* type_info */
				{
					/* name   */ "float",
					/* setter */ y_setter,
					/* getter */ y_getter
				},
				/* name                 */ "y",
				/* get_addr             */ y_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float>
			},
		},
		/* fields_info_map */
		{
			{ "x", &info.fields_info[0] },
			{ "y", &info.fields_info[1] },
		}
	};

	return info;
}

namespace
{

static const ObjectTypeInfo info_0 = DB::make_object_type_info<Point2D>();

} /* namespace <anonymous> */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point2D>() noexcept
{
	return &info_0;
}


/* * * * * * * * * * * * * * * * * * * * * * *

		Point3D

* * * * * * * * * * * * * * * * * * * * * * * */
template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point3D>() noexcept
{
	/* x */
	auto x_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto x_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto x_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->x;
	};

	/* y */
	auto y_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto y_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto y_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->y;
	};

	/* z */
	auto z_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto z_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto z_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->z;
	};

	/* point */
	auto point_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto point_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto point_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->point;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Point3D",
			/* setter */ (type_info_setter_t)(void (*)(Point3D*, const Variant&) noexcept)
				&DB::object_set_value,
			/* getter */ (type_info_getter_t)(void (*)(const Point3D*, Variant&) noexcept)
				&DB::object_get_value
		},
		/* ObjectTypeInfo */
		/* fields_info */
		{
			{
				/* type_info */
				{
					/* name   */ "float",
					/* setter */ x_setter,
					/* getter */ x_getter
				},
				/* name                 */ "x",
				/* get_addr             */ x_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float>
			},
			{
				/* type_info */
				{
					/* name   */ "float",
					/* setter */ y_setter,
					/* getter */ y_getter
				},
				/* name                 */ "y",
				/* get_addr             */ y_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float>
			},
			{
				/* type_info */
				{
					/* name   */ "float",
					/* setter */ z_setter,
					/* getter */ z_getter
				},
				/* name                 */ "z",
				/* get_addr             */ z_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float>
			},
			{
				/* type_info */
				{
					/* name   */ "Point2D",
					/* setter */ point_setter,
					/* getter */ point_getter
				},
				/* name                 */ "point",
				/* get_addr             */ point_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<Point2D>
			},
		},
		/* fields_info_map */
		{
			{ "x", &info.fields_info[0] },
			{ "y", &info.fields_info[1] },
			{ "z", &info.fields_info[2] },
			{ "point", &info.fields_info[3] },
		}
	};

	return info;
}

namespace
{

static const ObjectTypeInfo info_1 = DB::make_object_type_info<Point3D>();

} /* namespace <anonymous> */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point3D>() noexcept
{
	return &info_1;
}


/* * * * * * * * * * * * * * * * * * * * * * *

		Rect

* * * * * * * * * * * * * * * * * * * * * * * */
template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Rect>() noexcept
{
	/* name */
	auto name_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto name_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto name_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->name;
	};

	/* top_left */
	auto top_left_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto top_left_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto top_left_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->top_left;
	};

	/* bot_right */
	auto bot_right_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto bot_right_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto bot_right_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->bot_right;
	};

	/* arr */
	auto arr_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto arr_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto arr_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->arr;
	};

	/* arr2 */
	auto arr2_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto arr2_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto arr2_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->arr2;
	};

	/* str_arr */
	auto str_arr_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto str_arr_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_UNUSED(ptr);
		RTTI_UNUSED(value);
	};

	auto str_arr_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->str_arr;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Rect",
			/* setter */ (type_info_setter_t)(void (*)(Rect*, const Variant&) noexcept)
				&DB::object_set_value,
			/* getter */ (type_info_getter_t)(void (*)(const Rect*, Variant&) noexcept)
				&DB::object_get_value
		},
		/* ObjectTypeInfo */
		/* fields_info */
		{
			{
				/* type_info */
				{
					/* name   */ "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
					/* setter */ name_setter,
					/* getter */ name_getter
				},
				/* name                 */ "name",
				/* get_addr             */ name_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >>
			},
			{
				/* type_info */
				{
					/* name   */ "Point3D",
					/* setter */ top_left_setter,
					/* getter */ top_left_getter
				},
				/* name                 */ "top_left",
				/* get_addr             */ top_left_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<Point3D>
			},
			{
				/* type_info */
				{
					/* name   */ "Point3D",
					/* setter */ bot_right_setter,
					/* getter */ bot_right_getter
				},
				/* name                 */ "bot_right",
				/* get_addr             */ bot_right_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<Point3D>
			},
			{
				/* type_info */
				{
					/* name   */ "std::vector<float, std::allocator<float> >",
					/* setter */ arr_setter,
					/* getter */ arr_getter
				},
				/* name                 */ "arr",
				/* get_addr             */ arr_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<std::vector<float, std::allocator<float> >>
			},
			{
				/* type_info */
				{
					/* name   */ "float [5]",
					/* setter */ arr2_setter,
					/* getter */ arr2_getter
				},
				/* name                 */ "arr2",
				/* get_addr             */ arr2_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<float [5]>
			},
			{
				/* type_info */
				{
					/* name   */ "char [16]",
					/* setter */ str_arr_setter,
					/* getter */ str_arr_getter
				},
				/* name                 */ "str_arr",
				/* get_addr             */ str_arr_get_addr,
				/* get_object_type_info */ &DB::get_object_type_info<char [16]>
			},
		},
		/* fields_info_map */
		{
			{ "name", &info.fields_info[0] },
			{ "top_left", &info.fields_info[1] },
			{ "bot_right", &info.fields_info[2] },
			{ "arr", &info.fields_info[3] },
			{ "arr2", &info.fields_info[4] },
			{ "str_arr", &info.fields_info[5] },
		}
	};

	return info;
}

namespace
{

static const ObjectTypeInfo info_2 = DB::make_object_type_info<Rect>();

} /* namespace <anonymous> */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Rect>() noexcept
{
	return &info_2;
}

} /* namespace rtti */
