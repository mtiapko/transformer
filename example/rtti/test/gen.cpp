#include "rtti/DB/Log.h"
#include "rtti/Variant.h"
#include "rtti/DB.h"

#ifndef RTTI_DISABLE_BUILTIN_ARRAY_TYPES_DEFAULT_SETTER_GETTER
#	include "rtti/DB/ArrayTypesDefaultSetterGetter.h"
#endif

#ifndef RTTI_DISABLE_STL_TYPES_DEFAULT_SETTER_GETTER
#	include "rtti/DB/STLTypesDefaultSetterGetter.h"
#endif

/* Forward declaration of explicit specialization for template functions */

/* Forward declaration of type */ struct Point2D; // TODO(FiTH): forward declare with struct/class
template<> /* static */ rtti::ObjectTypeInfo        rtti::DB::make_object_type_info<Point2D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<Point2D>() noexcept;

/* Forward declaration of type */ struct Point3D; // TODO(FiTH): forward declare with struct/class
template<> /* static */ rtti::ObjectTypeInfo        rtti::DB::make_object_type_info<Point3D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<Point3D>() noexcept;

/* Forward declaration of type */ struct Rect; // TODO(FiTH): forward declare with struct/class
template<> /* static */ rtti::ObjectTypeInfo        rtti::DB::make_object_type_info<Rect>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<Rect>() noexcept;

#include "rect.h" // TODO: add file name var

// TODO: add 'const' fields support (fail assert in setter)
// TODO: add enum support
// TODO: add bool
#define RTTI_BUILTIN_TYPE_SETTER(cond, type, name, variant_type_1, variant_type_2, variant_type_3) \
	if constexpr (std::cond##_v<type>) { \
		if (value.is_number_##variant_type_1()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_1##_t*>(); \
			ref = *value_data; \
		} else if (value.is_number_##variant_type_2()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_2##_t*>(); \
			ref = *value_data; \
		} else if (value.is_number_##variant_type_3()) { \
			auto value_data = value.get_ptr<const variant_##variant_type_3##_t*>(); \
			ref = *value_data; \
		} else if (value.is_null()) { \
			ref = {}; \
		} else { \
			RTTI_ASSERT(false, "Failed to assign variable '" #name "' of type '" #type "' value of type '", \
				value.type_name(), "': ", value); \
		} \
	}

