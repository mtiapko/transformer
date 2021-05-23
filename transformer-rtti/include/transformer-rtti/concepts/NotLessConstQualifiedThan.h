#ifndef __TRANSFORMER_RTTI_CONCEPTS_NOT_LESS_CONST_QUALIFIED_THAN_H__
#define __TRANSFORMER_RTTI_CONCEPTS_NOT_LESS_CONST_QUALIFIED_THAN_H__

#include <type_traits>

namespace transformer::rtti
{

//   T   | CompareWithT | Result
// -----------------------------
//       |              |   OK
// const |              |   OK
// const |    const     |   OK
//       |    const     |  Fail

template<typename T, typename CompareWithT>
concept NotLessConstQualifiedThan = (std::is_const_v<T> || (std::is_const_v<CompareWithT> == false));

}

#endif // !__TRANSFORMER_RTTI_CONCEPTS_NOT_LESS_CONST_QUALIFIED_THAN_H__
