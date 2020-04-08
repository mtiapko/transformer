#ifndef __RTTI_DB_LOG_H__
#define __RTTI_DB_LOG_H__

#define RTTI_DB_LOG_FAILED_TO_SET_VALUE_FMT(variable_type, value) \
	"Failed to assign variable of type '" variable_type \
	"' value of type '", value.type_name(), "': ", value

#endif // !__RTTI_DB_LOG_H__
