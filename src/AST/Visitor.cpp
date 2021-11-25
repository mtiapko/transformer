#include <charconv>

#include <clang/Basic/SourceManager.h>
#include <clang/AST/RecordLayout.h>
#include <clang/Lex/PreprocessingRecord.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/Lexer.h>

#include "transformer/AST/TypePrinter.h"
#include "transformer/AST/Visitor.h"
#include "transformer/Config.h"
#include "transformer/Log.h"

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
	// TODO(FiTH): check 'src_mgr.isInMainFile()' and 'src_mgr.isWrittenInMainFile()'
	const auto& src_mgr = m_context.getSourceManager();
	return (src_mgr.getFileID(decl->getLocation()) == src_mgr.getMainFileID());
}

/* static */ bool Visitor::is_std_internal_name(std::string_view type_name) noexcept
{
	const auto is_capital = [](const char c) noexcept { return (c >= 'A' && c <= 'Z'); };

	if (type_name.size() >= 2 && type_name[0] == '_' && (type_name[1] == '_' || is_capital(type_name[1])))
		return true; // TODO(FiTH): check if TOP level namespace is 'std'

	return false;
}

bool Visitor::does_decl_require_content_gen(const clang::Decl* decl) const noexcept
{
	// TODO(FiTH): check this? - decl->isDefinedOutsideFunctionOrMethod()

	return (Config::gen_content_for_includes_opt || this->is_from_main_file(decl));
}

// TODO(FiTH): ahhh... why code that works with text looks like... this. is this a problem with me?
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

		static_assert(sizeof(chars_state.data) == sizeof(chars_state.total_count));

		// TODO(FiTH): add assert if curr == '\0' but total_count != 0?
		// TODO(FiTH): add assert if count < 0?
		// TODO(FiTH): handle \" \' cases?

		// NOTE(FiTH): std::string is always null-terminated (str[str.size()] == CharT())
		for (size_t i = beg_index; i <= str.size(); ++i) {
			const char curr = str[i];

			if ((curr == c && chars_state.total_count == 0) || (curr == '\0')) {
				// TODO(FiTH)
				assert(chars_state.data.double_quote == 0 && chars_state.data.single_quote == 0);

				return i;
			}

			// TODO(FiTH): add backslash support (\' and \")
			if      (curr == '"' ) chars_state.data.double_quote ^= 1; // invert
			else if (curr == '\'') chars_state.data.single_quote ^= 1; // invert
			else if (curr == '[') ++chars_state.data.l_bracket_count;
			else if (curr == ']') --chars_state.data.l_bracket_count;
			else if (curr == '{') ++chars_state.data.l_brace_count;
			else if (curr == '}') --chars_state.data.l_brace_count;
			else if (curr == '(') ++chars_state.data.l_paren_count;
			else if (curr == ')') --chars_state.data.l_paren_count;
		}

		return str.size();
	};

	const auto is_whitespace = [](const char c) noexcept
	{
		return (c == ' ' || c == '\t' || c == '\n');
	};

	const auto skip_whitespaces = [&is_whitespace](const std::string_view str, size_t beg_index, size_t end_index) noexcept
	{
		// '\0' is not a whitespace
		while (is_whitespace(str[beg_index]))
			++beg_index;

		// at the beginning 'end_index' is index of ',' or '\0'. Using '-1' to skip them
		while (beg_index < end_index && is_whitespace(str[end_index - 1]))
			--end_index;

		return std::pair { beg_index, end_index };
	};

	std::vector<std::string> attr_list {};
	for (const auto& attr: annotate_attr) {
		const auto& attr_str = attr.get_ref<const std::string&>();

		for (size_t beg = 0, end = 0; beg < attr_str.size(); beg = end + 1 /* skip ',' */) {
			end = find_char(attr_str, beg, ',');

			const auto [attr_beg, attr_end] = skip_whitespaces(attr_str, beg, end);
			if (attr_beg == attr_end)
				continue;

			// TODO(FiTH): add assert attr_beg < attr_end
			assert(attr_beg < attr_end);

			auto parsed_attr = std::string_view { attr_str.data() + attr_beg, attr_end - attr_beg };
			if (parsed_attr.starts_with(m_rtti_attributes_namespace)) {
				parsed_attr = parsed_attr.substr(m_rtti_attributes_namespace.size());

				// NOTE(FiTH): this attr starts with rtti prefix, so it does not have any whitespaces at the beginning
				size_t       equal_sign_index = 0;
				const size_t key_beg          = equal_sign_index;

				for (/* empty */; equal_sign_index < parsed_attr.size(); ++equal_sign_index) {
					// TODO(FiTH): TODO
					assert(parsed_attr[equal_sign_index] != ('_' | (1 << 5)));

					// TODO(FiTH): is it possible to have non-printable (e.g. new line, tab) in annotate attr?
					// all printable chars have (1 << 5) bit
					// capital    -> lower case
					// lower case -> lower case
					// digit      -> digit
					// '_' (0x5f) -> (0x7F)
					// '-'        -> '-' TODO(FiTH): allow this?
					const auto c = (parsed_attr[equal_sign_index] | (1 << 5));
					if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')
						continue;

					break;
				}

				const auto key_end = equal_sign_index;
				while (is_whitespace(parsed_attr[equal_sign_index]))
					++equal_sign_index;

				// NOTE(FiTH): 'parsed_attr' points to 'attr_str' (std::string), so it is always null-terminated
				if (parsed_attr[equal_sign_index] == '=') {
					const auto [val_beg, val_end] = skip_whitespaces(parsed_attr, equal_sign_index + 1, parsed_attr.size());

					// NOTE(FiTH): if attr has '\'' at the beginning, then it should also has it at the end
					assert((parsed_attr[val_beg] == '\'') == (parsed_attr[val_end - 1] == '\'')
						&& ((parsed_attr[val_beg] != '\'') || (val_end - val_beg >= 2)));

					const auto key = std::string_view { parsed_attr.data() + key_beg, key_end - key_beg };
					const auto val = std::string_view { // TODO(FiTH): this should be done in json lib
						parsed_attr.data() + val_beg + (parsed_attr[val_beg] == '\''),
						val_end            - val_beg - (parsed_attr[val_beg] == '\'') * 2
					};

					// TODO(FiTH): find API to work with sting_view
					content[std::string(key)] = (parsed_attr[val_beg] == '\'' ? val : inja::json::parse(val));
				} else {
					// TODO(FiTH): find API to work with sting_view
					content[std::string(parsed_attr)] = true;
				}

				continue;
			}

			attr_list.emplace_back(parsed_attr);
		}
	}

	return attr_list;
}

