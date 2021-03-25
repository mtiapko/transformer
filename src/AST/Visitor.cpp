#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>

#include "transformer/AST/Visitor.h"

// TODO(FiTH): check 'llvm::StringRef' to 'json' conversion

namespace nlohmann {
	template<>
	struct adl_serializer<llvm::StringRef> {
		static void to_json(json& j, const llvm::StringRef& str) {
			if (str.empty())
				j = nullptr;

			j = std::string_view { str.data(), str.size() };
		}
	};
}

namespace
{

template<size_t N>
consteval auto str_to_lower_case(const char (&src)[N])
{
	constexpr auto is_upper_case = [](char c)
		{ return (c >= 'A' && c <= 'Z'); };

	constexpr auto to_lower_case = [](char c)
		{
			return ((c >= 'A' && c <= 'Z')
				? (c | (1 << 5))
				: c);
		};

	constexpr size_t src_size = N - 1;
	constexpr size_t dst_size = src_size * 2 + 1 /* '\0' */;
	char dst[dst_size] {};

	size_t dst_pos = 0;
	for (size_t i = 0; i < src_size; ++i, ++dst_pos) {
		if (i != 0 && is_upper_case(src[i - 1]) == false && is_upper_case(src[i]))
			dst[dst_pos++] = '_';

		dst[dst_pos] = to_lower_case(src[i]);
	}

	return std::to_array(dst);
}

}

