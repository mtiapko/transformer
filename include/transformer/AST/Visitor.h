#ifndef __TRANSFORMER_AST_VISITOR_H__
#define __TRANSFORMER_AST_VISITOR_H__

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

private:
	bool is_from_main_file(const clang::Decl* decl) const noexcept;

private:
	void        gen_type_content(const clang::QualType& type, inja::json& content) const noexcept;
	void        gen_decl_content(const clang::Decl* decl, inja::json& content) const noexcept;
	static void gen_named_decl_content(const clang::NamedDecl* decl, inja::json& content) noexcept;
	static void gen_tag_decl_content(const clang::TagDecl* decl, inja::json& content) noexcept;
	static void gen_record_decl_content(const clang::RecordDecl* decl, inja::json& content) noexcept;
	void        gen_enum_decl_content(const clang::EnumDecl* decl, inja::json& content) const noexcept;
	void        gen_func_decl_content(const clang::FunctionDecl* decl, inja::json& content) const noexcept;

	static void gen_class_dflt_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_copy_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_move_ctor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_copy_assign_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_move_assign_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;
	static void gen_class_destructor_content(const clang::CXXRecordDecl* decl, inja::json& content) noexcept;

	void        gen_class_all_bases_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
					clang::CharUnits::QuantityType offset_in_chars, inja::json& bases_content,
					inja::json& fields_content) const noexcept;
	void        gen_class_all_fields_full_content(const clang::CXXRecordDecl* decl, const clang::ASTRecordLayout& layout,
					clang::CharUnits::QuantityType base_offset_in_chars, inja::json& fields_content) const noexcept;
	void        gen_class_method_full_content(const clang::CXXMethodDecl* decl, inja::json& content) const noexcept;

	void        gen_cxx_record_decl_content(const clang::CXXRecordDecl* decl, inja::json& content) const noexcept;

public:
	Visitor(const clang::ASTContext& context, inja::json& tmpl_content) noexcept;

	bool VisitCXXRecordDecl(const clang::CXXRecordDecl* decl) const noexcept;
	bool VisitEnumDecl(const clang::EnumDecl* decl) const noexcept;
	bool VisitFunctionDecl(const clang::FunctionDecl* decl) const noexcept;
};

}

#endif // !__TRANSFORMER_AST_VISITOR_H__
