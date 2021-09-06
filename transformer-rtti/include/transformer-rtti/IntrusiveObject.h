#ifndef __TRANSFORMER_RTTI_INTRUSIVE_OBJECT_H__
#define __TRANSFORMER_RTTI_INTRUSIVE_OBJECT_H__

#include "transformer-rtti/Object.h"

namespace transformer::rtti
{

// TODO(FiTH): move to 'detail' namespace?
template<typename T>
struct IntrusiveObjectHelper
{
	IntrusiveObjectHelper() noexcept
	{
		static_cast<T*>(this)->Object::m_type_id = DB::get_type_id<T>();
	}
};

template<typename ObjectT, typename BaseT>
struct IntrusiveObject
	: public BaseT
	, public IntrusiveObjectHelper<ObjectT>
{
	static_assert(std::is_same_v<ObjectT, BaseT> == false);
	static_assert(std::is_base_of_v<Object, BaseT>);

	using BaseT::BaseT;

	// TODO(FiTH): remove?
	using Super = BaseT;
};

}

#endif // !__TRANSFORMER_RTTI_INTRUSIVE_OBJECT_H__