namespace transformer::AST
{

bool Visitor::is_from_main_file(const clang::Decl* decl) const noexcept
{
	const auto& src_mgr = m_context.getSourceManager();
	return (src_mgr.getFileID(decl->getLocation()) == src_mgr.getMainFileID());
}

#define SET_VALUE_OF_PTR(name, decl, value) content[str_to_lower_case(#name).data()] = decl->value()
#define SET_VALUE_OF(decl, value)           SET_VALUE_OF_PTR(value, (&decl), value)
#define SET_VALUE(value)                    SET_VALUE_OF_PTR(value, decl, value)
#define SET_VALUE_WITH_NAME(value, name)    SET_VALUE_OF_PTR(name, decl, value)

void Visitor::gen_type_content(const clang::QualType& type, inja::json& content) const noexcept
{
	SET_VALUE_OF(type, isConstQualified);

	content["type"] = type.getAsString(m_printing_policy);
}

void Visitor::gen_decl_content(const clang::Decl* decl, inja::json& content) const noexcept
{
	SET_VALUE(isTemplateDecl);
	SET_VALUE(isInStdNamespace);
	SET_VALUE(isInAnonymousNamespace);

	auto decl_access = decl->getAccess();
	if (decl_access != clang::AccessSpecifier::AS_none)
		content["access"] = clang::getAccessSpelling(decl_access);

	const auto& src_mgr = m_context.getSourceManager();
	content["filename"] = src_mgr.getFilename(decl->getLocation());
}

/* static */ void Visitor::gen_named_decl_content(const clang::NamedDecl* decl,
	inja::json& content) noexcept
{
	auto& content_name = content["name"];
	const auto* decl_identifier = decl->getIdentifier();
	if (decl_identifier != nullptr)
		content_name = decl_identifier->getName();
	else if (const auto& name = decl->getNameAsString(); name.empty() == false)
		content_name = name;

	auto& content_full_name = content["full_name"];
	if (const auto& full_name = decl->getQualifiedNameAsString(); full_name.empty() == false)
		content_full_name = full_name;
}

/* static */ void Visitor::gen_tag_decl_content(const clang::TagDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(isStruct);
	SET_VALUE(isClass);
	SET_VALUE(isUnion);
	SET_VALUE(isEnum);
}

/* static */ void Visitor::gen_record_decl_content(const clang::RecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(isAnonymousStructOrUnion);
}

void Visitor::gen_enum_decl_content(const clang::EnumDecl* decl,
	inja::json& content) const noexcept
{
	SET_VALUE(isScoped);
	SET_VALUE(isScopedUsingClassTag);
	SET_VALUE(isFixed);

	content["integer_type"] = decl->getIntegerType().getAsString(m_printing_policy);

	auto& enums = content["enumerators"];
	for (const auto& decl_enumerator: decl->enumerators()) {
		auto& e = enums.emplace_back();

		e["name"] = decl_enumerator->getName();

		auto& e_value = e["value"];
		int64_t val = decl_enumerator->getInitVal().getExtValue();
		if (decl_enumerator->getInitVal().isSigned()) e_value = val;
		else e_value = static_cast<uint64_t>(val);
	}
}

void Visitor::gen_func_decl_content(const clang::FunctionDecl* decl,
	inja::json& content) const noexcept
{
	SET_VALUE(isVariadic);
	SET_VALUE(isVirtualAsWritten);
	SET_VALUE(isPure);
	SET_VALUE(isTrivial);
	SET_VALUE(isDefaulted);
	SET_VALUE(isExplicitlyDefaulted);
	SET_VALUE(isUserProvided);
	SET_VALUE(isConstexpr);
	SET_VALUE(isConsteval);
	SET_VALUE(isDeleted);
	SET_VALUE(isDeletedAsWritten);
	SET_VALUE(isInlineSpecified);
	SET_VALUE(isStatic);
	SET_VALUE(isOverloadedOperator);
	SET_VALUE_WITH_NAME(getMinRequiredArguments, MinRequiredArguments);

	// TODO(FiTH):
	//    - check 'noexcept' of this func
	//    - check 'const' of this func

	content["return_type"] = decl->getReturnType().getAsString(m_printing_policy);

	auto& parameters = content["parameters"];
	for (const auto& decl_parameter: decl->parameters()) {
		auto& param = parameters.emplace_back();

		this->gen_type_content(decl_parameter->getType(), param);
		Visitor::gen_named_decl_content(decl_parameter, param);

		const bool hasDefaultArg = decl_parameter->hasDefaultArg();
		param["has_default_arg"] = hasDefaultArg;

		if (hasDefaultArg) {
			const auto& default_arg_range = clang::CharSourceRange::getCharRange(decl_parameter->getDefaultArgRange());

			// TODO(FiTH): fix this, does not work
			param["default_arg"] = clang::Lexer::getSourceText(default_arg_range,
				m_context.getSourceManager(), m_context.getLangOpts());
		}
	}
}

/* static */ void Visitor::gen_class_dflt_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasDefaultConstructor);
	SET_VALUE(hasTrivialDefaultConstructor);
	SET_VALUE(hasNonTrivialDefaultConstructor);
	SET_VALUE(hasUserProvidedDefaultConstructor);
	SET_VALUE(hasConstexprDefaultConstructor);
	SET_VALUE(needsImplicitDefaultConstructor);
	SET_VALUE(defaultedDefaultConstructorIsConstexpr);
}

/* static */ void Visitor::gen_class_copy_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasSimpleCopyConstructor);
	SET_VALUE(hasTrivialCopyConstructor);
	SET_VALUE(hasNonTrivialCopyConstructor);
	SET_VALUE(hasUserDeclaredCopyConstructor);
	SET_VALUE(hasCopyConstructorWithConstParam);
	SET_VALUE(needsImplicitCopyConstructor);
	SET_VALUE(needsOverloadResolutionForCopyConstructor);
	SET_VALUE(implicitCopyConstructorHasConstParam);
	if (decl->needsOverloadResolutionForCopyConstructor() == false)
		SET_VALUE(defaultedCopyConstructorIsDeleted);
}

