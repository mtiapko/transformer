#ifndef __TRANSFORMER_RTTI_OBJECT_H__
#define __TRANSFORMER_RTTI_OBJECT_H__

#include "transformer-rtti/ObjectView.h"

namespace transformer::rtti
{

class Object
{
private: /* data */
	// TODO(FiTH): magic number! replace 0 with 'Null TypeID'?
	TypeID m_type_id = 0; // TODO(FiTH): must be QualifiedType?

	// forward declaration
	template<typename> friend struct IntrusiveObjectHelper;

public: /* API */
	TypeID object_type_id() const noexcept { return m_type_id; }

	// TODO(FiTH): too big overhead?
	QualifiedType object_qual_type() noexcept       { return { m_type_id, {} }; }
	QualifiedType object_qual_type() const noexcept { return QualifiedType::add_local_const({ m_type_id, {} }); }

	ObjectView object_find_field(StringView name) noexcept
	{
		return ObjectView::find_field(this->object_qual_type(), this, name);
	}

	ObjectView object_find_field(StringView name) const noexcept
	{
		return ObjectView::find_field(this->object_qual_type(), this, name);
	}

public: /* operators */
	// TODO(FiTH): move const_cast to constructor?
	operator ObjectView() noexcept       { return { this->object_qual_type(), this }; }
	operator ObjectView() const noexcept { return { this->object_qual_type(), this }; }
};

}

#endif // !__TRANSFORMER_RTTI_OBJECT_H__
