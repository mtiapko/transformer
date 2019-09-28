# Transformer - C/C++ code parser and generator
Tool for parsing C/C++ code and generating output using a given `Mustache` template. This might be useful for creating reflection/RTTI frameworks for C/C++ (or you can just wait until 2023 or 2026 year and use standard implementation... ~~[or use Java](https://www.youtube.com/watch?v=umDr0mPuyQc "Java")~~)

Currently supported features:
- struct/class fields parsing
- enum parsing
- generation output using [Mustache](https://mustache.github.io) template

# Usage example
Let's imagine that we have some files:

#### [example/source.hpp](example/source.hpp)
```c++
#include <vector>

namespace math::geometry
{

struct Point2D
{
	float x;
	float y;
};

struct Point3D : public Point2D
{
	float z;
};

enum class Direction : long
{
	UP = 0,
	DOWN,

	LEFT = -10,
	RIGHT
};

class Shape
{
private:
	std::vector<Point3D> m_points;
};

}
```

#### [example/template.mustache](example/template.mustache)
```mustache
We have in file {{file_path}} {{classes_count}} classes:
{{#classes}}
	{{^class_base_classes_is_empty}}
	{{class_name}} has {{class_base_classes_count}} base class:
	{{/class_base_classes_is_empty}}
	{{#class_base_classes}}
		- {{base_class_name}} from file {{base_class_file_path}}
	{{/class_base_classes}}
	{{#class_base_classes_is_empty}}
	{{class_name}} does not have base classes :(
	{{/class_base_classes_is_empty}}

	{{^class_fields_is_empty}}
	{{class_name}} has {{class_fields_count}} fields:
	{{/class_fields_is_empty}}
	{{#class_fields}}
		- {{field_type}} -> {{field_name}} (full name is {{field_full_name}})
	{{/class_fields}}

	{{#classes_is_last_elem}}
	THE END!
	{{/classes_is_last_elem}}
{{/classes}}

{{#enums}}
Enum {{enum_name}} has type {{enum_integer_type}}:
{{#enum_consts}}
	- {{const_full_name}} has value {{const_value}}
{{/enum_consts}}
{{/enums}}
```

After execution of command `transformer example/template.mustache example/source.hpp` the output will be:
```text
We have in file example/source.hpp 3 classes:
	Point2D does not have base classes :(

	Point2D has 2 fields:
		- float -> x (full name is math::geometry::Point2D::x)
		- float -> y (full name is math::geometry::Point2D::y)

	Point3D has 1 base class:
		- Point2D from file example/source.hpp

	Point3D has 3 fields:
		- float -> x (full name is math::geometry::Point2D::x)
		- float -> y (full name is math::geometry::Point2D::y)
		- float -> z (full name is math::geometry::Point3D::z)

	Shape does not have base classes :(

	Shape has 1 fields:
		- std::vector<Point3D> -> m_points (full name is math::geometry::Shape::m_points)

	THE END!

Enum Direction has type long:
	- math::geometry::Direction::UP has value 0
	- math::geometry::Direction::DOWN has value 1
	- math::geometry::Direction::LEFT has value -10
	- math::geometry::Direction::RIGHT has value -9
```

# Requirements
- [GCC](https://gcc.gnu.org/) with C++17 support
- [LibClang](https://clang.llvm.org/docs/Tooling.html) for parsing C/C++ core
- [Boost](https://www.boost.org/doc/libs/1_71_0/doc/html/variant.html) 1.54+ for Boost.Variant
- [CMake](https://cmake.org/) 3.0+ for building

# Third-party libs
- [mstch](https://github.com/no1msd/mstch) for parsing and rendering `Mustache` templates

# Installation
This is a piece of [cake](https://www.youtube.com/watch?v=dQw4w9WgXcQ ":cake:"):
```
git clone --recursive https://github.com/mtiapko/transformer
make
```

# Troubleshooting
> ### [ clang ] fatal error: 'stddef.h' file not found

This error occurs when `clang` looks for headers in `gcc` directories. You can create symlink, but `transformer` support arguments for compiler, so you can set system header directory path:
```
transformer <template-file-path> <source-file-path> --compiler-args -cxx-isystem /usr/lib/clang/<clang-version>/include
```