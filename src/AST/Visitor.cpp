#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>

#include "transformer/AST/Visitor.h"

// TODO(FiTH): check 'clang::StringRef' to 'json' conversion

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

#define CHECK_FLAG_OF_PTR(decl, flag) content[str_to_lower_case(#flag).data()] = decl->flag()
#define CHECK_FLAG_OF(decl, flag)     CHECK_FLAG_OF_PTR((&decl), flag)
#define CHECK_FLAG(flag)              CHECK_FLAG_OF_PTR(decl, flag)


void Visitor::gen_type_content(const clang::QualType& type, inja::json& content) const noexcept
{
	content["type"] = type.getAsString(m_printing_policy);

	CHECK_FLAG_OF(type, isConstQualified);
}

/* static */ void Visitor::gen_decl_content(const clang::Decl* decl, inja::json& content) noexcept
{
	CHECK_FLAG(isTemplateDecl);
	CHECK_FLAG(isInStdNamespace);
	CHECK_FLAG(isInAnonymousNamespace);

	auto decl_access = decl->getAccess();
	if (decl_access != clang::AccessSpecifier::AS_none)
		content["access"] = clang::getAccessSpelling(decl_access);
}

/* static */ void Visitor::gen_named_decl_content(const clang::NamedDecl* decl,
	inja::json& content) noexcept
{
	auto& content_name = content["name"];
	const auto* decl_identifier = decl->getIdentifier();
	if (decl_identifier != nullptr) content_name = decl_identifier->getName();
	else content_name = decl->getNameAsString();

	content["full_name"] = decl->getQualifiedNameAsString();
}

/* static */ void Visitor::gen_tag_decl_content(const clang::TagDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(isStruct);
	CHECK_FLAG(isClass);
	CHECK_FLAG(isUnion);
	CHECK_FLAG(isEnum);
}

/* static */ void Visitor::gen_record_decl_content(const clang::RecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(isAnonymousStructOrUnion);
}

void Visitor::gen_enum_decl_content(const clang::EnumDecl* decl,
	inja::json& content) const noexcept
{
	CHECK_FLAG(isScoped);
	CHECK_FLAG(isScopedUsingClassTag);
	CHECK_FLAG(isFixed);

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
	CHECK_FLAG(isVariadic);
	CHECK_FLAG(isVirtualAsWritten);
	CHECK_FLAG(isPure);
	CHECK_FLAG(isTrivial);
	CHECK_FLAG(isDefaulted);
	CHECK_FLAG(isExplicitlyDefaulted);
	CHECK_FLAG(isUserProvided);
	CHECK_FLAG(isConstexpr);
	CHECK_FLAG(isConsteval);
	CHECK_FLAG(isDeleted);
	CHECK_FLAG(isDeletedAsWritten);
	CHECK_FLAG(isInlineSpecified);
	CHECK_FLAG(isStatic);
	CHECK_FLAG(isOverloadedOperator);

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
			const auto& text = clang::Lexer::getSourceText(default_arg_range,
				m_context.getSourceManager(), m_context.getLangOpts());

			// TODO(FiTH): fix this, does not work
			param["default_arg"] = std::string_view { text.data(), text.size() };
		}
	}
}

/* static */ void Visitor::gen_class_dflt_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasDefaultConstructor);
	CHECK_FLAG(hasTrivialDefaultConstructor);
	CHECK_FLAG(hasNonTrivialDefaultConstructor);
	CHECK_FLAG(hasUserProvidedDefaultConstructor);
	CHECK_FLAG(hasConstexprDefaultConstructor);
	CHECK_FLAG(needsImplicitDefaultConstructor);
	CHECK_FLAG(defaultedDefaultConstructorIsConstexpr);
}

/* static */ void Visitor::gen_class_copy_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasSimpleCopyConstructor);
	CHECK_FLAG(hasTrivialCopyConstructor);
	CHECK_FLAG(hasNonTrivialCopyConstructor);
	CHECK_FLAG(hasUserDeclaredCopyConstructor);
	CHECK_FLAG(hasCopyConstructorWithConstParam);
	CHECK_FLAG(needsImplicitCopyConstructor);
	CHECK_FLAG(needsOverloadResolutionForCopyConstructor);
	CHECK_FLAG(implicitCopyConstructorHasConstParam);
	if (decl->needsOverloadResolutionForCopyConstructor() == false)
		CHECK_FLAG(defaultedCopyConstructorIsDeleted);
}