std::string Visitor::get_relative_path(std::string_view path) const noexcept
{
	return std::filesystem::relative(path, m_relative_dir_path);
}

// TODO(FiTH): erase 'volatile'
/* static */ std::string Visitor::erase_all_const(std::string str) noexcept
{
	// NOTE(FiTH): std::string is always null-terminated (str[str.size()] == CharT())
	const auto skip_whitespaces = [](const std::string& str, size_t beg_index) noexcept
	{
		while (true) {
			const char c = str[beg_index];
			if (c != ' ' && c != '\t')
				break;

			++beg_index;
		}

		return beg_index;
	};

	const auto is_not_identifier_part = [](const char current_char) noexcept
	{
		const auto to_lower_case = [](const char c) noexcept { return (c >= 'A' && c <= 'Z' ? (c | (1 << 5)) : c); };
		const auto c = to_lower_case(current_char);

		return (
			(c < 'a' || c > 'z') &&
			(c < '0' || c > '9') &&
			c != '_'
		);
	};

	size_t const_iter = 0;

	while ((const_iter = str.find("const", const_iter)) != std::string::npos) {
		const auto beg_iter = const_iter;
		const auto end_iter = const_iter + 5; // TODO(FiTH): "const"

		if (
				(beg_iter == 0          || is_not_identifier_part(str[beg_iter - 1])) &&
				(end_iter == str.size() || is_not_identifier_part(str[end_iter]))
		) {
			str.erase(beg_iter, skip_whitespaces(str, end_iter) - beg_iter);
		} else {
			const_iter += 5; // TODO(FiTH): "const"
		}
	}

	return str;
}

