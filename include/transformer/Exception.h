#ifndef __TRANSFORMER_EXCEPTION_H__
#define __TRANSFORMER_EXCEPTION_H__

#include <sstream>

#define TRANS_EXCEPTION(...) trans::Exception(__FILE__, __func__, __LINE__, ##__VA_ARGS__)

namespace trans
{

class Exception
{
private:
	std::string_view m_file;
	std::string_view m_func;
	size_t           m_line;

	std::string m_msg;

public:
	template<typename... Args>
	Exception(std::string_view file, std::string_view func, size_t line, Args&&... args) noexcept
		: m_file(file)
		, m_func(func)
		, m_line(line)
	{
		std::stringstream msg;
		(msg << ... << std::forward<Args>(args));
		m_msg = msg.str();
	}

	auto file() const noexcept { return m_file; }
	auto func() const noexcept { return m_func; }
	auto line() const noexcept { return m_line; }

	const auto& msg() const noexcept { return m_msg; }

	friend std::ostream& operator<<(std::ostream& out, const Exception& e) noexcept
	{
		out << "[EXCEPT!] " << e.file() << "::" << e.func()
			<< " (" << e.line() << ") - " << e.msg();

		return out;
	}
};

}

#endif // !__TRANSFORMER_EXCEPTION_H__