/* static */ void Visitor::gen_class_move_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasMoveConstructor);
	SET_VALUE(hasSimpleMoveConstructor);
	SET_VALUE(hasTrivialMoveConstructor);
	SET_VALUE(hasNonTrivialMoveConstructor);
	SET_VALUE(hasUserDeclaredMoveConstructor);
	SET_VALUE(needsImplicitMoveConstructor);
	SET_VALUE(needsOverloadResolutionForMoveConstructor);
	if (decl->needsOverloadResolutionForMoveConstructor() == false)
		SET_VALUE(defaultedMoveConstructorIsDeleted);
}

/* static */ void Visitor::gen_class_copy_assign_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasSimpleCopyAssignment);
	SET_VALUE(hasTrivialCopyAssignment);
	SET_VALUE(hasNonTrivialCopyAssignment);
	SET_VALUE(hasCopyAssignmentWithConstParam);
	SET_VALUE(hasUserDeclaredCopyAssignment);
	SET_VALUE(needsImplicitCopyAssignment);
	SET_VALUE(needsOverloadResolutionForCopyAssignment);
	SET_VALUE(implicitCopyAssignmentHasConstParam);
}

/* static */ void Visitor::gen_class_move_assign_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasMoveAssignment);
	SET_VALUE(hasSimpleMoveAssignment);
	SET_VALUE(hasTrivialMoveAssignment);
	SET_VALUE(hasNonTrivialMoveAssignment);
	SET_VALUE(hasUserDeclaredMoveAssignment);
	SET_VALUE(needsImplicitMoveAssignment);
	SET_VALUE(needsOverloadResolutionForMoveAssignment);
}

/* static */ void Visitor::gen_class_destructor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	SET_VALUE(hasSimpleDestructor);
	SET_VALUE(hasIrrelevantDestructor);
	SET_VALUE(hasTrivialDestructor);
	SET_VALUE(hasNonTrivialDestructor);
	SET_VALUE(hasUserDeclaredDestructor);
	SET_VALUE(hasConstexprDestructor);
	SET_VALUE(needsImplicitDestructor);
	SET_VALUE(needsOverloadResolutionForDestructor);
	if (decl->needsOverloadResolutionForDestructor() == false)
		SET_VALUE(defaultedDestructorIsDeleted);
}

void Visitor::gen_class_base_full_content(const clang::CXXBaseSpecifier& base,
	inja::json& content) const noexcept
{
	SET_VALUE_OF(base, isVirtual);
	SET_VALUE_OF(base, getInheritConstructors);

	content["name"] = base.getType().getAsString(m_printing_policy);
	content["access_specifier"] = clang::getAccessSpelling(base.getAccessSpecifier());
}

void Visitor::gen_class_field_full_content(const clang::FieldDecl* decl,
	inja::json& content) const noexcept
{
	Visitor::gen_named_decl_content(decl, content);
	this->gen_type_content(decl->getType(), content);
}

void Visitor::gen_class_method_full_content(const clang::CXXMethodDecl* decl,
	inja::json& content) const noexcept
{
	this->gen_decl_content(decl, content);
	Visitor::gen_named_decl_content(decl, content);
	this->gen_func_decl_content(decl, content);
}

