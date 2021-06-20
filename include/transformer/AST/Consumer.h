#ifndef __TRANSFORMER_AST_CONSUMER_H__
#define __TRANSFORMER_AST_CONSUMER_H__

#include <clang/AST/ASTConsumer.h>

#include "transformer/AST/Visitor.h"

namespace transformer::AST
{

class Consumer final : public clang::ASTConsumer
{
private:
	const clang::CompilerInstance& m_compiler;
	inja::json& m_tmpl_content;

public:
	Consumer(const clang::CompilerInstance& compiler, inja::json& tmpl_content) noexcept
		: m_compiler(compiler)
		, m_tmpl_content(tmpl_content)
	{}


private:
	void HandleTranslationUnit(clang::ASTContext& context) noexcept override
	{
		Visitor ast_visitor { m_compiler, context, m_tmpl_content };
		ast_visitor.TraverseDecl(context.getTranslationUnitDecl());
		ast_visitor.post_visit();
	}
};

}

#endif // !__TRANSFORMER_AST_CONSUMER_H__
