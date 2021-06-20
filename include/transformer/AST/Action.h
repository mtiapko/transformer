#ifndef __TRANSFORMER_AST_ACTION_H__
#define __TRANSFORMER_AST_ACTION_H__

#include <clang/Frontend/FrontendAction.h>

#include "transformer/AST/Consumer.h"

namespace transformer::AST
{

class Action final : public clang::ASTFrontendAction
{
private:
	inja::json& m_tmpl_content;

public:
	Action(inja::json& tmpl_content) noexcept
		: m_tmpl_content(tmpl_content)
	{}

private:
	std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef) noexcept override
	{
		compiler.getPreprocessor().createPreprocessingRecord(); // TODO(FiTH): re-check this code

		return std::make_unique<Consumer>(compiler, m_tmpl_content);
	}
};

}

#endif // !__TRANSFORMER_AST_ACTION_H__
