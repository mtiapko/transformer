#include "rtti/DB.h"
#include "../../../libs/json/include/nlohmann/json.hpp"

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
		&DB::object_set_value<Point2D>,
		{},
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"x", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
				auto this_ptr = (Point2D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) {
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
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point2D*)this_ptr)->x;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"y", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
				auto this_ptr = (Point2D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) {
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
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
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
		&DB::object_set_value<Point3D>,
		{},
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"x", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) {
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
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->x;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"y", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) {
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
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->y;
			},
			&DB::get_object_type_info<float>
		},
		{
			/* TypeInfo */
			{
				"z", /* type: float */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
				auto this_ptr = (Point3D*)inst;
				if constexpr (std::is_floating_point_v<float>) {
					if (value.type() == Variant::value_t::number_float) {
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
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Point3D*)this_ptr)->z;
			},
			&DB::get_object_type_info<float>
		},
	},
	{
		{ "x", &info.fields_info[0] },
		{ "y", &info.fields_info[1] },
		{ "z", &info.fields_info[2] },
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
		&DB::object_set_value<Rect>,
		{},
	},
	/* ObjectTypeInfo */
	{
		{
			/* TypeInfo */
			{
				"name", /* type: std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
					DB::object_set_value<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >>(&((Rect*)inst)->name, value);
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->name;
			},
			&DB::get_object_type_info<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >>
		},
		{
			/* TypeInfo */
			{
				"top_left", /* type: Point3D */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
					DB::object_set_value<Point3D>(&((Rect*)inst)->top_left, value);
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->top_left;
			},
			&DB::get_object_type_info<Point3D>
		},
		{
			/* TypeInfo */
			{
				"bot_right", /* type: Point3D */
				/* setter */ [](void* inst, const Variant& value) noexcept
				{
					DB::object_set_value<Point3D>(&((Rect*)inst)->bot_right, value);
				},
				/* getter */ {},
			},
			/* ObjectFieldTypeInfo */
			/* get addr */ [](void* this_ptr) noexcept -> void*
			{
				return &((Rect*)this_ptr)->bot_right;
			},
			&DB::get_object_type_info<Point3D>
		},
	},
	{
		{ "name", &info.fields_info[0] },
		{ "top_left", &info.fields_info[1] },
		{ "bot_right", &info.fields_info[2] },
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

