#ifndef __TRANSFORMER_RTTI_TYPE_TRAITS_COPY_CONST_H__
#define __TRANSFORMER_RTTI_TYPE_TRAITS_COPY_CONST_H__

#include <type_traits>

namespace transformer::rtti::type_traits
{

template<typename SrcT, typename DstT>
using copy_const_t = std::conditional_t<std::is_const_v<SrcT>, const DstT, DstT>;

}

#endif // !__TRANSFORMER_RTTI_TYPE_TRAITS_COPY_CONST_H__
