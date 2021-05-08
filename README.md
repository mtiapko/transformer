# Transformer - C/C++ code parser and generator
Tool for parsing C/C++ code and generating output using a given **Jinja**-like template. This might be useful for creating reflection/RTTI frameworks for C/C++ (or you can just wait until 2023 or 2026 year and use standard implementation... ~~[or use Java](https://www.youtube.com/watch?v=umDr0mPuyQc "Java")~~)

Currently supported features:
- struct/class fields parsing
- struct/class methods (including constructors) parsing
- enum parsing
- generate output using **[Jinja](https://palletsprojects.com/p/jinja/)**-like template
- [C++ custom attributes](https://en.cppreference.com/w/cpp/language/attributes) support

# WARNING! WIP!
After switching from `libclang` to `libtooling`, some of the information in this `README` file is **OUTDATED**. At the moment `transformer` is a fully working tool, but some command line interfaces have changed a little, and also names of some template variables have changed. I will try to update this manual in the near future.

# Usage example
Let's imagine that we have some files:

#### [example/simple/source.hpp](example/simple/source.hpp)
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

#### [example/simple/template.jinja](example/simple/template.jinja)
```jinja
We have in file {{ file_path }} {{ length(classes) }} classes:
{% for c in classes %}
	{% if length(c.base_classes) != 0 %}
	{{ c.name }} has {{ length(c.base_classes) }} base class:
	{% endif %}
	{% for b in c.base_classes %}
		- {{ b.name }} from file {{ b.file_path }}
	{% endfor %}
	{% if length(c.base_classes) == 0 %}
	{{ c.name }} does not have base classes :(
	{% endif %}

	{% if length(c.fields) != 0 %}
	{{ c.name }} has {{ length(c.fields) }} fields:
	{% endif %}
	{% for f in c.fields %}
		- {{ f.type }} -> {{ f.name }} (full name is {{ f.full_name }})
	{% endfor %}

	{% if loop.is_last %}
	THE END!
	{% endif %}
{% endfor %}

{% for e in enums %}
Enum {{ e.name }} has type {{ e.integer_type }}:
	{% for c in e.consts %}
	- {{ c.full_name }} has value {{ c.value }}
	{% endfor %}
{% endfor %}
```

After execution of command `transformer example/simple/template.jinja example/simple/source.hpp` the output will be:
```text
We have in file example/simple/source.hpp 3 classes:
	Point2D does not have base classes :(

	Point2D has 2 fields:
		- float -> x (full name is math::geometry::Point2D::x)
		- float -> y (full name is math::geometry::Point2D::y)

	Point3D has 1 base class:
		- Point2D from file example/simple/source.hpp

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

For more information about tool usage execute command:
```bash
transformer --help
```

# Beta! RTTI framework for C++
You can also find a more advanced example of creating a RTTI framework for C++ in the directory [`example/rtti/`](example/rtti/). Some examples with it:
```c++
#include "rtti/ObjectType.h"

struct Point2D
{
	float x;
	float y;
};

struct Point3D : Point2D
{
	float z;
};

// all we need is to inherit from rtti::ObjectType class and use RTTI_OBJECT_TYPE_BODY macro
class Rect : public rtti::ObjectType<Rect>
{
private: // <- we even have access to private fields
	RTTI_OBJECT_TYPE_BODY;

	std::string name;
	Point3D top_left;
	Point3D bot_right;
};

int main()
{
	Rect r {};

	r.object_field("name") = "Hello, Rect!";
	r.object_field("top_left") = 123; // <- wrong type
	r.object_field("bot_right") = { { "x", 12345.f } };
	r.object_field("bot_right").subfield("x") = -1.5f;

	r.object_set_value(
		{
			{ "top_left", {
				{ "x", 123u },
				{ "z", 3.0  },
				{ "y", 5.1f },
			} },
			{ "name", "New name from " __FILE__ },
			{ "some name", 123 - 456 }
		}
	);
}
```

# Requirements
- [GCC](https://gcc.gnu.org/) with C++17 support
- [GNU Make](https://www.gnu.org/software/make/) for building
- [LibClang](https://clang.llvm.org/docs/Tooling.html) for parsing C/C++ code

# Third-party libs
- [inja](https://github.com/pantor/inja) for parsing and rendering **Jinja**-like templates
- [json](https://github.com/nlohmann/json) required by `inja` for data input and handling

# Downloading and compilation
This is a piece of [cake](https://www.youtube.com/watch?v=dQw4w9WgXcQ "lie"):
```
git clone --recursive https://github.com/mtiapko/transformer
make
```

Compiled tool binary will be in `bin/` directory.

### But!
[Be careful](https://www.youtube.com/watch?v=mFElmSV87pg), `nlohmann/json` is very heavy repo (240MB+). But `inja` requires only source files (only include directory). So you can clone this repo without `--recursive` if `nlohmann/json` already exists in your filesystem or manually download only source files and put them in `libs/json/include/nlohmann/<header-files>`.

# Troubleshooting
> ### [ clang ] fatal error: 'stddef.h' file not found

This error occurs when `clang` looks for headers in `gcc` directories. You can create symlink, but `transformer` support arguments for compiler, so you can set system header directory path:
```
transformer <template-file-path> <source-file-path> --compiler-args -cxx-isystem /usr/lib/clang/<clang-version>/include
```
