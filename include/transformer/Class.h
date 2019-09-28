#ifndef __TRANSFORMER_CLASS_H__
#define __TRANSFORMER_CLASS_H__

#include <vector>
#include "transformer/ClassField.h"

namespace trans
{

class Class : public Entity
{
private:
	std::vector<size_t>     m_base_classes_id;
	std::vector<ClassField> m_fields;

	static CXChildVisitResult visitor(CXCursor cur, CXCursor /* parent */,
		CXClientData closure) noexcept;

public:
	Class(class Parser* parser, CXCursor cur, std::string full_name) noexcept;

	void add_base_class(size_t id) noexcept { m_base_classes_id.emplace_back(id); }
	const auto& base_classes() const noexcept { return m_base_classes_id; }

	auto add_field(CXCursor cur) noexcept { m_fields.emplace_back(cur); }
	const auto& fields() const noexcept { return m_fields; }
};

}

#endif // !__TRANSFORMER_CLASS_H__
