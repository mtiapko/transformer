#ifndef __TRANSFORMER_RTTI_DEFS_DEFINE_TYPE_H__
#define __TRANSFORMER_RTTI_DEFS_DEFINE_TYPE_H__

// TODO(FiTH): is this good solution?
// TODO(FiTH): '...' can be 'std::map<T1, T2>' - this is 2! args because of ','
#define RTTI_DEFINE_TYPE(...) static_assert(sizeof(__VA_ARGS__) >= 1)

#endif // !__TRANSFORMER_RTTI_DEFS_DEFINE_TYPE_H__
