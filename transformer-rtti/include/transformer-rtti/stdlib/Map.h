#ifndef __TRANSFORMER_RTTI_STDLIB_MAP_H__
#define __TRANSFORMER_RTTI_STDLIB_MAP_H__

#include "transformer-rtti/Config.h"

#include <map>

namespace transformer::rtti
{

template<typename KeyT, typename ValueT>
using Map = std::map<KeyT, ValueT>;

}

#endif // !__TRANSFORMER_RTTI_STDLIB_MAP_H__
