#ifndef __TRANSFORMER_AST_VISITOR_H__
#define __TRANSFORMER_AST_VISITOR_H__

#include <filesystem>
#include <set>

#include <clang/AST/RecursiveASTVisitor.h>
#include <inja/inja.hpp>

namespace transformer::AST
{

class Visitor final : public clang::RecursiveASTVisitor<Visitor>
{
private:
	const clang::ASTContext&     m_context;
	const clang::PrintingPolicy& m_printing_policy;

	inja::json& m_tmpl_content;
	inja::json& m_tmpl_classes;
	inja::json& m_tmpl_enums;
	inja::json& m_tmpl_funcs;

	// TODO(FiTH): is it possible to define types with the same name in one file (not templates)?
	inja::json::object_t     m_used_types;
	std::vector<std::string> m_defined_types;

private: // TODO(FiTH): get this from command line
	std::string           m_rtti_attributes_namespace = "rtti::";
	std::filesystem::path m_relative_dir_path;

private:
	bool is_from_main_file(const clang::Decl* decl) const noexcept;
	static bool is_std_internal_name(std::string_view type_name) noexcept;
	bool does_decl_require_content_gen(const clang::Decl* decl) const noexcept;
	std::vector<std::string> split_annotate_attributes(const inja::json& annotate_attr, inja::json& content) const noexcept;
	std::string get_relative_path(std::string_view path) const noexcept;

private:
	void        gen_type_content(const clang::QualType& type, inja::json& content, bool is_used_type = false) noexcept;
	void        gen_decl_content(const clang::Decl* decl, inja::json& content) const noexcept;
	void        gen_named_decl_content(const clang::NamedDecl* decl, inja::json& content, bool is_type_decl = false) noexcept;
	static void gen_tag_decl_content(const clang::TagDecl* decl, inja::json& content) noexcept;
	static void gen_record_decl_content(const clang::RecordDecl* decl, inja::json& content) noexcept;
	void        gen_enum_decl_content(const clang::EnumDecl* decl, inja::json& content) noexcept;
	void        gen_func_decl_content(const clang::FunctionDecl* decl, inja::json& content) noexcept;

	static void gen_class_dflt_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_copy_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_move_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_copy_assign_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_move_assign_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_destructor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;

	void        gen_class_all_bases_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
					clang::CharUnits::QuantityType current_offset_in_chars, inja::json& bases_content,
					inja::json& fields_content) noexcept;
	void        gen_class_all_fields_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
					clang::CharUnits::QuantityType base_offset_in_chars, inja::json& fields_content) noexcept;
	void        gen_class_method_full_content(const clang::CXXMethodDecl* decl, inja::json& content) noexcept;

	void        gen_cxx_record_decl_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;

public:
	Visitor(const clang::ASTContext& context, inja::json& tmpl_content) noexcept;

	bool VisitCXXRecordDecl(const clang::CXXRecordDecl* decl) noexcept;
	bool VisitEnumDecl(const clang::EnumDecl* decl) noexcept;
	bool VisitFunctionDecl(const clang::FunctionDecl* decl) noexcept;

public:
	void post_visit() noexcept;
};

}

#endif // !__TRANSFORMER_AST_VISITOR_H__
