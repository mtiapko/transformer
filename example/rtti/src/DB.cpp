#include "../../../libs/json/include/nlohmann/json.hpp" // TODO: replace by ... ?

#include "rtti/DB.h"

namespace rtti
{

/* static */ void DB::object_set_value_default(void* obj, const Variant& value, const ObjectTypeInfo* info) noexcept
{
	if (value.type() != Variant::value_t::object) {
		// TODO: log
		std::cerr << "Error! Trying to set non-object (" << value.type_name() << ") value for '" << info->name << "'\n";
		return;
	}

	const Variant::object_t* map = value.get_ptr<const Variant::object_t*>();
	for (const auto& [key, val]: *map) {
		auto field_iter = info->fields_info_map.find(key);
		if (field_iter == info->fields_info_map.cend()) {
			// TODO: log
			std::cerr << "Error! Object of type '" << info->name
				<< "' does not have any field named '" << key << "'\n";
			continue;
		}

		field_iter->second->setter(obj, val);
	}
}

// TODO: add more. maybe add macro? and make another file? disable by marco in Config.h?
template<> /* static */ void DB::object_set_value<std::string>(void* obj, const Variant& value) noexcept
{
	auto& str = *(std::string*)obj;

	auto val = value.get_ptr<const Variant::string_t*>();
	if (val == nullptr) {
		// TODO: log
		std::cerr << "Error! Set value type mismatch: 'std::string' and '" << value.type_name() << "'\n";
		return;
	}

	str = *val;
}

}
