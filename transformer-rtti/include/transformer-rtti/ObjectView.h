#ifndef __TRANSFORMER_RTTI_OBJECT_VIEW_H__
#define __TRANSFORMER_RTTI_OBJECT_VIEW_H__

#include "transformer-rtti/type_traits/CopyConst.h"
#include "transformer-rtti/RecordTypeInfo.h"

namespace transformer::rtti
{

// TODO(FiTH): also add 'ConstObjectView' (or 'ConstView') and 'MutableObjectView'?

struct ObjectView
{
public: /* data */
	QualifiedType type = {}; // TODO(FiTH): RTTI_ASSERT(type.rank == 0) in constructor?
	void*         self = nullptr; // TODO(FiTH): or use byte_t and remove all static_cast<char*>()?

public: /* ctor & dtor */
	constexpr ObjectView() noexcept = default;

	ObjectView(const QualifiedType& obj_type, void* obj_self) noexcept
		: type(obj_type)
		, self(obj_self)
	{}

	ObjectView(const QualifiedType& obj_type, const void* obj_self) noexcept
		: ObjectView(obj_type, const_cast<void*>(obj_self))
	{}

public: /* internal API */
	template<typename SelfT>
	static ObjectView find_field(const QualifiedType& self_type, SelfT* self_ptr, StringView name) noexcept
	{
		// TODO(FiTH): log error? do this check only in DEBUG mode?
		if (self_ptr == nullptr)
			return {}; // TODO(FiTH): add constant 'Null Object'?

		// TODO(FiTH): RecordTemplateSpecializationTypeInfo must be derived from RecordTypeInfo
		static_assert(std::is_base_of_v<RecordTypeInfo, RecordTemplateSpecializationTypeInfo>);

		const auto* self_info = DB::get_type_info(self_type.id);
		RTTI_ASSERT(
			self_info != nullptr &&
			(
				self_info->kind == TypeKind::RECORD_TYPE ||
				self_info->kind == TypeKind::RECORD_TEMPLATE_SPECIALIZATION_TYPE
			)
		); // TODO(FiTH)

		const auto* record_info = static_cast<const RecordTypeInfo*>(self_info);
		const auto field_iter = record_info->fields_map.find(name);
		if (field_iter == record_info->fields_map.cend())
			return {}; // TODO(FiTH): log error?

		const auto* field_info = field_iter->second;

		if constexpr (std::is_const_v<SelfT>) {
			return {
				QualifiedType::add_local_const(field_info->qual_type),
				reinterpret_cast<const char*>(self_ptr) + field_info->offset // TODO(FiTH): use byte_t
			};
		} else {
			return {
				field_info->qual_type,
				reinterpret_cast<char*>(self_ptr) + field_info->offset
			};
		}
	}

	// TODO(FiTH): or use 'const QualifiedType& self_type, SelfT* self_ptr'?
	template<typename Ret, typename... Args>
	static Ret call_method(const ObjectView& obj, StringView name, Args&&... args) noexcept
	{
		const auto* self_info = DB::get_type_info(obj.type.id);
		RTTI_ASSERT(
			self_info != nullptr &&
			(
				self_info->kind == TypeKind::RECORD_TYPE ||
				self_info->kind == TypeKind::RECORD_TEMPLATE_SPECIALIZATION_TYPE
			)
		); // TODO(FiTH)

		const auto* record_info = static_cast<const RecordTypeInfo*>(self_info);
		const auto method_iter = record_info->methods_map.find(name);
		if (method_iter == record_info->methods_map.cend()) {
			RTTI_LOG_ERR("Type '", record_info->name, "' does not have method '", name, '\'');

			// TODO(FiTH): ?
			if constexpr (std::is_same_v<Ret, void>) return; else return {};
		}

		const auto* method_info = method_iter->second;
		if (obj.type.is_local_const() && method_info->is_const == false) {
			RTTI_LOG_ERR("Failed to call non-const method '", record_info->name, "::", name, "' of const object");
			if constexpr (std::is_same_v<Ret, void>) return; else return {};
		}

		if (method_info->params.size() != sizeof...(args)) {
			RTTI_LOG_ERR("Method '", record_info->name, "::", name, "' expected ",
				method_info->params.size(), " arguments, but has ", sizeof...(args));
			if constexpr (std::is_same_v<Ret, void>) return; else return {};
		}

		const void* method_args[] = {
			[]<typename T>(T&& arg) noexcept {
				if constexpr (std::is_pointer_v<T> || std::is_array_v<T>)
					return arg;
				else
					return &arg;
			}(std::forward<Args>(args))...
		};

		if constexpr (std::is_same_v<Ret, void>) {
			method_info->ptr(obj.self, nullptr, method_args);
		} else {
			alignas(Ret) char ret[sizeof(Ret)];
			method_info->ptr(obj.self, ret, method_args);

			return *reinterpret_cast<Ret*>(ret);
		}
	}

public: /* API */
	ObjectView find_field(StringView name) const noexcept
	{
		if (type.is_local_const())
			return ObjectView::find_field(type, static_cast<const void*>(self), name);

		return ObjectView::find_field(type, self, name);
	}