/* static */ void Visitor::gen_class_move_ctor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasMoveConstructor);
	CHECK_FLAG(hasSimpleMoveConstructor);
	CHECK_FLAG(hasTrivialMoveConstructor);
	CHECK_FLAG(hasNonTrivialMoveConstructor);
	CHECK_FLAG(hasUserDeclaredMoveConstructor);
	CHECK_FLAG(needsImplicitMoveConstructor);
	CHECK_FLAG(needsOverloadResolutionForMoveConstructor);
	if (decl->needsOverloadResolutionForMoveConstructor() == false)
		CHECK_FLAG(defaultedMoveConstructorIsDeleted);
}

/* static */ void Visitor::gen_class_copy_assign_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasSimpleCopyAssignment);
	CHECK_FLAG(hasTrivialCopyAssignment);
	CHECK_FLAG(hasNonTrivialCopyAssignment);
	CHECK_FLAG(hasCopyAssignmentWithConstParam);
	CHECK_FLAG(hasUserDeclaredCopyAssignment);
	CHECK_FLAG(needsImplicitCopyAssignment);
	CHECK_FLAG(needsOverloadResolutionForCopyAssignment);
	CHECK_FLAG(implicitCopyAssignmentHasConstParam);
}

/* static */ void Visitor::gen_class_move_assign_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasMoveAssignment);
	CHECK_FLAG(hasSimpleMoveAssignment);
	CHECK_FLAG(hasTrivialMoveAssignment);
	CHECK_FLAG(hasNonTrivialMoveAssignment);
	CHECK_FLAG(hasUserDeclaredMoveAssignment);
	CHECK_FLAG(needsImplicitMoveAssignment);
	CHECK_FLAG(needsOverloadResolutionForMoveAssignment);
}

/* static */ void Visitor::gen_class_destructor_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
{
	CHECK_FLAG(hasSimpleDestructor);
	CHECK_FLAG(hasIrrelevantDestructor);
	CHECK_FLAG(hasTrivialDestructor);
	CHECK_FLAG(hasNonTrivialDestructor);
	CHECK_FLAG(hasUserDeclaredDestructor);
	CHECK_FLAG(hasConstexprDestructor);
	CHECK_FLAG(needsImplicitDestructor);
	CHECK_FLAG(needsOverloadResolutionForDestructor);
	if (decl->needsOverloadResolutionForDestructor() == false)
		CHECK_FLAG(defaultedDestructorIsDeleted);
}

void Visitor::gen_class_base_full_content(const clang::CXXBaseSpecifier& base,
	inja::json& content) const noexcept
{
	CHECK_FLAG_OF(base, isVirtual);
	CHECK_FLAG_OF(base, getInheritConstructors);

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
	Visitor::gen_decl_content(decl, content);
	Visitor::gen_named_decl_content(decl, content);
	this->gen_func_decl_content(decl, content);
}

void Visitor::gen_cxx_record_decl_content(const clang::CXXRecordDecl* decl,
	inja::json& content) const noexcept
{
	CHECK_FLAG(isEmpty);
	CHECK_FLAG(isAggregate);
	CHECK_FLAG(isStandardLayout);
	CHECK_FLAG(isTriviallyCopyable);
	CHECK_FLAG(isPOD);
	CHECK_FLAG(isTrivial);
	CHECK_FLAG(isPolymorphic);
	CHECK_FLAG(isAbstract);
	CHECK_FLAG(isLiteral);

	content["is_local_class"] = (decl->isLocalClass() != nullptr);

	CHECK_FLAG(hasUserDeclaredConstructor);
	CHECK_FLAG(hasConstexprNonCopyMoveConstructor);
	CHECK_FLAG(hasMutableFields);
	CHECK_FLAG(hasVariantMembers);
	CHECK_FLAG(allowConstDefaultInit);

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

	auto& methods_content = content["methods"];
	for (const auto& method: decl->methods()) {
		if (method->isThisDeclarationADefinition() && method->isImplicit() == false)
			this->gen_class_method_full_content(method, methods_content.emplace_back());
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

	if (decl->isCompleteDefinition()) {
		auto& content = m_tmpl_classes.emplace_back();

		Visitor::gen_decl_content(decl, content);
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

		Visitor::gen_decl_content(decl, content);
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

		Visitor::gen_decl_content(decl, content);
		Visitor::gen_named_decl_content(decl, content);

		this->gen_func_decl_content(decl, content);
	}

	return true;
}

#undef CHECK_FLAG
#undef CHECK_FLAG_OF
#undef CHECK_FLAG_OF_PTR

}
