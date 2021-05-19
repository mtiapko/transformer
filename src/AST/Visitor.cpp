#include <charconv>

#include <clang/Basic/SourceManager.h>
#include <clang/AST/RecordLayout.h>
#include <clang/Lex/Lexer.h>

#include "transformer/AST/Visitor.h"
#include "transformer/Config.h"

// TODO(FiTH): check 'llvm::StringRef' to 'json' conversion

namespace nlohmann {
	template<>
	struct adl_serializer<llvm::StringRef>
	{
		static void to_json(json& j, const llvm::StringRef& str)
		{
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

bool Visitor::does_decl_require_content_gen(const clang::Decl* decl) const noexcept
{
	// TODO(FiTH): check this? - decl->isDefinedOutsideFunctionOrMethod()

	return (Config::gen_content_for_includes_opt || this->is_from_main_file(decl));
}

std::vector<std::string> Visitor::split_annotate_attributes(const inja::json& annotate_attr,
	inja::json& content) const noexcept
{
	const auto find_char = [](const std::string& str, size_t beg_index, const char c) noexcept
	{
		union
		{
			struct
			{
				uint32_t l_bracket_count: 8; // [
				uint32_t l_brace_count  : 8; // {
				uint32_t l_paren_count  : 8; // (
				uint32_t double_quote   : 1; // "
				uint32_t single_quote   : 1; // '
			} data;

			uint32_t total_count = 0;
		} chars_state {};

		static_assert(sizeof(chars_state) == sizeof(chars_state.total_count));

		// TODO(FiTH): add assert if curr == '\0' but total_count != 0?
		// TODO(FiTH): add assert if count < 0?
		// TODO(FiTH): handle \" \' cases?

		// NOTE(FiTH): std::string is always null-terminated (str[str.size()] == CharT())
		for (size_t i = beg_index; i <= str.size(); ++i) {
			const char curr = str[i];

			if ((curr == c && chars_state.total_count == 0) || (curr == '\0'))
				return i;

			if      (chars_state.data.single_quote == 0) { if (curr == '"' ) chars_state.data.double_quote ^= 1; } // invert
			else if (chars_state.data.double_quote == 0) { if (curr == '\'') chars_state.data.single_quote ^= 1; } // invert
			else if (curr == '[') ++chars_state.data.l_bracket_count;
			else if (curr == ']') --chars_state.data.l_bracket_count;
			else if (curr == '{') ++chars_state.data.l_brace_count;
			else if (curr == '}') --chars_state.data.l_brace_count;
			else if (curr == '(') ++chars_state.data.l_paren_count;
			else if (curr == ')') --chars_state.data.l_paren_count;
		}

		return str.size();
	};

	const auto skip_whitespaces = [](const std::string& str, size_t beg_index, size_t end_index) noexcept
	{
		const auto is_whitespace = [](const char c) noexcept
		{ return (c == ' ' || c == '\t' || c == '\n'); };

		while (is_whitespace(str[beg_index])) // '\0' is not a whitespace
			++beg_index;

		while (beg_index > end_index && is_whitespace(str[end_index]))
			--end_index;

		return std::pair { beg_index, end_index };
	};

	std::vector<std::string> attr_list {};
	for (const auto& attr: annotate_attr) {
		const auto& attr_str = attr.get_ref<const std::string&>();

		for (size_t beg = 0, end = 0; beg < attr_str.size(); beg = end + 1 /* skip ',' */) {
			end = find_char(attr_str, beg, ',');

			const auto [attr_beg, attr_end] = skip_whitespaces(attr_str, beg, end - 1 /* skip ',' or '\0' */);
			if (attr_str[attr_beg] == ',') // case with ", ," or even ",,"
				continue;

			// TODO(FiTH): add assert attr_beg <= attr_end
			assert(attr_beg <= attr_end);

			// attr_beg can be equal to attr_end! using +1 for size
			const auto parsed_attr = std::string_view { attr_str.data() + attr_beg, attr_end - attr_beg + 1 };
			if (parsed_attr.starts_with(m_rtti_attributes_namespace)) {
				// TODO(FiTH): add assert that 'parsed_attr' does not contain '=', '(' etc.
				content[parsed_attr.data() + m_rtti_attributes_namespace.size()] = true;
				continue;
			}

			attr_list.emplace_back(parsed_attr);
		}
	}

	return attr_list;
}

#define SET_VALUE_OF_PTR(name, decl, value) content[str_to_lower_case(#name).data()] = decl->value()
#define SET_VALUE_OF(decl, value)           SET_VALUE_OF_PTR(value, (&decl), value)
#define SET_VALUE(value)                    SET_VALUE_OF_PTR(value, decl, value)
#define SET_VALUE_WITH_NAME(value, name)    SET_VALUE_OF_PTR(name, decl, value)

// NOTE(FiTH): 'is_type_used' is true only for types of fields (required only for serialization?)
void Visitor::gen_type_content(const clang::QualType& type, inja::json& content, bool is_type_used /* = false */) noexcept
{
	SET_VALUE_OF(type, isConstQualified);
	SET_VALUE_OF(type, isVolatileQualified);

	content["is_builtin"         ] = type->isBuiltinType();
	content["is_record"          ] = type->isRecordType();
	content["is_enumeral"        ] = type->isEnumeralType();
	content["is_signed_integer"  ] = type->isSignedIntegerType();
	content["is_unsigned_integer"] = type->isUnsignedIntegerType();

	// TODO(FiTH): check if this is too slow

	static constexpr const char* pointee_content_name = "pointee_type";

	// ref info
	{
		const auto* ref_type = type->getAs<clang::ReferenceType>();
		const bool is_const_reference = (ref_type != nullptr && ref_type->getPointeeType().isConstQualified());

		content["is_reference"          ] = (ref_type != nullptr);
		content["is_const_reference"    ] = is_const_reference;
		content["is_non_const_reference"] = (ref_type != nullptr && !is_const_reference);
		if (ref_type != nullptr)
			this->gen_type_content(ref_type->getPointeeType(), content[pointee_content_name], is_type_used);
	}

	// ptr info
	{
		const auto* ptr_type = type->getAs<clang::PointerType>();
		const bool is_const_pointer = (ptr_type != nullptr && ptr_type->getPointeeType().isConstQualified());

		content["is_pointer"          ] = (ptr_type != nullptr);
		content["is_const_pointer"    ] = is_const_pointer;
		content["is_non_const_pointer"] = (ptr_type != nullptr && !is_const_pointer);
		if (ptr_type != nullptr) // TODO(FiTH): assert(ref_type == nullptr)
			this->gen_type_content(ptr_type->getPointeeType(), content[pointee_content_name], is_type_used);
	}

	// arr info
	{
		const auto* arr_type = m_context.getAsConstantArrayType(type);
		content["is_array"] = (arr_type != nullptr);
		if (arr_type != nullptr) { // TODO(FiTH): assert(ref_type == nullptr && ptr_type == nullptr)
			content["array_size"] = arr_type->getSize().getZExtValue();
			this->gen_type_content(arr_type->getElementType(), content[pointee_content_name], is_type_used);
		}
	}

	content["name"] = type.getAsString(m_printing_policy);

	if (is_type_used && (type->isBuiltinType() == false || Config::report_used_builtin_types_opt)) {
		// TODO(FiTH): add this as a new field to content?
		auto canonical_name = type.getCanonicalType().withoutLocalFastQualifiers().getAsString(m_printing_policy);
		m_used_types.try_emplace(canonical_name, content);
	}
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

	auto& annotate_attributes_content = content["annotate_attributes"];
	for (const auto& attr: decl->attrs()) {
		if (attr->getKind() == clang::attr::Kind::Annotate) {
			const auto& annotate = static_cast<clang::AnnotateAttr*>(attr)->getAnnotation();
			annotate_attributes_content += annotate;
		}
	}

	content["annotate_attributes_list"]
		= Visitor::split_annotate_attributes(annotate_attributes_content, content["rtti_attributes"]);
}

void Visitor::gen_named_decl_content(const clang::NamedDecl* decl,
	inja::json& content, bool is_type_decl /* = false */) noexcept
{
	auto& name_content = content["name"];
	const auto* decl_identifier = decl->getIdentifier();
	if (decl_identifier != nullptr)
		name_content = decl_identifier->getName();
	else if (const auto& name = decl->getNameAsString(); name.empty() == false)
		name_content = name;

	auto& full_name_content = content["full_name"];
	if (const auto& full_name = decl->getQualifiedNameAsString(); full_name.empty() == false) {
		full_name_content = full_name;

		if (is_type_decl)
			m_defined_types.emplace_back(full_name);
	}
}

// TODO(FiTH): not call for enums?
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
	inja::json& content) noexcept
{
	SET_VALUE(isScoped);
	SET_VALUE(isScopedUsingClassTag);
	SET_VALUE(isFixed);

	this->gen_type_content(decl->getIntegerType(), content["integer_type"]);

	auto& enumerators_content                  = content["enumerators"];
	auto& enumerators_grouped_by_value_content = content["enumerators_grouped_by_value"];
	for (const auto& decl_enumerator: decl->enumerators()) {
		auto& e = enumerators_content.emplace_back();

		const auto& enumerator_name = decl_enumerator->getName();
		e["name"] = enumerator_name;

		auto& e_value_content = e["value"];

		// NOTE(FiTH): 'end() - 1' - leave place for '\0'
		std::array<char, 32> enum_init_val_str {};

		const auto& enum_init_val = decl_enumerator->getInitVal();
		assert(enum_init_val.isSigned() == decl->getIntegerType()->isSignedIntegerType()); // TODO(FiTH)

		if (enum_init_val.isSigned()) {
			int64_t signed_value = enum_init_val.getExtValue();

			auto [ptr, ec] = std::to_chars(enum_init_val_str.begin(), enum_init_val_str.end() - 1, signed_value);
			*ptr = '\0';
		} else {
			uint64_t unsigned_value = enum_init_val.getZExtValue();

			auto [ptr, ec] = std::to_chars(enum_init_val_str.begin(), enum_init_val_str.end() - 1, unsigned_value);
			*ptr = '\0';
		}

		// TODO(FiTH): ohhh... inja does not distinguish between signed and unsigned integers
		e_value_content = enum_init_val_str.data();

		// key: enumerators value -> data: array of enumerators (all have the same value)
		enumerators_grouped_by_value_content[enum_init_val_str.data()] += e;
	}
}

void Visitor::gen_func_decl_content(const clang::FunctionDecl* decl,
	inja::json& content) noexcept
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
	//    - check 'noexcept' of this func (decl->getExceptionSpecType() & clang::ExceptionSpecificationType::EST_BasicNoexcept)
	//    - check 'const' of this func