void Visitor::gen_cxx_record_decl_content(const clang::CXXRecordDecl* decl,
	inja::json& content) const noexcept
{
	SET_VALUE(isEmpty);
	SET_VALUE(isAggregate);
	SET_VALUE(isStandardLayout);
	SET_VALUE(isTriviallyCopyable);
	SET_VALUE(isPOD);
	SET_VALUE(isTrivial);
	SET_VALUE(isPolymorphic);
	SET_VALUE(isAbstract);
	SET_VALUE(isLiteral);

	content["is_local_class"] = (decl->isLocalClass() != nullptr);

	SET_VALUE(hasUserDeclaredConstructor);
	SET_VALUE(hasConstexprNonCopyMoveConstructor);
	SET_VALUE(hasMutableFields);
	SET_VALUE(hasVariantMembers);
	SET_VALUE(allowConstDefaultInit);

	Visitor::gen_class_dflt_ctor_content  (decl, content["default_ctor"]);
	Visitor::gen_class_copy_ctor_content  (decl, content["copy_ctor"   ]);
	Visitor::gen_class_move_ctor_content  (decl, content["move_ctor"   ]);
	Visitor::gen_class_copy_assign_content(decl, content["copy_assign" ]);
	Visitor::gen_class_move_assign_content(decl, content["move_assign" ]);
	Visitor::gen_class_destructor_content (decl, content["destructor"  ]);

	auto& bases_content = content["base_classes"];
	for (const auto& base: decl->bases())
		this->gen_class_base_full_content(base, bases_content.emplace_back());

	auto& conv_funcs_content = content["conversion_functions"];
	for (const auto& conv_func: decl->getVisibleConversionFunctions())
		Visitor::gen_named_decl_content(conv_func, conv_funcs_content.emplace_back());

	auto& fields_content = content["fields"];
	for (const auto& field: decl->fields())
		this->gen_class_field_full_content(field, fields_content.emplace_back());

	auto& methods_content                    = content["methods"];
	auto& constructors_content               = content["constructors"];
	auto& overloaded_operators_equal_content = content["overloaded_operators_equal"];

	for (const auto& method: decl->methods()) {
		inja::json* content_ptr = (llvm::isa<clang::CXXConstructorDecl>(method)
			? &constructors_content
			: method->getOverloadedOperator() == clang::OverloadedOperatorKind::OO_Equal
				? &overloaded_operators_equal_content
				: &methods_content
		);

		this->gen_class_method_full_content(method, content_ptr->emplace_back());
	}
}

Visitor::Visitor(const clang::ASTContext& context, inja::json& tmpl_content) noexcept
	: m_context(context)
	, m_printing_policy(context.getPrintingPolicy())

	, m_tmpl_content(tmpl_content             )
	, m_tmpl_classes(tmpl_content["classes"  ])
	, m_tmpl_enums  (tmpl_content["enums"    ])
	, m_tmpl_funcs  (tmpl_content["functions"])
{}

bool Visitor::VisitCXXRecordDecl(const clang::CXXRecordDecl* decl) const noexcept
{
	if (this->is_from_main_file(decl) == false)
		return true;

	// NOTE(FiTH): decl->isAnonymousStructOrUnion() does not work for some reason
	if (decl->isCompleteDefinition() && decl->getIdentifier() != nullptr) {
		auto& content = m_tmpl_classes.emplace_back();

		this->gen_decl_content(decl, content);
		Visitor::gen_named_decl_content(decl, content);
		Visitor::gen_tag_decl_content(decl, content);
		Visitor::gen_record_decl_content(decl, content);

		this->gen_cxx_record_decl_content(decl, content);
	}

	return true;
}

bool Visitor::VisitEnumDecl(const clang::EnumDecl* decl) const noexcept
{
	if (this->is_from_main_file(decl) == false)
		return true;

	if (decl->isCompleteDefinition()) {
		auto& content = m_tmpl_enums.emplace_back();

		this->gen_decl_content(decl, content);
		Visitor::gen_named_decl_content(decl, content);
		Visitor::gen_tag_decl_content(decl, content);

		this->gen_enum_decl_content(decl, content);
	}

	return true;
}

bool Visitor::VisitFunctionDecl(const clang::FunctionDecl* decl) const noexcept
{
	if (this->is_from_main_file(decl) == false)
		return true;

	// visit only non-member functions
	if (decl->getAccess() == clang::AccessSpecifier::AS_none) {
		auto& content = m_tmpl_funcs.emplace_back();

		this->gen_decl_content(decl, content);
		Visitor::gen_named_decl_content(decl, content);

		this->gen_func_decl_content(decl, content);
	}

	return true;
}

#undef SET_VALUE_WITH_NAME
#undef SET_VALUE
#undef SET_VALUE_OF
#undef SET_VALUE_OF_PTR

}
