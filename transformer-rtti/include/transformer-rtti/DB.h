#ifndef __TRANSFORMER_RTTI_DB_H__
#define __TRANSFORMER_RTTI_DB_H__

#include "transformer-rtti/type_traits/RemoveAllCV.h"
#include "transformer-rtti/stdlib/Vector.h"
#include "transformer-rtti/dbg/Assert.h"
#include "transformer-rtti/NamedTypeInfo.h" // TODO(FiTH): include only TypeInfo.h?

namespace transformer::rtti
{

class DB
{
// TODO(FiTH): make this public?
private:
	static auto& get_types() noexcept
	{
		// TODO(FiTH): register nullptr type with id == 0? (see QualifiedType, default value of 'id')
		static Vector<const TypeInfo*> types { nullptr };
		return types;
	}

private: /* internal */
	template<typename T>
	static TypeID get_unqualified_type_id() noexcept
	{
		// NOTE(FiTH): 'type_id' == index in 'types' array
		static const TypeID type_id = (
			DB::get_types().emplace_back(nullptr), // TODO(FiTH): emplace ptr to default (UNKNOWN)
			DB::get_types().size() - 1             //             type? remove all null checks?
		);

		return type_id;
	}

public: /* get type id */
	template<typename T>
	static TypeID get_type_id() noexcept
	{
		// static_assert(std::is_reference_v<T> == false); // TODO(FiTH): handle this

		return DB::get_unqualified_type_id<type_traits::remove_all_cv_t<std::remove_reference_t<T>>>();
	}

	// TODO(FiTH): see P1985 "Universal Template Parameters"
#define RETURN_TYPE_ID static const TypeID type_id = ( \
		DB::get_types().emplace_back(nullptr), \
		DB::get_types().size() - 1 \
	); \
	\
	return type_id

	template<template<class...> class T>    static TypeID get_type_id() noexcept { RETURN_TYPE_ID; }
	template<template<class, auto> class T> static TypeID get_type_id() noexcept { RETURN_TYPE_ID; }
	template<template<auto> class T>        static TypeID get_type_id() noexcept { RETURN_TYPE_ID; }

#undef RETURN_TYPE_ID

public: /* API */
	static TypeID set_type_info(const TypeInfo* type_info, TypeID type_id) noexcept
	{
		// TODO(FiTH): remove?
		if (const auto* type_info = DB::get_types()[type_id]; type_info != nullptr) {
			std::clog << "[WARNING] " << __PRETTY_FUNCTION__ << " - Type with ID already reported (#" << type_id << ")! Name: "
			<< ((type_info->kind >= TypeKind::BEG_NAMES_TYPES_VALUE && type_info->kind <= TypeKind::END_NAMED_TYPES_VALUE)
				? static_cast<const NamedTypeInfo*>(type_info)->name
				: "(unnamed)") // TODO(FiTH): use QualifiedType::evaluate_name()
			<< "\n";
		}

		// RTTI_ASSERT(DB::get_types()[type_id] == nullptr); // TODO(FiTH)
		DB::get_types()[type_id] = type_info;

		return type_id;
	}

	static const TypeInfo* get_type_info(TypeID type_id) noexcept
	{
		// TODO(FiTH)
		RTTI_ASSERT(type_id < DB::get_types().size());

		return DB::get_types()[type_id];
	}

public: /* templated versions of API (not for all cases) */
	template<typename T>
	static TypeID set_type_info(const TypeInfo* type_info) noexcept
	{
		const TypeID type_id = DB::get_type_id<T>();

		// TODO(FiTH): remove?
		if (DB::get_types()[type_id] != nullptr)
			std::clog << "[WARNING] " << __PRETTY_FUNCTION__ << " - Type with this ID already reported!\n";

		// RTTI_ASSERT(DB::get_types()[type_id] == nullptr); // TODO(FiTH)
		DB::get_types()[type_id] = type_info;

		return type_id;
	}

	template<template<auto> typename T>
	static TypeID set_type_info(const TypeInfo* type_info) noexcept
	{
		const TypeID type_id = DB::get_type_id<T>();

		// TODO(FiTH): remove?
		if (DB::get_types()[type_id] != nullptr)
			std::clog << "[WARNING] " << __PRETTY_FUNCTION__ << " - Type with this ID already reported!\n";

		// RTTI_ASSERT(DB::get_types()[type_id] == nullptr); // TODO(FiTH)
		DB::get_types()[type_id] = type_info;

		return type_id;
	}

	template<typename T>
	static const TypeInfo* get_type_info() noexcept
	{
		return DB::get_type_info(DB::get_type_id<T>());
	}
};

}

#endif // !__TRANSFORMER_RTTI_DB_H__
