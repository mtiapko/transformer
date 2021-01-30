#ifndef __TRANSFORMER_AST_CONSUMER_H__
#define __TRANSFORMER_AST_CONSUMER_H__

#include <clang/AST/ASTConsumer.h>

#include "transformer/AST/Visitor.h"

namespace transformer::AST
{

class Consumer final : public clang::ASTConsumer
{
private:
	inja::json& m_tmpl_content;

public:
	Consumer(inja::json& tmpl_content) noexcept
		: m_tmpl_content(tmpl_content)
	{}


private:
	void HandleTranslationUnit(clang::ASTContext& context) noexcept override
	{
		Visitor ast_visitor { context, m_tmpl_content };
		ast_visitor.TraverseDecl(context.getTranslationUnitDecl());
	}
};

}

#endif // !__TRANSFORMER_AST_CONSUMER_H__