namespace rtti
{

/* * * * * * * * * * * * * * * * * * * * * * *

		Point2D

* * * * * * * * * * * * * * * * * * * * * * * */

namespace
{

static const ObjectTypeInfo info_0 = DB::make_object_type_info<Point2D>();

} /* namespace <anonymous> */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point2D>() noexcept
{
	const type_info_setter_t Point2D_setter = [](void* obj, const Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		const Variant::object_t* object_value_ptr = object_value.get_ptr<const Variant::object_t*>();
		RTTI_ASSERT(object_value_ptr != nullptr, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(
			"Point2D", object_value));

		for (const auto& [key, val]: *object_value_ptr) {
			auto field_info_iter = info_0.fields_info_map.find(key);
			RTTI_ASSERT_CONTINUE(field_info_iter != info_0.fields_info_map.cend(),
				"Object of type 'Point2D' does not have field with name '", key, "'. "
				"Failed to set value of type '", val.type_name(), "': ", val);

			field_info_iter->second->type_info.setter(obj, val);
		}

#else
		RTTI_OBJECT_SETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	const type_info_getter_t Point2D_getter = [](const void* obj, Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		if (!object_value.is_object()) {
			object_value = Variant::object_t {};
		} else {
			object_value.clear();
		}

		Variant::object_t* object_value_ptr = object_value.get_ptr<Variant::object_t*>();

		for (const ObjectFieldInfo& field_info: info_0.fields_info) {
			Variant field_value;
			field_info.type_info.getter(obj, field_value);

			if (!field_value.is_null())
				object_value_ptr->emplace(field_info.name, std::move(field_value));
		}

#else
		RTTI_OBJECT_GETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	/* x */
	const type_info_setter_t x_setter = [](void* ptr, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		// TODO: rewrite. ptr already must points to field
		auto& ref = static_cast<Point2D*>(ptr)->x;

		RTTI_BUILTIN_TYPE_SETTER(is_floating_point, float, Point2D::x, float,    integer,  unsigned)
		else RTTI_BUILTIN_TYPE_SETTER(is_signed,    float, Point2D::x, integer,  unsigned, float   )
		else RTTI_BUILTIN_TYPE_SETTER(is_unsigned,  float, Point2D::x, unsigned, integer,  float   )
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point2D*>(ptr)->x, value);
#endif
	};

	const type_info_getter_t x_getter = [](const void* ptr, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		value = static_cast<const Point2D*>(ptr)->x;
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point2D*>(ptr)->x, value);
#endif
	};

	auto x_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point2D*)parent_ptr)->x;
	};

	/* y */
	const type_info_setter_t y_setter = [](void* ptr, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		// TODO: rewrite. ptr already must points to field
		auto& ref = static_cast<Point2D*>(ptr)->y;

		RTTI_BUILTIN_TYPE_SETTER(is_floating_point, float, Point2D::y, float,    integer,  unsigned)
		else RTTI_BUILTIN_TYPE_SETTER(is_signed,    float, Point2D::y, integer,  unsigned, float   )
		else RTTI_BUILTIN_TYPE_SETTER(is_unsigned,  float, Point2D::y, unsigned, integer,  float   )
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point2D*>(ptr)->y, value);
#endif
	};

	const type_info_getter_t y_getter = [](const void* ptr, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		value = static_cast<const Point2D*>(ptr)->y;
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point2D*>(ptr)->y, value);
#endif
	};

	auto y_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point2D*)parent_ptr)->y;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Point2D",
			/* setter */ Point2D_setter,
			/* getter */ Point2D_getter
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
			/*
				NOTE(FiTH): Since we are using addresses of elements from std::vector created inside local variable,
					compiler must support NRVO or move constructor for std::vector or program will crash.
			*/
			{ "x", &info.fields_info[0] },
			{ "y", &info.fields_info[1] },
		}
	};

	return info;
}

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point2D>() noexcept
{
	return &info_0;
}


/* * * * * * * * * * * * * * * * * * * * * * *

		Point3D

* * * * * * * * * * * * * * * * * * * * * * * */

