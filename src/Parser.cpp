#include "transformer/Exception.h"
#include "transformer/Parser.h"
#include "transformer/Util.h"
#include "transformer/Log.h"

namespace trans
{

/* static */ CXChildVisitResult Parser::visitor(CXCursor cur, CXCursor /* parent */,
	CXClientData closure) noexcept
{
	auto parser = static_cast<Parser*>(closure);
	switch (clang_getCursorKind(cur)) {
		case CXCursor_ClassDecl:
		case CXCursor_StructDecl: {
			// TODO(FiTH): goto def?
			if (clang_Cursor_isAnonymous(cur))
				return CXChildVisit_Continue;

			auto full_name = Util::get_full_name(cur);
			if (full_name.empty())
				return CXChildVisit_Continue;

			parser->classes().add(std::move(full_name), parser, cur);
			break;
		}

		case CXCursor_EnumDecl: {
			if (clang_Cursor_isAnonymous(cur))
				return CXChildVisit_Continue;

			auto full_name = Util::get_full_name(cur);
			if (full_name.empty())
				return CXChildVisit_Continue;

			parser->enums().add(Util::get_full_name(cur), cur);
			break;
		}

		default: break;
	}

	return CXChildVisit_Recurse;
}

Parser::Parser(const Config& cfg)
{
	std::unique_ptr<
		std::remove_pointer_t<CXIndex>, decltype(&clang_disposeIndex)
	> index { clang_createIndex(/* excludeDeclsFromPCH */ 1, 0), &clang_disposeIndex };

	std::unique_ptr<
		std::remove_pointer_t<CXTranslationUnit>, decltype(&clang_disposeTranslationUnit)
	> unit {
		clang_parseTranslationUnit(
			index.get(), cfg.src_file_path().c_str(),
			cfg.compiler_argv(), cfg.compiler_argc(),
			nullptr, 0,
			CXTranslationUnit_None
		),
		clang_disposeTranslationUnit
	};

	if (unit == nullptr)
		throw TRANS_EXCEPTION("Failed to parse translation unit: ",
			cfg.src_file_path().string());

	bool stop_parsing = false;
	auto diags_count = clang_getNumDiagnostics(unit.get());
	for (unsigned i = 0; i != diags_count; ++i) {
		std::unique_ptr<
			std::remove_pointer_t<CXDiagnostic>, decltype(&clang_disposeDiagnostic)
		> diag { clang_getDiagnostic(unit.get(), i), clang_disposeDiagnostic };

		stop_parsing |= (clang_getDiagnosticSeverity(diag.get()) > CXDiagnostic_Warning);
		TRANS_PRINT_ERR("[ clang ] ", clang_formatDiagnostic(diag.get(),
			clang_defaultDiagnosticDisplayOptions()));
	}

	if (stop_parsing)
		throw TRANS_EXCEPTION("Error during parsing translation unit: ",
			cfg.src_file_path().string());

	auto cursor = clang_getTranslationUnitCursor(unit.get());
	clang_visitChildren(cursor, Parser::visitor, this);
}

}