	template<typename Ret = void, typename... Args>
	Ret call_method(StringView name, Args&&... args) const noexcept
	{
		return ObjectView::call_method<Ret>(*this, name, std::forward<Args>(args)...);
	}

	template<typename T>
	T* as_ptr() const noexcept
	{
		// TODO(FiTH): log error? do this check only in DEBUG mode?
		if (self == nullptr)
			return nullptr; // TODO(FiTH): add constant 'Null Object'?

		// TODO(FiTH): do all these checks only in DEBUG mode?
		const auto& qual_type = QualifiedType::get_qualified_type<T>();
		if (qual_type.id != type.id) {
			RTTI_LOG_ERR("Type mismatch: '", type.evaluate_name(),
				"' and '", qual_type.evaluate_name(), '\''); // TODO(FiTH)

			// TODO(FiTH): try to convert?
			return nullptr;
		}

		const auto src_cv = type.quals.cv_qualifiers;
		const auto dst_cv = qual_type.quals.cv_qualifiers;
		if ((src_cv & dst_cv) != src_cv) {
			RTTI_LOG_ERR("Source type '", type.evaluate_name(),
				"' is more qualified than destination '", qual_type.evaluate_name(), '\''); // TODO(FiTH)
			return nullptr;
		}

		// TODO(FiTH): re-check this logic
		const auto inv_src_cv = ~src_cv;
		if ((((dst_cv & inv_src_cv) >> 2) & inv_src_cv) != 0) {
			RTTI_LOG_ERR("Invalid conversion from '", type.evaluate_name(),
				"' to '", qual_type.evaluate_name(), '\''); // TODO(FiTH)
			return nullptr;
		}

		// TODO(FiTH): remove ref
		return static_cast<T*>(self);
	}

	template<typename T>
	T& as() const noexcept
	{
		return *this->as_ptr<T>();
	}

	template<typename T>
	T* cast_to() const noexcept
	{
		if (self == nullptr)
			return nullptr;

		const auto& dst_type = QualifiedType::get_qualified_type<T>();
		RTTI_ASSERT(
			dst_type.quals.is_lvalue_ref == false &&
			dst_type.quals.qualifiers_rank == 0
		); // TODO(FiTH)

		if (type.quals.cv_qualifiers > dst_type.quals.cv_qualifiers) {
			RTTI_LOG_ERR("Cannot cast from '", type.evaluate_name(),
				"' to less qualified type '", dst_type.evaluate_name(), '\'');
			return nullptr;
		}

		if (type.id == dst_type.id)
			return reinterpret_cast<T*>(self);

		const auto* src_info = DB::get_type_info(type.id);
		if (src_info == nullptr) {
			RTTI_LOG_ERR("No info for type #", type.id);
			return nullptr;
		}

		if (
			src_info->kind != TypeKind::RECORD_TYPE &&
			src_info->kind != TypeKind::RECORD_TEMPLATE_SPECIALIZATION_TYPE
		) {
			RTTI_LOG_ERR("Type '", type.evaluate_name(), "' is not a record");
			return nullptr;
		}

		const auto* src_record_info = static_cast<const RecordTypeInfo*>(src_info);
		const auto base_iter = src_record_info->bases_map.find(dst_type.id);
		if (base_iter == src_record_info->bases_map.cend()) {
			RTTI_LOG_ERR("Type '", type.evaluate_name(),
				"' does not derived from '", dst_type.evaluate_name(), '\'');
			return nullptr;
		}

		const auto* base = base_iter->second;
		return reinterpret_cast<T*>(
			reinterpret_cast<char*>(self) + base->offset
		);
	}

public: /* operators */
	ObjectView operator[](StringView name) const noexcept
	{
		return this->find_field(name);
	}
};

template<typename T>
ObjectView to_object_view(T* t) noexcept
{
	return { QualifiedType::get_qualified_type<T>(), t };
}

template<typename T>
ObjectView to_object_view(T& t) noexcept
{
	return rtti::to_object_view(&t);
}

template<typename T>
T* cast_to(const ObjectView& obj) noexcept
{
	return obj.cast_to<T>();
}

}

#endif // !__TRANSFORMER_RTTI_OBJECT_VIEW_H__