namespace
{

static const ObjectTypeInfo info_1 = DB::make_object_type_info<Point3D>();

} /* namespace <anonymous> */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point3D>() noexcept
{
	const type_info_setter_t Point3D_setter = [](void* obj, const Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		const Variant::object_t* object_value_ptr = object_value.get_ptr<const Variant::object_t*>();
		RTTI_ASSERT(object_value_ptr != nullptr, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(
			"Point3D", object_value));

		for (const auto& [key, val]: *object_value_ptr) {
			auto field_info_iter = info_1.fields_info_map.find(key);
			RTTI_ASSERT_CONTINUE(field_info_iter != info_1.fields_info_map.cend(),
				"Object of type 'Point3D' does not have field with name '", key, "'. "
				"Failed to set value of type '", val.type_name(), "': ", val);

			field_info_iter->second->type_info.setter(obj, val);
		}

#else
		RTTI_OBJECT_SETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	const type_info_getter_t Point3D_getter = [](const void* obj, Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		if (!object_value.is_object()) {
			object_value = Variant::object_t {};
		} else {
			object_value.clear();
		}

		Variant::object_t* object_value_ptr = object_value.get_ptr<Variant::object_t*>();

		for (const ObjectFieldInfo& field_info: info_1.fields_info) {
			Variant field_value;
			field_info.type_info.getter(obj, field_value);

			if (!field_value.is_null())
				object_value_ptr->emplace(field_info.name, std::move(field_value));
		}

#else
		RTTI_OBJECT_GETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	/* x */
	const type_info_setter_t x_setter = [](void* ptr, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		// TODO: rewrite. ptr already must points to field
		auto& ref = static_cast<Point3D*>(ptr)->x;

		RTTI_BUILTIN_TYPE_SETTER(is_floating_point, float, Point2D::x, float,    integer,  unsigned)
		else RTTI_BUILTIN_TYPE_SETTER(is_signed,    float, Point2D::x, integer,  unsigned, float   )
		else RTTI_BUILTIN_TYPE_SETTER(is_unsigned,  float, Point2D::x, unsigned, integer,  float   )
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point3D*>(ptr)->x, value);
#endif
	};

	const type_info_getter_t x_getter = [](const void* ptr, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		value = static_cast<const Point3D*>(ptr)->x;
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point3D*>(ptr)->x, value);
#endif
	};

	auto x_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->x;
	};

	/* y */
	const type_info_setter_t y_setter = [](void* ptr, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		// TODO: rewrite. ptr already must points to field
		auto& ref = static_cast<Point3D*>(ptr)->y;

		RTTI_BUILTIN_TYPE_SETTER(is_floating_point, float, Point2D::y, float,    integer,  unsigned)
		else RTTI_BUILTIN_TYPE_SETTER(is_signed,    float, Point2D::y, integer,  unsigned, float   )
		else RTTI_BUILTIN_TYPE_SETTER(is_unsigned,  float, Point2D::y, unsigned, integer,  float   )
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point3D*>(ptr)->y, value);
#endif
	};

	const type_info_getter_t y_getter = [](const void* ptr, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		value = static_cast<const Point3D*>(ptr)->y;
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point3D*>(ptr)->y, value);
#endif
	};

	auto y_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->y;
	};

	/* z */
	const type_info_setter_t z_setter = [](void* ptr, const Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		// TODO: rewrite. ptr already must points to field
		auto& ref = static_cast<Point3D*>(ptr)->z;

		RTTI_BUILTIN_TYPE_SETTER(is_floating_point, float, Point3D::z, float,    integer,  unsigned)
		else RTTI_BUILTIN_TYPE_SETTER(is_signed,    float, Point3D::z, integer,  unsigned, float   )
		else RTTI_BUILTIN_TYPE_SETTER(is_unsigned,  float, Point3D::z, unsigned, integer,  float   )
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point3D*>(ptr)->z, value);
#endif
	};

	const type_info_getter_t z_getter = [](const void* ptr, Variant& value) noexcept
	{
#ifndef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		value = static_cast<const Point3D*>(ptr)->z;
#endif

#ifdef RTTI_DISABLE_BUILTIN_TYPES_DEFAULT_SETTER_GETTER
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point3D*>(ptr)->z, value);
#endif
	};

	auto z_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->z;
	};

	/* point */
	const type_info_setter_t point_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Point3D*>(ptr)->point, value);
	};

	const type_info_getter_t point_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Point3D*>(ptr)->point, value);
	};

	auto point_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Point3D*)parent_ptr)->point;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Point3D",
			/* setter */ Point3D_setter,
			/* getter */ Point3D_getter
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
			/*
				NOTE(FiTH): Since we are using addresses of elements from std::vector created inside local variable,
					compiler must support NRVO or move constructor for std::vector or program will crash.
			*/
			{ "x", &info.fields_info[0] },
			{ "y", &info.fields_info[1] },
			{ "z", &info.fields_info[2] },
			{ "point", &info.fields_info[3] },
		}
	};

	return info;
}

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point3D>() noexcept
{
	return &info_1;
}


/* * * * * * * * * * * * * * * * * * * * * * *

		Rect

* * * * * * * * * * * * * * * * * * * * * * * */

