#include "transformer/ClassCallableEntity.h"
#include "transformer/Log.h"

namespace transformer
{

ClassCallableEntity::ClassCallableEntity(CXCursor cur) noexcept
	: Entity(cur)
{
	int num_args = clang_Cursor_getNumArguments(cur);
	// TODO(FiTH): add assert

	if (num_args == -1) {
		TF_LOG_ERROR("Failed to get number of method arguments!");
		return;
	}

	m_args.reserve(num_args);
	for (int i = 0; i < num_args; ++i)
		m_args.emplace_back(clang_Cursor_getArgument(cur, i));
}

}
