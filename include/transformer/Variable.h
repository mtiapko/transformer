#ifndef __TRANSFORMER_VARIABLE_H__
#define __TRANSFORMER_VARIABLE_H__

#include "transformer/Entity.h"

namespace transformer
{

class Variable : public Entity
{
private:
	std::string m_type;

	bool m_is_builtin_type = false;
	bool m_is_pointer_type = false;
	bool m_is_lvalue_reference_type = false;
	bool m_is_rvalue_reference_type = false;
	bool m_is_array_type = false;
	bool m_is_enum_type = false;

	/* for array */ // TODO(FiTH): make another class?
	std::string m_element_type;
	size_t m_elements_count = 0;

public:
	Variable(CXCursor cur) noexcept;

	const auto& type() const noexcept { return m_type; }

	bool is_builtin_type() const noexcept { return m_is_builtin_type; }
	bool is_pointer_type() const noexcept { return m_is_pointer_type; }
	bool is_lvalue_reference_type() const noexcept { return m_is_lvalue_reference_type; }
	bool is_rvalue_reference_type() const noexcept { return m_is_rvalue_reference_type; }
	bool is_array_type() const noexcept { return m_is_array_type; }
	bool is_enum_type() const noexcept { return m_is_enum_type; }

	bool is_incomplete_array() const noexcept { return (m_elements_count == std::numeric_limits<size_t>::max()); }
	const auto& element_type() const noexcept { return m_element_type; }
	size_t elements_count() const noexcept { return m_elements_count; }
};

}

#endif // !__TRANSFORMER_VARIABLE_H__