namespace
{

static const ObjectTypeInfo info_2 = DB::make_object_type_info<Rect>();

} /* namespace <anonymous> */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Rect>() noexcept
{
	const type_info_setter_t Rect_setter = [](void* obj, const Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		const Variant::object_t* object_value_ptr = object_value.get_ptr<const Variant::object_t*>();
		RTTI_ASSERT(object_value_ptr != nullptr, RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(
			"Rect", object_value));

		for (const auto& [key, val]: *object_value_ptr) {
			auto field_info_iter = info_2.fields_info_map.find(key);
			RTTI_ASSERT_CONTINUE(field_info_iter != info_2.fields_info_map.cend(),
				"Object of type 'Rect' does not have field with name '", key, "'. "
				"Failed to set value of type '", val.type_name(), "': ", val);

			field_info_iter->second->type_info.setter(obj, val);
		}

#else
		RTTI_OBJECT_SETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	const type_info_getter_t Rect_getter = [](const void* obj, Variant& object_value) noexcept
	{
#ifndef RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER

		if (!object_value.is_object()) {
			object_value = Variant::object_t {};
		} else {
			object_value.clear();
		}

		Variant::object_t* object_value_ptr = object_value.get_ptr<Variant::object_t*>();

		for (const ObjectFieldInfo& field_info: info_2.fields_info) {
			Variant field_value;
			field_info.type_info.getter(obj, field_value);

			if (!field_value.is_null())
				object_value_ptr->emplace(field_info.name, std::move(field_value));
		}

#else
		RTTI_OBJECT_GETTER_IMPL(obj, object_value);
#endif // !RTTI_DISABLE_OBJECT_DEFAULT_SETTER_GETTER
	};

	/* name */
	const type_info_setter_t name_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Rect*>(ptr)->name, value);
	};

	const type_info_getter_t name_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Rect*>(ptr)->name, value);
	};

	auto name_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->name;
	};

	/* top_left */
	const type_info_setter_t top_left_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Rect*>(ptr)->top_left, value);
	};

	const type_info_getter_t top_left_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Rect*>(ptr)->top_left, value);
	};

	auto top_left_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->top_left;
	};

	/* bot_right */
	const type_info_setter_t bot_right_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Rect*>(ptr)->bot_right, value);
	};

	const type_info_getter_t bot_right_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Rect*>(ptr)->bot_right, value);
	};

	auto bot_right_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->bot_right;
	};

	/* arr */
	const type_info_setter_t arr_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_SETTER_IMPL(&static_cast<Rect*>(ptr)->arr, value);
	};

	const type_info_getter_t arr_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_TYPE_GETTER_IMPL(&static_cast<const Rect*>(ptr)->arr, value);
	};

	auto arr_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->arr;
	};

	/* arr2 */
	const type_info_setter_t arr2_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_ARRAY_TYPE_SETTER_IMPL(float, 5,
			static_cast<Rect*>(ptr)->arr2, value);
	};

	const type_info_getter_t arr2_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_ARRAY_TYPE_GETTER_IMPL(float, 5,
			static_cast<const Rect*>(ptr)->arr2, value);
	};

	auto arr2_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->arr2;
	};

	/* str_arr */
	const type_info_setter_t str_arr_setter = [](void* ptr, const Variant& value) noexcept
	{
		RTTI_BUILTIN_ARRAY_TYPE_SETTER_IMPL(char, 16,
			static_cast<Rect*>(ptr)->str_arr, value);
	};

	const type_info_getter_t str_arr_getter = [](const void* ptr, Variant& value) noexcept
	{
		RTTI_BUILTIN_ARRAY_TYPE_GETTER_IMPL(char, 16,
			static_cast<const Rect*>(ptr)->str_arr, value);
	};

	auto str_arr_get_addr = [](void* parent_ptr) noexcept -> void*
	{
		return &((Rect*)parent_ptr)->str_arr;
	};

	ObjectTypeInfo info {
		/* TypeInfo */
		{
			/* name   */ "Rect",
			/* setter */ Rect_setter,
			/* getter */ Rect_getter
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
			/*
				NOTE(FiTH): Since we are using addresses of elements from std::vector created inside local variable,
					compiler must support NRVO or move constructor for std::vector or program will crash.
			*/
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

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Rect>() noexcept
{
	return &info_2;
}

} /* namespace rtti */
