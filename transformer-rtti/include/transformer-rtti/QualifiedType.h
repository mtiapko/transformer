#ifndef __TRANSFORMER_RTTI_QUALIFIED_TYPE_H__
#define __TRANSFORMER_RTTI_QUALIFIED_TYPE_H__

#include "transformer-rtti/stdlib/String.h"
#include "transformer-rtti/dbg/Assert.h"
#include "transformer-rtti/DB.h"

namespace transformer::rtti
{

struct Qualifiers
{
	// TODO(FiTH): add 'is_rvalue_ref'?
	// TODO(FiTH): remove prefix and suffix 'qualifiers'?

	uint16_t is_lvalue_ref  : 1;
	uint16_t is_rvalue_ref  : 1;
	uint16_t qualifiers_rank: 3;
	uint16_t cv_qualifiers;

	template<typename T>
	static consteval Qualifiers get_qualifiers(uint16_t level = 0) noexcept
	{
		if constexpr (std::is_lvalue_reference_v<T>)
		{
			// no +1 for level!
			auto qualifiers = Qualifiers::get_qualifiers<std::remove_reference_t<T>>(level);
			return {
				.is_lvalue_ref   = true,
				.is_rvalue_ref   = false,
				.qualifiers_rank = qualifiers.qualifiers_rank,
				.cv_qualifiers   = qualifiers.cv_qualifiers
			};
		} else if constexpr (std::is_rvalue_reference_v<T>) {
			// no +1 for level!
			auto qualifiers = Qualifiers::get_qualifiers<std::remove_reference_t<T>>(level);
			return {
				.is_lvalue_ref   = false,
				.is_rvalue_ref   = true,
				.qualifiers_rank = qualifiers.qualifiers_rank,
				.cv_qualifiers   = qualifiers.cv_qualifiers
			};
		}

		// TODO(FiTH): add check to prevent overflow!
		const uint16_t local_qualifiers
			= (std::is_volatile_v<T> << (level * 2 + 1))  // TODO(FiTH): magic numbers!
			| (std::is_const_v<T>    << (level * 2 + 0));

		if constexpr (std::is_pointer_v<T>) {
			auto other_qualifiers = Qualifiers::get_qualifiers<std::remove_pointer_t<T>>(level + 1);
			return {
				.is_lvalue_ref   = false,
				.is_rvalue_ref   = false,
				.qualifiers_rank = other_qualifiers.qualifiers_rank,
				.cv_qualifiers   = (uint16_t)(other_qualifiers.cv_qualifiers | local_qualifiers)
			};
		} else if constexpr (std::is_array_v<T>) {
			auto other_qualifiers = Qualifiers::get_qualifiers<std::remove_extent_t<T>>(level + 1);
			return {
				.is_lvalue_ref   = false,
				.is_rvalue_ref   = false,
				.qualifiers_rank = other_qualifiers.qualifiers_rank,
				.cv_qualifiers   = (uint16_t)(other_qualifiers.cv_qualifiers | local_qualifiers)
			};
		} else {
			// NOTE(FiTH): implicit +1 for 'level', see description of Qualifiers struct
			return {
				.is_lvalue_ref   = false,
				.is_rvalue_ref   = false,
				.qualifiers_rank = level,
				.cv_qualifiers   = local_qualifiers
			};
		}
	}

	// TODO(FiTH)
	//
	// bool is_compatible_with(const Qualifiers& other) const noexcept
	// {
	// 	return true;
	// }
};

// TODO(FiTH): rewrite all methods
struct QualifiedType
{
	TypeID     id    = 0;
	Qualifiers quals = {};

	// TODO(FiTH): add comment?
	static_assert(sizeof(id) == sizeof(quals));

	// TODO(FiTH): must be part of Qualifiers class?
	static QualifiedType add_local_const(const QualifiedType& qual_type) noexcept
	{
		auto local_const_qual_type = qual_type;
		local_const_qual_type.quals.cv_qualifiers |= 1; // TODO(FiTH): magic number!

		return local_const_qual_type;
	}

	bool is_local_const() const noexcept
	{
		return (quals.cv_qualifiers & 1); // TODO(FiTH): magic number!
	}

	// TODO(FiTH): move this to the DB class?
	template<typename T>
	static QualifiedType get_qualified_type() noexcept
	{
		static constexpr const auto quals = Qualifiers::get_qualifiers<T>();

		// TODO(FiTH): make it all static?
		return {
			.id    = DB::get_type_id<T>(),
			.quals = quals
		};
	}

	// TODO(FiTH): rewrite! (rename to 'get_name')
	String evaluate_name() const noexcept;

	// TODO(FiTH): write template version of 'evaluate_name', to handle all combinations of ptrs
	template<typename T>
	static String evaluate_name() noexcept
	{
		return QualifiedType::get_qualified_type<T>().evaluate_name();
	}
};

}

#endif // !__TRANSFORMER_RTTI_QUALIFIED_TYPE_H__
