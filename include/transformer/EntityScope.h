#ifndef __TRANSFORMER_ENTITY_SCOPE_H__
#define __TRANSFORMER_ENTITY_SCOPE_H__

#include <vector>
#include <string>
#include <assert.h>
#include <unordered_map>
#include <clang-c/Index.h>

namespace transformer
{

template<typename EntityT>
class EntityScope
{
private:
	std::unordered_map<std::string, size_t> m_entity_by_full_name_lookup;
	std::vector<EntityT>                    m_entities;

public:
	template<typename... Args>
	EntityT& add(std::string full_name, Args... args) noexcept
	{
		/* XXX(FiTH): check 'clang_Cursor_isAnonymus()' before call */
		assert(!full_name.empty());

		auto [it, is_inserted] = m_entity_by_full_name_lookup.try_emplace(
			full_name, m_entities.size()
		);

		if (!is_inserted)
			return m_entities[it->second];

		/* XXX(FiTH): the entity must have the same address during construction */
		auto e = EntityT { std::forward<Args>(args)..., std::move(full_name) };
		return m_entities.emplace_back(std::move(e));
	}

	auto& entities() noexcept { return m_entities; }
	const auto& entities() const noexcept { return m_entities; }

	EntityT* find_by_full_name(const std::string& full_name) noexcept
	{
		auto it = m_entity_by_full_name_lookup.find(full_name);
		if (it != m_entity_by_full_name_lookup.cend())
			return &m_entities[it->second];

		return nullptr;
	}
};

}

#endif // !__TRANSFORMER_ENTITY_SCOPE_H__
