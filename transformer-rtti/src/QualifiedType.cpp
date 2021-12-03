#include "transformer-rtti/ArrayTypeInfo.h"
#include "transformer-rtti/NamedTypeInfo.h"
#include "transformer-rtti/QualifiedType.h"

namespace transformer::rtti
{

// TODO(FiTH): rewrite this as soon as possible!
String QualifiedType::evaluate_name() const noexcept
{
	String name {};
	if (quals.is_lvalue_ref)
		name = '&';

	const auto* type_info = DB::get_type_info(id);

	const auto insert_local_cv = [&](uint32_t i) noexcept
	{
		// TODO(FiTH): magic numbers! same as in the TypeQualifiersInfo.h!
		const uint32_t local_qualifiers  = ((quals.cv_qualifiers >> (i * 2)) & 0b11);
		const uint32_t is_local_const    = (local_qualifiers & 0b01);
		const uint32_t is_local_volatile = (local_qualifiers & 0b10);

		if (is_local_const)    name.insert(0, "const ");
		if (is_local_volatile) name.insert(0, "volatile ");
	};

	// TODO(FiTH): wrong format: 'int* (*)[3]' (ptr to array of ptrs) will be 'int*[3]*'
	// cv with (level == quals.qualifiers_rank) will be inserted later in this func
	for (uint32_t i = 0; i < quals.qualifiers_rank; ++i) {
		// TODO(FiTH): log warning?
		if (type_info == nullptr)
			return "(no info)";

		insert_local_cv(i);

		switch (type_info->kind) {
			case TypeKind::POINTER_TYPE:
				name.insert(0, 1, '*');
				break;
			case TypeKind::ARRAY_TYPE:
				name.insert(0, 1, ']');
				name.insert(0, std::to_string(static_cast<const ArrayTypeInfo*>(type_info)->extent));
				name.insert(0, 1, '[');
				break;
			default:
				RTTI_ASSERT(i == quals.qualifiers_rank, ""); // TODO(FiTH)
				continue;
		};

		const auto* pointer_type_info = static_cast<const PointerTypeInfo*>(type_info);
		type_info = DB::get_type_info(pointer_type_info->pointee_type.id);
	}

	// TODO(FiTH): these types do not have 'name' field!
	RTTI_ASSERT(type_info == nullptr || (
			type_info->kind != TypeKind::POINTER_TYPE &&
			type_info->kind != TypeKind::ARRAY_TYPE
		),
		"" // TODO(FiTH)
	);

	if (type_info == nullptr)
		name.insert(0, "(no info)");
	else if (type_info->kind >= TypeKind::BEG_NAMES_TYPES_VALUE
			&& type_info->kind <= TypeKind::END_NAMED_TYPES_VALUE)
		name.insert(0, static_cast<const NamedTypeInfo*>(type_info)->name);
	else
		name.insert(0, "(unnamed type)");

	insert_local_cv(quals.qualifiers_rank);
	return name;
}

}
