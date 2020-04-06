#include "rtti/Variant.h"
#include "rtti/Object.h"

namespace rtti
{

Variant Object::object_get_value() const noexcept
{
	Variant value;
	this->object_get_value(value);

	return value;
}

}
