#include <clang/AST/Type.h>

namespace transformer::AST
{

std::string GetQualTypeAsString(const clang::QualType& qual_type, const clang::PrintingPolicy &Policy);

}