/* static */ void Visitor::append_scope(const clang::DeclContext* decl_ctx, llvm::raw_ostream& ostream) noexcept
{
	if (decl_ctx->isTranslationUnit())
		return;

	// FIXME: Consider replacing this with NamedDecl::printNestedNameSpecifier,
	// which can also print names for function and method scopes.
	if (decl_ctx->isFunctionOrMethod())
		return;

	// if (Policy.Callbacks && Policy.Callbacks->isScopeVisible(decl_ctx))
	// 	return;

	if (const auto *NS = dyn_cast<clang::NamespaceDecl>(decl_ctx)) {
		// if (Policy.SuppressUnwrittenScope && NS->isAnonymousNamespace())
		// 	return Visitor::append_scope(decl_ctx->getParent(), ostream, NameInScope);

		// Only suppress an inline namespace if the name has the same lookup
		// results in the enclosing namespace.
		// if (Policy.SuppressInlineNamespace && NS->isInline() && NameInScope &&
		// 		NS->isRedundantInlineQualifierFor(NameInScope))
		// 	return Visitor::append_scope(decl_ctx->getParent(), ostream, NameInScope);

		Visitor::append_scope(decl_ctx->getParent(), ostream);
		if (NS->getIdentifier())
			ostream << NS->getName() << "::";
		else
			ostream << "(anonymous namespace)::";
	} else if (const auto *Spec = dyn_cast<clang::ClassTemplateSpecializationDecl>(decl_ctx)) {
		// 	assert(false);
		(void)Spec;
		ostream << "(class tmpl spec)";

		// Visitor::append_scope(decl_ctx->getParent(), ostream, Spec->getDeclName());
		// IncludeStrongLifetimeRAII Strong(Policy);
		// ostream << Spec->getIdentifier()->getName();
		// const TemplateArgumentList &TemplateArgs = Spec->getTemplateArgs();
		// printTemplateArgumentList(
		// 	ostream, TemplateArgs.asArray(), Policy,
		// 	Spec->getSpecializedTemplate()->getTemplateParameters());
		// ostream << "::";
	} else if (const auto *Tag = dyn_cast<clang::TagDecl>(decl_ctx)) {
		Visitor::append_scope(decl_ctx->getParent(), ostream);
		if (clang::TypedefNameDecl *Typedef = Tag->getTypedefNameForAnonDecl())
			ostream << Typedef->getIdentifier()->getName() << "::";
		else if (Tag->getIdentifier())
			ostream << Tag->getIdentifier()->getName() << "::";
		else
			return;
	} else {
		Visitor::append_scope(decl_ctx->getParent(), ostream);
	}
}

std::string Visitor::get_qualified_name_as_string(const clang::NamedDecl* decl) const noexcept
{
	std::string qual_name;
	llvm::raw_string_ostream out_stream(qual_name);

	auto print_policy = m_printing_policy;
	print_policy.SuppressUnwrittenScope = true;
	decl->printQualifiedName(out_stream, print_policy);

	return qual_name;
}

clang::AccessSpecifier Visitor::push_current_access_specifier(clang::AccessSpecifier access) noexcept
{
	static_assert(
		clang::AccessSpecifier::AS_public    < clang::AccessSpecifier::AS_protected &&
		clang::AccessSpecifier::AS_protected < clang::AccessSpecifier::AS_private,
		"Unexpected enum values"
	);

	const auto prev_access = m_current_access_specifier;
	if (m_current_access_specifier < access)
		m_current_access_specifier = access;

	return prev_access;
}

#define SET_VALUE_OF_PTR(name, decl, value) content[str_to_lower_case(#name).data()] = decl->value()
#define SET_VALUE_OF(decl, value)           SET_VALUE_OF_PTR(value, (&decl), value)
#define SET_VALUE(value)                    SET_VALUE_OF_PTR(value, decl, value)
#define SET_VALUE_WITH_NAME(value, name)    SET_VALUE_OF_PTR(name, decl, value)

