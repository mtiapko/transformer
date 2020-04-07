#include "rtti/Variant.h"
#include "rtti/DB.h"

template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Point2D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Point3D>() noexcept;
template<> /* static */ const rtti::ObjectTypeInfo* rtti::DB::get_object_type_info<struct Rect>() noexcept;

#include "rect.h" // TODO: add file name var

namespace rtti
{

/* info */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point2D>() noexcept
{
	ObjectTypeInfo info {
	/* TypeInfo */
	{
		"Point2D",
		(type_info_setter_t)
			(void (*)(Point2D*, const Variant&) noexcept)
				&DB::object_set_value<Point2D>, // TODO: remove <>?
		(type_info_getter_t)
			(void (*)(const Point2D*, Variant&) noexcept)
				&DB::object_get_value<Point2D>,
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"float", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
				auto this_ptr = (Point2D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) { // TODO: handle null
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_unsigned_v<float>) {
					if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_signed_v<float>) {
					if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else {
					extern void not_existing_func();
					not_existing_func();

					std::cerr << "Error! Unexpected type 'float' of 'Point2D::x'\n";
				}
#else

// TODO: code duplication
					DB::object_set_value /* <float> */ (&((Point2D*)inst)->x, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
					value = ((const Point2D*)obj)->x;
#else

					DB::object_get_value(&((const Point2D*)obj)->x, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
			},
			/* ObjectFieldInfo */
			/* name */ "x",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point2D*)this_ptr)->x;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"float", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
				auto this_ptr = (Point2D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) { // TODO: handle null
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_unsigned_v<float>) {
					if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_signed_v<float>) {
					if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else {
					extern void not_existing_func();
					not_existing_func();

					std::cerr << "Error! Unexpected type 'float' of 'Point2D::y'\n";
				}
#else

// TODO: code duplication
					DB::object_set_value /* <float> */ (&((Point2D*)inst)->y, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
					value = ((const Point2D*)obj)->y;
#else

					DB::object_get_value(&((const Point2D*)obj)->y, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
			},
			/* ObjectFieldInfo */
			/* name */ "y",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point2D*)this_ptr)->y;
			},
			&DB::get_object_type_info<float>
		},
	},
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

/* DB::get_object_type_info() */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point2D>() noexcept
{
	return &info_0;
}

} /* namespace rtti */