	this->gen_type_content(decl->getReturnType(), content["return_type"]);

	auto& parameters = content["parameters"];
	for (const auto& decl_parameter: decl->parameters()) {
		auto& param = parameters.emplace_back();

		this->gen_type_content(decl_parameter->getType(), param["type"]);
		this->gen_named_decl_content(decl_parameter, param);

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

void Visitor::gen_class_all_bases_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
	clang::CharUnits::QuantityType offset_in_chars, inja::json& bases_content, inja::json& fields_content) noexcept
{
	for (const auto& base: decl->bases()) {
		const auto* base_decl   = base.getType()->getAsCXXRecordDecl();
		const auto& base_layout = m_context.getASTRecordLayout(base_decl);

		// TODO(FiTH): getVBaseClassOffset?
		auto base_offset_in_chars = layout.getBaseClassOffset(base_decl).getQuantity() + offset_in_chars;

		this->gen_class_all_bases_full_content(base_decl, base_layout, offset_in_chars, bases_content, fields_content);

		auto& content = bases_content.emplace_back();
		content["offset_in_chars"       ] = base_offset_in_chars;
		content["vb_ptr_offset_in_chars"] = layout.getVBPtrOffset().getQuantity();
		content["has_own_vf_ptr"        ] = layout.hasOwnVFPtr();
		content["has_extendable_vf_ptr" ] = layout.hasExtendableVFPtr();
		content["has_own_vb_ptr"        ] = layout.hasOwnVBPtr();
		content["has_vb_ptr"            ] = layout.hasVBPtr();

		SET_VALUE_OF(base, isVirtual);
		SET_VALUE_OF(base, getInheritConstructors);

		content["name"] = base.getType().getAsString(m_printing_policy);
		content["access_specifier"] = clang::getAccessSpelling(base.getAccessSpecifier());

		this->gen_class_all_fields_full_content(base_decl, base_layout, base_offset_in_chars, fields_content);
		// TODO(FiTH): also gen content for all inherited methods?
	}
}

void Visitor::gen_class_all_fields_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
	clang::CharUnits::QuantityType base_offset_in_chars, inja::json& fields_content) noexcept
{
	const auto base_offset_in_bits = base_offset_in_chars * CHAR_BIT;

	for (const auto& field: decl->fields()) {
		auto& content = fields_content.emplace_back();
		content["offset_in_bits"] = layout.getFieldOffset(field->getFieldIndex()) + base_offset_in_bits;

		// TODO(FiTH): add check and set to MAX if this is bit field
		content["offset_in_chars"] = layout.getFieldOffset(field->getFieldIndex()) / CHAR_BIT + base_offset_in_chars;

		this->gen_named_decl_content(field, content);
		this->gen_type_content(field->getType(), content["type"], /* is_type_used */ true);
	}
}

void Visitor::gen_class_method_full_content(const clang::CXXMethodDecl* decl,
	inja::json& content) noexcept
{
	this->gen_decl_content(decl, content);
	this->gen_named_decl_content(decl, content);
	this->gen_func_decl_content(decl, content);

	SET_VALUE(isStatic);
	SET_VALUE(isConst);
	SET_VALUE(isVirtual);
}

void Visitor::gen_cxx_record_decl_content(const clang::CXXRecordDecl* decl,
	inja::json& content) noexcept
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
	// TODO(FiTH): SET_VALUE(hasConstFields)? gen content from clang::RecordDecl?