// NOTE(FiTH): 'is_type_used' is true only for types of fields (required only for serialization?)
void Visitor::gen_type_content(const clang::QualType& type, inja::json& content, bool is_type_used /* = false */) noexcept
{
	auto access = clang::AccessSpecifier::AS_none;
	bool is_std_internal_type = false;

	if (type->isRecordType()) {
		const auto* record_type = type->getAsRecordDecl();
		access = record_type->getAccess();

		const auto& record_name = record_type->getName();
		is_std_internal_type = Visitor::is_std_internal_name(record_name);
	}

	SET_VALUE_OF(type, isConstQualified);
	SET_VALUE_OF(type, isVolatileQualified);

	content["is_builtin"          ] = type->isBuiltinType();
	content["is_record"           ] = type->isRecordType();
	content["is_enumeral"         ] = type->isEnumeralType();
	content["is_signed_integer"   ] = type->isSignedIntegerType();
	content["is_unsigned_integer" ] = type->isUnsignedIntegerType();
	content["is_function_pointer" ] = type->isFunctionPointerType();
	content["is_std_internal_type"] = is_std_internal_type;

	// TODO(FiTH): record or enum?
	// TODO(FiTH): rename to 'access_specifier'?
	content["access"] = (access != clang::AccessSpecifier::AS_none ? clang::getAccessSpelling(access) : "");

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

		content["is_lvalue_reference"] = (ref_type != nullptr && type->getAs<clang::LValueReferenceType>());
		content["is_rvalue_reference"] = (ref_type != nullptr && type->getAs<clang::RValueReferenceType>());
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
	content["canonical_name"] = type.getCanonicalType().getAsString(m_printing_policy);

	// TODO(FiTH): add cmd-line flag to report even 'std internal' types?
	if (is_type_used && is_std_internal_type == false) {
		if (const auto* template_spec = type->getAs<clang::TemplateSpecializationType>(); template_spec != nullptr) {
			m_used_template_spec.emplace_back(type.getTypePtr());
		} else if (type->isBuiltinType() == false || Config::report_used_builtin_types_opt) {
			// TODO(FiTH): add this as a new field to content?
			// TODO(FiTH): canonical_unqualified_name
			auto canonical_unqualified_name = Visitor::erase_all_const(
				type.getCanonicalType().withoutLocalFastQualifiers().getAsString(m_printing_policy)
			);

			m_used_types.try_emplace(canonical_unqualified_name, content);
		}
	}
}

void Visitor::gen_decl_content(const clang::Decl* decl, inja::json& content) const noexcept
{
	SET_VALUE(isTemplated);
	SET_VALUE(isTemplateDecl);
	SET_VALUE(isInStdNamespace);
	SET_VALUE(isInAnonymousNamespace);

	// TODO(FiTH): rename 'access' to 'access_specifier'?
	auto decl_access = decl->getAccess();
	if (decl_access != clang::AccessSpecifier::AS_none)
		content["access"] = clang::getAccessSpelling(std::max(decl_access, m_current_access_specifier));

	const auto& src_mgr = m_context.getSourceManager();
	content["file_path"] = src_mgr.getFilename(decl->getLocation());

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
			m_defined_types.emplace(full_name);
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

		// TODO(FiTH): if enum_init_val.{getMinSignedBits or getActiveBits}() > 32 -> add "l" or "ll" at the end?
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
	clang::CharUnits::QuantityType current_offset_in_chars, inja::json& bases_content, inja::json& fields_content,
	inja::json& methods_content, inja::json& constructors_content, inja::json& overloaded_operators_equal_content) noexcept
{
	for (const auto& base: decl->bases()) {
		const auto* base_decl   = base.getType()->getAsCXXRecordDecl();
		const auto& base_layout = m_context.getASTRecordLayout(base_decl);

		// TODO(FiTH): getVBaseClassOffset?
		auto base_offset_in_chars = layout.getBaseClassOffset(base_decl).getQuantity() + current_offset_in_chars;

		const auto prev_access = this->push_current_access_specifier(base.getAccessSpecifier());
		this->gen_class_all_bases_full_content(base_decl, base_layout, base_offset_in_chars, bases_content, fields_content,
			methods_content, constructors_content, overloaded_operators_equal_content);

		// TODO(FiTH): remove?
		//
		// std::string base_name_scope;
		// if (base.getType()->getTypeClass() == clang::Type::TypeClass::TemplateSpecialization) {
		// 	const auto* base_record_decl = base.getType()->getAsRecordDecl();
		// 	assert(base_record_decl != nullptr); // TODO(FiTH)

		// 	llvm::raw_string_ostream out_stream(base_name_scope);
		// 	Visitor::append_scope(base_record_decl->getDeclContext(), out_stream);
		// }

		// const auto base_type_name = base.getType().getAsString(m_printing_policy);
		const auto base_name = GetQualTypeAsString(base.getType(), m_printing_policy); // base_name_scope + base_type_name;

		auto [content_iter, is_inserted] = bases_content.emplace(base_name, nullptr /* empty json */);
		assert(is_inserted); // TODO(FiTH): 2 base classes with the same name?

		auto& content = *content_iter;
		content["name"                ] = base_name;
		content["access_specifier"    ] = clang::getAccessSpelling(base.getAccessSpecifier());
		content["is_std_internal_type"] = Visitor::is_std_internal_name(base_name);

		content["offset_in_chars"       ] = base_offset_in_chars;
		content["vb_ptr_offset_in_chars"] = layout.getVBPtrOffset().getQuantity();
		content["has_own_vf_ptr"        ] = layout.hasOwnVFPtr();
		content["has_extendable_vf_ptr" ] = layout.hasExtendableVFPtr();
		content["has_own_vb_ptr"        ] = layout.hasOwnVBPtr();
		content["has_vb_ptr"            ] = layout.hasVBPtr();

		SET_VALUE_OF(base, isVirtual);
		SET_VALUE_OF(base, getInheritConstructors);

		this->gen_class_all_fields_full_content(base_decl, base_layout, base_offset_in_chars, fields_content);
		this->gen_class_all_methods_full_content(base_decl, methods_content,
			/* constructors_content */ nullptr, /* overloaded_operators_equal_content */ nullptr);

		/* restore previous access specifier */
		m_current_access_specifier = prev_access;

	}
}