namespace rtti
{

/* info */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Point3D>() noexcept
{
	ObjectTypeInfo info {
	/* TypeInfo */
	{
		"Point3D",
		(type_info_setter_t)
			(void (*)(Point3D*, const Variant&) noexcept)
				&DB::object_set_value<Point3D>, // TODO: remove <>?
		(type_info_getter_t)
			(void (*)(const Point3D*, Variant&) noexcept)
				&DB::object_get_value<Point3D>,
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"float", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) { // TODO: handle null
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_unsigned_v<float>) {
					if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_signed_v<float>) {
					if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::x type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::x setted to " << *v << '\n';
						this_ptr->x = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else {
					extern void not_existing_func();
					not_existing_func();

					std::cerr << "Error! Unexpected type 'float' of 'Point2D::x'\n";
				}
#else

// TODO: code duplication
					DB::object_set_value /* <float> */ (&((Point3D*)inst)->x, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
					value = ((const Point3D*)obj)->x;
#else

					DB::object_get_value(&((const Point3D*)obj)->x, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
			},
			/* ObjectFieldInfo */
			/* name */ "x",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->x;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"float", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) { // TODO: handle null
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_unsigned_v<float>) {
					if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_signed_v<float>) {
					if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point2D::y type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point2D::y setted to " << *v << '\n';
						this_ptr->y = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else {
					extern void not_existing_func();
					not_existing_func();

					std::cerr << "Error! Unexpected type 'float' of 'Point2D::y'\n";
				}
#else

// TODO: code duplication
					DB::object_set_value /* <float> */ (&((Point3D*)inst)->y, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
					value = ((const Point3D*)obj)->y;
#else

					DB::object_get_value(&((const Point3D*)obj)->y, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
			},
			/* ObjectFieldInfo */
			/* name */ "y",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->y;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"float", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) { // TODO: handle null
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_unsigned_v<float>) {
					if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else if constexpr (std::is_signed_v<float>) {
					if (value.type() == Variant::value_t::number_integer) {
						const auto v = value.get_ptr<const Variant::number_integer_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Integer>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_unsigned) {
						const auto v = value.get_ptr<const Variant::number_unsigned_t*>();
						if (v == nullptr) {
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Unsigned>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else if (value.type() == Variant::value_t::number_float) {
						const auto v = value.get_ptr<const Variant::number_float_t*>();
						if (v == nullptr) {
							// TODO: rewrite error msg
							std::cerr << "Error! Point3D::z type mismatch: 'float' and '" << value.type_name() << "'\n";
							return;
						}

						std::clog << "Success! <Float>: Point3D::z setted to " << *v << '\n';
						this_ptr->z = static_cast<float>(*v);
					} else {
						std::cerr << "Error! Unexpected value type '" << value.type_name() << "': " << value << '\n';
					}
				} else {
					extern void not_existing_func();
					not_existing_func();

					std::cerr << "Error! Unexpected type 'float' of 'Point3D::z'\n";
				}
#else

// TODO: code duplication
					DB::object_set_value /* <float> */ (&((Point3D*)inst)->z, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

#ifndef RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE
					value = ((const Point3D*)obj)->z;
#else

					DB::object_get_value(&((const Point3D*)obj)->z, value);

#endif // !RTTI_DISABLE_BUILTIN_TYPES_SET_AND_GET_VALUE

				},
			},
			/* ObjectFieldInfo */
			/* name */ "z",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->z;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"Point2D", /* type: Point2D */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value /* <Point2D> */ (&((Point3D*)inst)->point, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value(&((const Point3D*)obj)->point, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "point",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->point;
			},
			&DB::get_object_type_info<Point2D>
		},
	},
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

/* DB::get_object_type_info() */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Point3D>() noexcept
{
	return &info_1;
}

} /* namespace rtti */

namespace rtti
{

/* info */

template<>
/* static */ ObjectTypeInfo DB::make_object_type_info<Rect>() noexcept
{
	ObjectTypeInfo info {
	/* TypeInfo */
	{
		"Rect",
		(type_info_setter_t)
			(void (*)(Rect*, const Variant&) noexcept)
				&DB::object_set_value<Rect>, // TODO: remove <>?
		(type_info_getter_t)
			(void (*)(const Rect*, Variant&) noexcept)
				&DB::object_get_value<Rect>,
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >", /* type: std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value /* <std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >> */ (&((Rect*)inst)->name, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value(&((const Rect*)obj)->name, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "name",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->name;
			},
			&DB::get_object_type_info<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >>
		},
		{
			/* TypeInfo */
			{
				"Point3D", /* type: Point3D */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value /* <Point3D> */ (&((Rect*)inst)->top_left, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value(&((const Rect*)obj)->top_left, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "top_left",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->top_left;
			},
			&DB::get_object_type_info<Point3D>
		},
		{
			/* TypeInfo */
			{
				"Point3D", /* type: Point3D */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value /* <Point3D> */ (&((Rect*)inst)->bot_right, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value(&((const Rect*)obj)->bot_right, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "bot_right",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->bot_right;
			},
			&DB::get_object_type_info<Point3D>
		},
		{
			/* TypeInfo */
			{
				"std::vector<float, std::allocator<float> >", /* type: std::vector<float, std::allocator<float> > */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value /* <std::vector<float, std::allocator<float> >> */ (&((Rect*)inst)->arr, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value(&((const Rect*)obj)->arr, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "arr",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->arr;
			},
			&DB::get_object_type_info<std::vector<float, std::allocator<float> >>
		},
		{
			/* TypeInfo */
			{
				"float [5]", /* type: float [5] */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value<float, 5>(((Rect*)inst)->arr2, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value<float, 5>(((const Rect*)obj)->arr2, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "arr2",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->arr2;
			},
			&DB::get_object_type_info<float [5]>
		},
		{
			/* TypeInfo */
			{
				"char [16]", /* type: char [16] */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{

					DB::object_set_value<char, 16>(((Rect*)inst)->str_arr, value);

				},
				/* getter */ [](const void* obj, Variant& value) noexcept
				{

					DB::object_get_value<char, 16>(((const Rect*)obj)->str_arr, value);

				},
			},
			/* ObjectFieldInfo */
			/* name */ "str_arr",
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->str_arr;
			},
			&DB::get_object_type_info<char [16]>
		},
	},
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

/* DB::get_object_type_info() */

template<>
/* static */ const ObjectTypeInfo* DB::get_object_type_info<Rect>() noexcept
{
	return &info_2;
}

} /* namespace rtti */