	Visitor::gen_class_dflt_ctor_content  (decl, content["default_ctor"]);
	Visitor::gen_class_copy_ctor_content  (decl, content["copy_ctor"   ]);
	Visitor::gen_class_move_ctor_content  (decl, content["move_ctor"   ]);
	Visitor::gen_class_copy_assign_content(decl, content["copy_assign" ]);
	Visitor::gen_class_move_assign_content(decl, content["move_assign" ]);
	Visitor::gen_class_destructor_content (decl, content["destructor"  ]);

	const auto& record_layout = m_context.getASTRecordLayout(decl);
	content["alignment_in_chars"] = record_layout.getAlignment().getQuantity();
	content["size_in_chars"     ] = record_layout.getSize().getQuantity();
	content["data_size_in_chars"] = record_layout.getDataSize().getQuantity();

	auto& bases_content = content["base_classes"];
	auto& fields_content = content["fields"];

	this->gen_class_all_bases_full_content(decl, record_layout, /* offset of first base */ 0, bases_content, fields_content);
	this->gen_class_all_fields_full_content(decl, record_layout, /* offset of first field */ 0, fields_content);

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

	auto& conv_funcs_content = content["conversion_functions"];
	for (const auto& conv_func: decl->getVisibleConversionFunctions())
		this->gen_named_decl_content(conv_func, conv_funcs_content.emplace_back());

