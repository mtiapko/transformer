#ifndef __TRANSFORMER_RTTI_CONCEPTS_IS_VOID_OR_CONST_VOID_H__
#define __TRANSFORMER_RTTI_CONCEPTS_IS_VOID_OR_CONST_VOID_H__

#include <type_traits>

namespace transformer::rtti
{

template<typename T>
concept IsVoidOrConstVoid = std::is_same_v<std::remove_const_t<T>, void>;

}

#endif // !__TRANSFORMER_RTTI_CONCEPTS_IS_VOID_OR_CONST_VOID_H__