void Visitor::gen_class_all_fields_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
	clang::CharUnits::QuantityType base_offset_in_chars, inja::json& fields_content) noexcept
{
	const auto base_offset_in_bits = base_offset_in_chars * CHAR_BIT;

	for (const auto& field: decl->fields()) {
		auto& content = fields_content.emplace_back();
		const bool is_anonymous_struct_or_union = field->isAnonymousStructOrUnion();
		content["is_anonymous_struct_or_union"] = is_anonymous_struct_or_union;
		content["offset_in_bits"] = layout.getFieldOffset(field->getFieldIndex()) + base_offset_in_bits;

		// TODO(FiTH): add check and set to MAX if this is bit field
		content["offset_in_chars"] = layout.getFieldOffset(field->getFieldIndex()) / CHAR_BIT + base_offset_in_chars;

		this->gen_decl_content(field, content);
		this->gen_named_decl_content(field, content);
		this->gen_type_content(field->getType(), content["type"], /* is_type_used */ (is_anonymous_struct_or_union == false));
	}
}

void Visitor::gen_class_all_methods_full_content(const clang::CXXRecordDecl* decl, inja::json& methods_content,
	inja::json* constructors_content, inja::json* overloaded_operators_equal_content) noexcept
{
	// TODO(FiTH): handle case with overrided methods
	for (const auto& method: decl->methods()) {
		// TODO(FiTH): skip this decl? or set 'content_ptr' to 'conversion_functions'?
		if (llvm::isa<clang::CXXConversionDecl>(method))
			continue;

		if (llvm::isa<clang::CXXDestructorDecl>(method))
			continue;

		// TODO(FiTH): add cmd-line flag to report deprecated methods?
		if (method->hasAttr<clang::DeprecatedAttr>())
			continue;

		inja::json* content_ptr = (llvm::isa<clang::CXXConstructorDecl>(method)
			? constructors_content
			: method->getOverloadedOperator() == clang::OverloadedOperatorKind::OO_Equal
				? overloaded_operators_equal_content
				: &methods_content
		);

		if (content_ptr == nullptr)
			continue;

		this->gen_class_method_full_content(method, content_ptr->emplace_back());
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

	// TODO(FiTH): 'fields' and 'methods' must be objects, not array? format: { "<field-name>", { <field-content> } }?
	// TODO(FiTH): handle inherited fields with the same name. For duplicated fields from bases add prefix "Base::"?
	auto& bases_content = content["base_classes"];
	auto& fields_content = content["fields"];

	auto& methods_content                    = content["methods"];
	auto& constructors_content               = content["constructors"];
	auto& overloaded_operators_equal_content = content["overloaded_operators_equal"];

	this->gen_class_all_bases_full_content(decl, record_layout, /* offset of first base */ 0, bases_content, fields_content,
		methods_content, constructors_content, overloaded_operators_equal_content);
	this->gen_class_all_fields_full_content(decl, record_layout, /* offset of first field */ 0, fields_content);
	this->gen_class_all_methods_full_content(decl, methods_content, &constructors_content, &overloaded_operators_equal_content);

	auto& conv_funcs_content = content["conversion_functions"];
	for (const auto& conv_func: decl->getVisibleConversionFunctions())
		this->gen_named_decl_content(conv_func, conv_funcs_content.emplace_back());

	// TODO(FiTH): gen content for friends
}

#undef SET_VALUE_WITH_NAME
#undef SET_VALUE
#undef SET_VALUE_OF
#undef SET_VALUE_OF_PTR

Visitor::Visitor(const clang::CompilerInstance& compiler, const clang::ASTContext& context, inja::json& tmpl_content) noexcept
	: m_compiler(compiler)
	, m_context(context)
	, m_printing_policy(context.getPrintingPolicy())

	, m_tmpl_content(tmpl_content             )
	, m_tmpl_classes(tmpl_content["classes"  ])
	, m_tmpl_enums  (tmpl_content["enums"    ])
	, m_tmpl_funcs  (tmpl_content["functions"])

	, m_current_access_specifier(clang::AccessSpecifier::AS_public) // NOTE(FiTH): AS_none == 3!

	, m_relative_dir_path(
		Config::relative_dir_path_opt.empty() == false
			? std::filesystem::path(Config::relative_dir_path_opt.getValue())
			: std::filesystem::current_path()
	)
{
	// TODO(FiTH): remove this and use flag only in local scope (like in 'get_qualified_name_as_string')?
	m_printing_policy.SuppressUnwrittenScope = true;

	const auto& src_mgr = m_context.getSourceManager();
	m_tmpl_content["main_file_path"] = Visitor::get_relative_path(
		src_mgr.getFilename(src_mgr.getLocForStartOfFile(src_mgr.getMainFileID()))
	);

	auto& included_files_content = m_tmpl_content["included_files"];
	if (auto* pp_record = m_compiler.getPreprocessor().getPreprocessingRecord(); pp_record != nullptr) {
		for (const auto* pp_entity: *pp_record) {
			if (pp_entity->getKind() != clang::PreprocessedEntity::EntityKind::InclusionDirectiveKind)
				continue;

			const auto* include_entity = static_cast<const clang::InclusionDirective*>(pp_entity);
			if (include_entity->getKind() != clang::InclusionDirective::InclusionKind::Include)
				continue;

			if (src_mgr.isInMainFile(include_entity->getSourceRange().getBegin()) == false)
				continue;

			auto& include_content = included_files_content.emplace_back();
			include_content["file_path"] = include_entity->getFileName();
			include_content["was_in_quotes"] = include_entity->wasInQuotes();

		}
	}
}

bool Visitor::VisitCXXRecordDecl(const clang::CXXRecordDecl* decl) noexcept
{
	// NOTE(FiTH): decl->isAnonymousStructOrUnion() does not work for some reason

	// TODO(FiTH): add? decl->getDeclKind() != ClassTemplatePartialSpecialization and Partial?
	// TODO(FiTH): add? decl->isDependentType() == false
	if (
		decl->isCompleteDefinition() &&
		decl->getIdentifier() != nullptr &&
		this->does_decl_require_content_gen(decl)
	) {
		// TODO(FiTH): rewrite this 'if'?
		// TODO(FiTH): add this as flag or remove?
		// const auto& src_mgr = m_context.getSourceManager();
		// if (src_mgr.isInSystemHeader(decl->getLocation()))
		// 	return true;

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
	if (
		decl->isCompleteDefinition() &&
		this->does_decl_require_content_gen(decl) &&

		// TODO(FiTH): this is (anonymous) enum? rewrite, check NamedDecl::printQualifiedName,
		//             method NamedDecl::printName is virtual?
		decl->getDeclName().isEmpty() == false
	) {
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
	// TODO(FiTH): rewrite this as soon as possible!
	while (m_used_template_spec.empty() == false) {
		// TODO(FiTH):
		//
		// template_spec_full_name -> std::vector<float>
		// template_decl_full_name -> std::vector
		// args                    -> [ float ]

		const auto* template_spec_type = m_used_template_spec.back();
		auto template_spec_full_name = clang::QualType(template_spec_type, 0).getAsString(m_printing_policy);
		m_used_template_spec.pop_back();

		if (m_defined_types.contains(template_spec_full_name))
			continue;

		const auto* template_spec = template_spec_type->getAs<clang::TemplateSpecializationType>();
		assert(template_spec != nullptr); // TODO(FiTH)

		const auto* template_decl = template_spec->getTemplateName().getAsTemplateDecl();
		assert(template_decl != nullptr); // TODO(FiTH)

		auto template_decl_full_name = this->get_qualified_name_as_string(template_decl);
		auto template_decl_name      = template_decl->getNameAsString();

		if (m_defined_types.contains(template_decl_full_name) == false) {
			// TODO(FiTH): rewrite? If 'template_spec_type' is 'std::vector<int>' this code will report 'std::vector' as used
			auto [record_template_iter, _] = m_used_types.try_emplace(template_decl_full_name);
			auto& record_template = record_template_iter->second;
			record_template["name"              ] = template_decl_name;
			record_template["full_name"         ] = template_decl_full_name;
			record_template["is_record_template"] = true;
		}

		m_defined_types.emplace(template_spec_full_name);

		const auto& decl = template_spec_type->getAsCXXRecordDecl();
		if (decl->isCompleteDefinition() == false || decl->getIdentifier() == nullptr) {
			TF_LOG_WARN("Templated type '", template_spec_full_name, "' is not defined. Use 'RTTI_DEFINE_TYPE()' macro");
			continue;
		}

		std::string template_spec_name;
		if (const auto* template_spec_typedef = template_spec_type->getAs<clang::TypedefType>(); template_spec_typedef == nullptr) {
			template_spec_name = clang::QualType(template_spec_type->getAs<clang::TemplateSpecializationType>(), 0)
				.getAsString(m_printing_policy);
		} else {
			template_spec_name = template_spec_typedef->getDecl()->getNameAsString();
		}

		// TODO(FiTH): type->getAs<clang::ElaboratedType>()->getNamedType()->getTypeClassName() == Typedef for std::string.
		//             Check source code of getAs<clang::TemplateSpecializationType>()
		//
		// const auto* template_spec = type->getAs<clang::TemplateSpecializationType>();
		// content["is_template_specialization"] = (template_spec != nullptr);

		auto& content = m_tmpl_classes.emplace_back();
		content["name"                      ] = std::move(template_spec_name);
		content["full_name"                 ] = std::move(template_spec_full_name);
		content["is_template_specialization"] = true;

		// TODO(FiTH): code duplication, same as in VisitCXXRecordDecl (except 'is_from_main_file' and 'gen_named_decl')
		this->gen_decl_content(decl, content);
		// TODO(FiTH): remove? - this->gen_named_decl_content(decl, content, /* is_type_decl */ true);
		Visitor::gen_tag_decl_content(decl, content);
		Visitor::gen_record_decl_content(decl, content);

		this->gen_cxx_record_decl_content(decl, content);

		auto& template_content = content["template"];
		template_content["name"     ] = std::move(template_decl_name);
		template_content["full_name"] = std::move(template_decl_full_name);

		auto& template_args_content = template_content["args"];

		for (uint32_t i = 0; i < template_spec->getNumArgs(); ++i) {
			const auto& template_arg = template_spec->getArg(i);
			if (template_arg.getKind() != clang::TemplateArgument::ArgKind::Type) {
				llvm::errs() << "TODO(FiTH): unsupported Template Argument kind: " << template_arg.getKind() << '\n';
				continue;
			}

			auto& arg_content = template_args_content.emplace_back();
			this->gen_type_content(template_arg.getAsType(), arg_content, /* is_type_used */ true);
		}
	}

	for (const auto& type: m_defined_types)
		m_used_types.erase(type);

	// TODO(FiTH): 'm_used_types' must be a reference?
	m_tmpl_content["used_types"] = std::move(m_used_types);
}

}
