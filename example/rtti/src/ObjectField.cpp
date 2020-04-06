#include "rtti/ObjectField.h"
#include "rtti/Variant.h"

namespace rtti
{

Variant ObjectField::get_value() const noexcept
{
	Variant value {};
	this->get_value(value);

	return value;
}

ObjectField::operator Variant() const noexcept
{
	return this->get_value();
}

}