	// TODO(FiTH): gen content for friends
}

#undef SET_VALUE_WITH_NAME
#undef SET_VALUE
#undef SET_VALUE_OF
#undef SET_VALUE_OF_PTR

Visitor::Visitor(const clang::ASTContext& context, inja::json& tmpl_content) noexcept
	: m_context(context)
	, m_printing_policy(context.getPrintingPolicy())

	, m_tmpl_content(tmpl_content             )
	, m_tmpl_classes(tmpl_content["classes"  ])
	, m_tmpl_enums  (tmpl_content["enums"    ])
	, m_tmpl_funcs  (tmpl_content["functions"])
{}

bool Visitor::VisitCXXRecordDecl(const clang::CXXRecordDecl* decl) noexcept
{
	// NOTE(FiTH): decl->isAnonymousStructOrUnion() does not work for some reason

	if (
		decl->isCompleteDefinition() &&
		decl->getIdentifier() != nullptr &&
		this->does_decl_require_content_gen(decl)
	) {
		auto& content = m_tmpl_classes.emplace_back();

		this->gen_decl_content(decl, content);
		this->gen_named_decl_content(decl, content, /* is_type_decl */ true);
		Visitor::gen_tag_decl_content(decl, content);
		Visitor::gen_record_decl_content(decl, content);

		this->gen_cxx_record_decl_content(decl, content);
	}

	return true;
}

bool Visitor::VisitEnumDecl(const clang::EnumDecl* decl) noexcept
{
	if (decl->isCompleteDefinition() && this->does_decl_require_content_gen(decl)) {
		auto& content = m_tmpl_enums.emplace_back();

		this->gen_decl_content(decl, content);
		this->gen_named_decl_content(decl, content, /* is_type_decl */ true);
		Visitor::gen_tag_decl_content(decl, content);

		this->gen_enum_decl_content(decl, content);
	}

	return true;
}

bool Visitor::VisitFunctionDecl(const clang::FunctionDecl* decl) noexcept
{
	// visit only non-member functions
	if (this->does_decl_require_content_gen(decl) && decl->getAccess() == clang::AccessSpecifier::AS_none) {
		auto& content = m_tmpl_funcs.emplace_back();

		this->gen_decl_content(decl, content);
		this->gen_named_decl_content(decl, content);

		this->gen_func_decl_content(decl, content);
	}

	return true;
}

void Visitor::post_visit() noexcept
{
	for (const auto& type: m_defined_types)
		m_used_types.erase(type);

	// TODO(FiTH): 'm_used_types' must be a reference?
	m_tmpl_content["used_types"] = std::move(m_used_types);
}

}
